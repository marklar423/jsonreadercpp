#include "jsoncpp/states/String.h"

using std::make_unique;
using std::unique_ptr;

namespace jsoncpp::states
{  
    
    unique_ptr<ParserState> CreateValueStringState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::ValueString, 
                                        {
                                            ParserStateTransition(ParserInputSymbol::DoubleQuote, ParserStateType::PostValue)                                                
                                                                    .SetValueAction(ParserValueAction::PushPop, JValueType::String),
                                                
                                            ParserStateTransition(ParserInputSymbol::Backslash, ParserStateType::EscapeChar)
                                                                    .SetStack(ParserStackSymbol::None, ParserStackSymbol::ValueString), 
                                        },
                                         ParserStateTransition(ParserInputSymbol::None, ParserStateType::ValueString)
                                                                .SetCharDestination(ParserCharDestination::Value) ));
    }
    
    unique_ptr<ParserState> CreatePropertyStringState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::PropertyString, 
                                        {
                                            { ParserInputSymbol::DoubleQuote, ParserStateType::PostProperty },
                                                
                                            ParserStateTransition(ParserInputSymbol::Backslash, ParserStateType::EscapeChar)
                                                                    .SetStack(ParserStackSymbol::None, ParserStackSymbol::PropertyString), 
                                        },
                                        ParserStateTransition(ParserInputSymbol::None, ParserStateType::PropertyString)
                                                                .SetCharDestination(ParserCharDestination::Name) ));
    }

    void PopulateEscapeCharStates(ParserState& state, ParserStateType next_state, ParserStackSymbol stack_pop, ParserCharDestination char_dest)
    {
        const ParserInputSymbol inputs[] = {ParserInputSymbol::DoubleQuote, ParserInputSymbol::Backslash, ParserInputSymbol::Forwardslash,
                                            ParserInputSymbol::AlphaB, ParserInputSymbol::AlphaF, ParserInputSymbol::AlphaN, ParserInputSymbol::AlphaR,
                                            ParserInputSymbol::AlphaT};

        for (auto input : inputs)
        {
            state.AddTransition(ParserStateTransition(input, next_state)
                                                .SetStack(stack_pop, ParserStackSymbol::None)
                                                .SetCharDestination(char_dest));
        }
    }

    unique_ptr<ParserState> CreateEscapeCharState()
    {
        auto state_ptr = unique_ptr<ParserState>(new ParserState(ParserStateType::EscapeChar, 
                                        {
                                            ParserStateTransition(ParserInputSymbol::AlphaU, ParserStateType::UnicodeValue)
                                                .SetStack(ParserStackSymbol::ValueString, ParserStackSymbol::First)
                                                .SetCharDestination(ParserCharDestination::Value),
                                                
                                            ParserStateTransition(ParserInputSymbol::AlphaU, ParserStateType::UnicodeProperty)
                                                .SetStack(ParserStackSymbol::PropertyString, ParserStackSymbol::First)
                                                .SetCharDestination(ParserCharDestination::Value),

                                            //more states populated with PopulateEscapeCharStates()

                                        }, { ParserInputSymbol::None, ParserStateType::Error }));

        PopulateEscapeCharStates(*state_ptr, ParserStateType::ValueString, ParserStackSymbol::ValueString, ParserCharDestination::Value);
        PopulateEscapeCharStates(*state_ptr, ParserStateType::PropertyString, ParserStackSymbol::PropertyString, ParserCharDestination::Name);

        return state_ptr;
    }
   
    void PopulateUnicodeStates(ParserState& state, ParserStateType next_state, ParserStackSymbol stack_pop, ParserStackSymbol stack_push, ParserCharDestination char_dest)
    {
        const ParserInputSymbol inputs[] = {ParserInputSymbol::Number, ParserInputSymbol::AlphaA, ParserInputSymbol::AlphaB, ParserInputSymbol::AlphaC,
                                            ParserInputSymbol::AlphaD, ParserInputSymbol::AlphaE, ParserInputSymbol::AlphaF };

        for (auto input : inputs)
        {
            state.AddTransition(ParserStateTransition(input, next_state)
                                                .SetStack(stack_pop, stack_push)
                                                .SetCharDestination(char_dest));
        }
    }

    unique_ptr<ParserState> CreateUnicodeValueState()
    {
        auto state_ptr = unique_ptr<ParserState>(new ParserState(ParserStateType::UnicodeValue, 
                                        {
                                            //states populated with PopulateUnicodeStates()
                                        }, { ParserInputSymbol::None, ParserStateType::Error }));            

        PopulateUnicodeStates(*state_ptr, ParserStateType::UnicodeValue, ParserStackSymbol::First, ParserStackSymbol::Second, ParserCharDestination::Value);
        PopulateUnicodeStates(*state_ptr, ParserStateType::UnicodeValue, ParserStackSymbol::Second, ParserStackSymbol::Third, ParserCharDestination::Value);
        PopulateUnicodeStates(*state_ptr, ParserStateType::UnicodeValue, ParserStackSymbol::Third, ParserStackSymbol::Fourth, ParserCharDestination::Value);
        PopulateUnicodeStates(*state_ptr, ParserStateType::ValueString, ParserStackSymbol::Fourth, ParserStackSymbol::None, ParserCharDestination::Value);

        return state_ptr;
    }

    unique_ptr<ParserState> CreateUnicodePropertyState()
    {
        auto state_ptr = unique_ptr<ParserState>(new ParserState(ParserStateType::UnicodeProperty, 
                                        {
                                            //states populated with PopulateUnicodeStates()
                                        }, { ParserInputSymbol::None, ParserStateType::Error }));            

        PopulateUnicodeStates(*state_ptr, ParserStateType::UnicodeProperty, ParserStackSymbol::First, ParserStackSymbol::Second, ParserCharDestination::Name);
        PopulateUnicodeStates(*state_ptr, ParserStateType::UnicodeProperty, ParserStackSymbol::Second, ParserStackSymbol::Third, ParserCharDestination::Name);
        PopulateUnicodeStates(*state_ptr, ParserStateType::UnicodeProperty, ParserStackSymbol::Third, ParserStackSymbol::Fourth, ParserCharDestination::Name);
        PopulateUnicodeStates(*state_ptr, ParserStateType::PropertyString, ParserStackSymbol::Fourth, ParserStackSymbol::None, ParserCharDestination::Name);

        return state_ptr;
    }
}
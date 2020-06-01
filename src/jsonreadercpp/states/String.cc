#include "jsonreadercpp/states/String.h"

using std::make_unique;
using std::unique_ptr;

namespace jsonreadercpp::states
{  
    
    ParserState CreateValueStringState()
    {
        return ParserState(ParserStateType::ValueString, 
                                        {
                                            ParserStateTransition(ParserInputSymbol::DoubleQuote, ParserStateType::PostValue)                                                
                                                                    .SetValueAction(ParserValueAction::PushPop, JValueType::String),
                                                
                                            ParserStateTransition(ParserInputSymbol::Backslash, ParserStateType::EscapeChar)
                                                                    .SetStack(ParserStackSymbol::None, ParserStackSymbol::ValueString), 
                                        },
                                         ParserStateTransition(ParserInputSymbol::None, ParserStateType::ValueString)
                                                                .SetCharDestination(ParserCharDestination::Value) );
    }
    
    ParserState CreatePropertyStringState()
    {
        return ParserState(ParserStateType::PropertyString, 
                                        {
                                            { ParserInputSymbol::DoubleQuote, ParserStateType::PostProperty },
                                                
                                            ParserStateTransition(ParserInputSymbol::Backslash, ParserStateType::EscapeChar)
                                                                    .SetStack(ParserStackSymbol::None, ParserStackSymbol::PropertyString), 
                                        },
                                        ParserStateTransition(ParserInputSymbol::None, ParserStateType::PropertyString)
                                                                .SetCharDestination(ParserCharDestination::Name) );
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

    ParserState CreateEscapeCharState()
    {
        ParserState state(ParserStateType::EscapeChar, 
                                        {
                                            ParserStateTransition(ParserInputSymbol::AlphaU, ParserStateType::UnicodeValue)
                                                .SetStack(ParserStackSymbol::ValueString, ParserStackSymbol::First)
                                                .SetCharDestination(ParserCharDestination::Value),
                                                
                                            ParserStateTransition(ParserInputSymbol::AlphaU, ParserStateType::UnicodeProperty)
                                                .SetStack(ParserStackSymbol::PropertyString, ParserStackSymbol::First)
                                                .SetCharDestination(ParserCharDestination::Value),

                                            //more states populated with PopulateEscapeCharStates()

                                        }, { ParserInputSymbol::None, ParserStateType::Error });

        PopulateEscapeCharStates(state, ParserStateType::ValueString, ParserStackSymbol::ValueString, ParserCharDestination::Value);
        PopulateEscapeCharStates(state, ParserStateType::PropertyString, ParserStackSymbol::PropertyString, ParserCharDestination::Name);

        return state;
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

    ParserState CreateUnicodeValueState()
    {
        ParserState state(ParserStateType::UnicodeValue, 
                                        {
                                            //states populated with PopulateUnicodeStates()
                                        }, { ParserInputSymbol::None, ParserStateType::Error });            

        PopulateUnicodeStates(state, ParserStateType::UnicodeValue, ParserStackSymbol::First, ParserStackSymbol::Second, ParserCharDestination::Value);
        PopulateUnicodeStates(state, ParserStateType::UnicodeValue, ParserStackSymbol::Second, ParserStackSymbol::Third, ParserCharDestination::Value);
        PopulateUnicodeStates(state, ParserStateType::UnicodeValue, ParserStackSymbol::Third, ParserStackSymbol::Fourth, ParserCharDestination::Value);
        PopulateUnicodeStates(state, ParserStateType::ValueString, ParserStackSymbol::Fourth, ParserStackSymbol::None, ParserCharDestination::Value);

        return state;
    }

    ParserState CreateUnicodePropertyState()
    {
        ParserState state(ParserStateType::UnicodeProperty, 
                                        {
                                            //states populated with PopulateUnicodeStates()
                                        }, { ParserInputSymbol::None, ParserStateType::Error });            

        PopulateUnicodeStates(state, ParserStateType::UnicodeProperty, ParserStackSymbol::First, ParserStackSymbol::Second, ParserCharDestination::Name);
        PopulateUnicodeStates(state, ParserStateType::UnicodeProperty, ParserStackSymbol::Second, ParserStackSymbol::Third, ParserCharDestination::Name);
        PopulateUnicodeStates(state, ParserStateType::UnicodeProperty, ParserStackSymbol::Third, ParserStackSymbol::Fourth, ParserCharDestination::Name);
        PopulateUnicodeStates(state, ParserStateType::PropertyString, ParserStackSymbol::Fourth, ParserStackSymbol::None, ParserCharDestination::Name);

        return state;
    }
}
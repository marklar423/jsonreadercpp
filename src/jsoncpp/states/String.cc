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

    unique_ptr<ParserState> CreateEscapeCharState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::EscapeChar, 
                                        {
                                            ParserStateTransition(ParserInputSymbol::AlphaU, ParserStateType::Unicode)
                                                .SetCharDestination(ParserCharDestination::Value),
                                                

                                            ParserStateTransition(ParserInputSymbol::DoubleQuote, ParserStateType::ValueString)
                                                .SetCharDestination(ParserCharDestination::Value)
                                                .SetStack(ParserStackSymbol::ValueString, ParserStackSymbol::None),

                                            ParserStateTransition(ParserInputSymbol::Backslash, ParserStateType::ValueString)
                                                .SetCharDestination(ParserCharDestination::Value)
                                                .SetStack(ParserStackSymbol::ValueString, ParserStackSymbol::None),

                                            ParserStateTransition(ParserInputSymbol::Forwardslash, ParserStateType::ValueString)
                                                .SetCharDestination(ParserCharDestination::Value)
                                                .SetStack(ParserStackSymbol::ValueString, ParserStackSymbol::None),

                                            ParserStateTransition(ParserInputSymbol::AlphaB, ParserStateType::ValueString)
                                                .SetCharDestination(ParserCharDestination::Value)
                                                .SetStack(ParserStackSymbol::ValueString, ParserStackSymbol::None),

                                            ParserStateTransition(ParserInputSymbol::AlphaF, ParserStateType::ValueString)
                                                .SetCharDestination(ParserCharDestination::Value)
                                                .SetStack(ParserStackSymbol::ValueString, ParserStackSymbol::None),

                                            ParserStateTransition(ParserInputSymbol::AlphaN, ParserStateType::ValueString)
                                                .SetCharDestination(ParserCharDestination::Value)
                                                .SetStack(ParserStackSymbol::ValueString, ParserStackSymbol::None),

                                            ParserStateTransition(ParserInputSymbol::AlphaR, ParserStateType::ValueString)
                                                .SetCharDestination(ParserCharDestination::Value)
                                                .SetStack(ParserStackSymbol::ValueString, ParserStackSymbol::None),

                                            ParserStateTransition(ParserInputSymbol::AlphaT, ParserStateType::ValueString)
                                                .SetCharDestination(ParserCharDestination::Value)
                                                .SetStack(ParserStackSymbol::ValueString, ParserStackSymbol::None),
                                                

                                            ParserStateTransition(ParserInputSymbol::DoubleQuote, ParserStateType::PropertyString)
                                                .SetCharDestination(ParserCharDestination::Name)
                                                .SetStack(ParserStackSymbol::PropertyString, ParserStackSymbol::None),

                                            ParserStateTransition(ParserInputSymbol::Backslash, ParserStateType::PropertyString)
                                                .SetCharDestination(ParserCharDestination::Name)
                                                .SetStack(ParserStackSymbol::PropertyString, ParserStackSymbol::None),

                                            ParserStateTransition(ParserInputSymbol::Forwardslash, ParserStateType::PropertyString)
                                                .SetCharDestination(ParserCharDestination::Name)
                                                .SetStack(ParserStackSymbol::PropertyString, ParserStackSymbol::None),

                                            ParserStateTransition(ParserInputSymbol::AlphaB, ParserStateType::PropertyString)
                                                .SetCharDestination(ParserCharDestination::Name)
                                                .SetStack(ParserStackSymbol::PropertyString, ParserStackSymbol::None),

                                            ParserStateTransition(ParserInputSymbol::AlphaF, ParserStateType::PropertyString)
                                                .SetCharDestination(ParserCharDestination::Name)
                                                .SetStack(ParserStackSymbol::PropertyString, ParserStackSymbol::None),

                                            ParserStateTransition(ParserInputSymbol::AlphaN, ParserStateType::PropertyString)
                                                .SetCharDestination(ParserCharDestination::Name)
                                                .SetStack(ParserStackSymbol::PropertyString, ParserStackSymbol::None),

                                            ParserStateTransition(ParserInputSymbol::AlphaR, ParserStateType::PropertyString)
                                                .SetCharDestination(ParserCharDestination::Name)
                                                .SetStack(ParserStackSymbol::PropertyString, ParserStackSymbol::None),

                                            ParserStateTransition(ParserInputSymbol::AlphaT, ParserStateType::PropertyString)
                                                .SetCharDestination(ParserCharDestination::Name)
                                                .SetStack(ParserStackSymbol::PropertyString, ParserStackSymbol::None),

                                        }, { ParserInputSymbol::None, ParserStateType::Error }));
    }
    

}
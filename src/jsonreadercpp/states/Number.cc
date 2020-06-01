#include "jsonreadercpp/states/Number.h"

using std::make_unique;
using std::unique_ptr;

namespace jsonreadercpp::states
{    
    ParserState CreateIntState()
    {
        return ParserState(ParserStateType::Int, 
                                        {   
                                            ParserStateTransition( ParserInputSymbol::Number, ParserStateType::Int)
                                                .SetCharDestination(ParserCharDestination::Value),

                                            ParserStateTransition( ParserInputSymbol::Comma, ParserStateType::Value)
                                                .SetStack(ParserStackSymbol::Array, ParserStackSymbol::Array)
                                                .SetValueAction(ParserValueAction::PushPop, JValueType::Number),

                                            ParserStateTransition( ParserInputSymbol::Comma, ParserStateType::Object)
                                                .SetStack(ParserStackSymbol::Object, ParserStackSymbol::Object)
                                                .SetValueAction(ParserValueAction::PushPop, JValueType::Number),

                                            ParserStateTransition( ParserInputSymbol::Period, ParserStateType::Double)
                                                .SetCharDestination(ParserCharDestination::Value),

                                            ParserStateTransition( ParserInputSymbol::CloseBracket, ParserStateType::PostIntParent)
                                                .SetStack(ParserStackSymbol::Array, ParserStackSymbol::Array)
                                                .SetValueAction(ParserValueAction::PushPop, JValueType::Number),

                                            ParserStateTransition( ParserInputSymbol::CloseBrace, ParserStateType::PostIntParent)
                                                .SetStack(ParserStackSymbol::Object, ParserStackSymbol::Object)
                                                .SetValueAction(ParserValueAction::PushPop, JValueType::Number),
                                                
                                            ParserStateTransition( ParserInputSymbol::Whitespace, ParserStateType::PostValue)
                                                .SetValueAction(ParserValueAction::PushPop, JValueType::Number),

                                        }, { ParserInputSymbol::None, ParserStateType::Error });
    }
    
    ParserState CreatePostIntParentState()
    {
        return ParserState(ParserStateType::PostIntParent, 
                                        {   
                                            ParserStateTransition( ParserInputSymbol::None, ParserStateType::PostValue)
                                                .SetStack(ParserStackSymbol::Array, ParserStackSymbol::None)
                                                .SetValueAction(ParserValueAction::Pop, {}),

                                            ParserStateTransition( ParserInputSymbol::None, ParserStateType::PostObject)
                                                .SetStack(ParserStackSymbol::Object, ParserStackSymbol::None)
                                                .SetValueAction(ParserValueAction::Pop, {})

                                        }, { ParserInputSymbol::None, ParserStateType::Error });
    }

    ParserState CreateDoubleState()
    {
        return ParserState(ParserStateType::Double, 
                                        { 
                                            ParserStateTransition(ParserInputSymbol::Number, ParserStateType::Double)
                                                .SetCharDestination(ParserCharDestination::Value),
                                                
                                            ParserStateTransition(ParserInputSymbol::AlphaE, ParserStateType::ExponentSign)
                                                .SetCharDestination(ParserCharDestination::Value),
                                                
                                            ParserStateTransition(ParserInputSymbol::AlphaECap, ParserStateType::ExponentSign)
                                                .SetCharDestination(ParserCharDestination::Value),
                                                
                                            ParserStateTransition(ParserInputSymbol::Whitespace, ParserStateType::PostValue)
                                                .SetValueAction(ParserValueAction::PushPop, JValueType::Number),
                                                
                                            ParserStateTransition(ParserInputSymbol::Comma, ParserStateType::Object)
                                                .SetStack(ParserStackSymbol::Object, ParserStackSymbol::Object)
                                                .SetValueAction(ParserValueAction::PushPop, JValueType::Number),
                                                
                                            ParserStateTransition(ParserInputSymbol::Comma, ParserStateType::Value)
                                                .SetStack(ParserStackSymbol::Array, ParserStackSymbol::Array)
                                                .SetValueAction(ParserValueAction::PushPop, JValueType::Number),

                                            ParserStateTransition(ParserInputSymbol::CloseBrace, ParserStateType::PostIntParent)
                                                .SetStack(ParserStackSymbol::Object, ParserStackSymbol::Object)
                                                .SetValueAction(ParserValueAction::PushPop, JValueType::Number),
                                                
                                            ParserStateTransition(ParserInputSymbol::CloseBracket, ParserStateType::PostIntParent)
                                                .SetStack(ParserStackSymbol::Array, ParserStackSymbol::Array)
                                                .SetValueAction(ParserValueAction::PushPop, JValueType::Number),
                                            
                                        }, { ParserInputSymbol::None, ParserStateType::Error });
    }

    ParserState CreateExponentSignState()
    {
        return ParserState(ParserStateType::ExponentSign, 
                                        { 
                                            ParserStateTransition(ParserInputSymbol::Plus, ParserStateType::ExponentDigits)
                                                .SetCharDestination(ParserCharDestination::Value),

                                            ParserStateTransition(ParserInputSymbol::Dash, ParserStateType::ExponentDigits)
                                                .SetCharDestination(ParserCharDestination::Value) 

                                        }, { ParserInputSymbol::None, ParserStateType::Error });
    }

    ParserState CreateExponentDigitsState()
    {  
        return ParserState(ParserStateType::ExponentDigits, 
                                        {       
                                            ParserStateTransition(ParserInputSymbol::Number, ParserStateType::ExponentDigits)
                                                .SetCharDestination(ParserCharDestination::Value),

                                            ParserStateTransition(ParserInputSymbol::Whitespace, ParserStateType::PostValue)
                                                .SetValueAction(ParserValueAction::PushPop, JValueType::Number),

                                            ParserStateTransition(ParserInputSymbol::Comma, ParserStateType::Object)
                                                .SetStack(ParserStackSymbol::Object, ParserStackSymbol::Object)
                                                .SetValueAction(ParserValueAction::PushPop, JValueType::Number),
                                                
                                            ParserStateTransition(ParserInputSymbol::Comma, ParserStateType::Value)
                                                .SetStack(ParserStackSymbol::Array, ParserStackSymbol::Array)
                                                .SetValueAction(ParserValueAction::PushPop, JValueType::Number),
                                                
                                            ParserStateTransition(ParserInputSymbol::CloseBrace, ParserStateType::PostIntParent)
                                                .SetStack(ParserStackSymbol::Object, ParserStackSymbol::Object)
                                                .SetValueAction(ParserValueAction::PushPop, JValueType::Number),
                                                
                                            ParserStateTransition(ParserInputSymbol::CloseBracket, ParserStateType::PostIntParent)
                                                .SetStack(ParserStackSymbol::Array, ParserStackSymbol::Array)
                                                .SetValueAction(ParserValueAction::PushPop, JValueType::Number),

                                        }, { ParserInputSymbol::None, ParserStateType::Error });
    }
}
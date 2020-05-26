#include "ParserStateImpls.h"

#include <vector>
#include <array>

#include "ParserStateTransition.h"

using std::make_unique;
using std::unique_ptr;
using std::vector;
using std::unordered_map;
using std::array;


namespace jsoncpp 
{   
    unique_ptr<ParserState> CreateStartState();
    unique_ptr<ParserState> CreateFinishState();
    unique_ptr<ParserState> CreateRootObjectState();
    unique_ptr<ParserState> CreateObjectState();
    unique_ptr<ParserState> CreatePostObjectState();
    unique_ptr<ParserState> CreateValueStringState();
    unique_ptr<ParserState> CreatePropertyStringState();
    unique_ptr<ParserState> CreatePostPropertyState();
    unique_ptr<ParserState> CreateValueState();
    unique_ptr<ParserState> CreatePostValueState();
    unique_ptr<ParserState> CreateIntState();
    unique_ptr<ParserState> CreatePostIntParentState();
    unique_ptr<ParserState> CreateDoubleState();
    unique_ptr<ParserState> CreateExponentSignState();
    unique_ptr<ParserState> CreateExponentDigitsState();
    unique_ptr<ParserState> CreateTrueState();
    unique_ptr<ParserState> CreateFalseState();
    unique_ptr<ParserState> CreateNullState();

    unordered_map<ParserStateType, unique_ptr<ParserState>> CreateStatesMap()
    {
        unique_ptr<ParserState> states[] {
            CreateStartState(),
            CreateFinishState(),
            CreateRootObjectState(),
            CreateObjectState(),
            CreatePostObjectState(),
            CreateValueStringState(),
            CreatePropertyStringState(),
            CreatePostPropertyState(),
            CreateValueState(),
            CreatePostValueState(),
            CreateIntState(),
            CreatePostIntParentState(),           
            CreateDoubleState(),
            CreateExponentSignState(),
            CreateExponentDigitsState(),
            CreateTrueState(),
            CreateFalseState(),
            CreateNullState()
        };

        unordered_map<ParserStateType, unique_ptr<ParserState>> states_map;

        for (auto& state : states)
        {
            auto state_type = state->GetStateType();
            states_map.emplace(state_type, std::move(state));
        }

        return states_map;
    }   


    unique_ptr<ParserState> CreateStartState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::Start, 
                                        {
                                            ParserStateTransition(ParserInputSymbol::None, ParserStateType::RootObject)
                                                                    .SetStack(ParserStackSymbol::None, ParserStackSymbol::Start)
                                        }));
    }

    unique_ptr<ParserState> CreateFinishState()
    {
       return unique_ptr<ParserState>(new ParserState(ParserStateType::Finish, 
                                        {
                                            { ParserInputSymbol::Whitespace, ParserStateType::Finish }
                                        },
                                        { ParserInputSymbol::None, ParserStateType::Error }));
      
    }

    unique_ptr<ParserState> CreateRootObjectState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::RootObject, 
                                        {
                                            { ParserInputSymbol::Whitespace, ParserStateType::RootObject },

                                            ParserStateTransition(ParserInputSymbol::OpenBrace, ParserStateType::Object)
                                                                   .SetStack(ParserStackSymbol::None, ParserStackSymbol::Object)
                                                                   .SetValueAction(ParserValueAction::Push, JValueType::Object)
                                        },
                                        { ParserInputSymbol::None, ParserStateType::Error } ));
    }

    unique_ptr<ParserState> CreateObjectState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::Object, 
                                        {
                                            { ParserInputSymbol::Whitespace, ParserStateType::Object },

                                            ParserStateTransition(ParserInputSymbol::CloseBrace, ParserStateType::PostObject)
                                                                    .SetStack(ParserStackSymbol::Object, ParserStackSymbol::None)
                                                                    .SetValueAction(ParserValueAction::Pop, {}),
                                                
                                            { ParserInputSymbol::DoubleQuote, ParserStateType::PropertyString }
                                        },
                                        { ParserInputSymbol::None, ParserStateType::Error } ));
    }
    
    unique_ptr<ParserState> CreatePostObjectState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::PostObject, 
                                        {
                                            ParserStateTransition(ParserInputSymbol::None, ParserStateType::Finish)
                                                .SetStack(ParserStackSymbol::Start, ParserStackSymbol::None ),

                                            ParserStateTransition(ParserInputSymbol::None, ParserStateType::PostValue)
                                                .SetStack(ParserStackSymbol::Object, ParserStackSymbol::Object ),

                                            ParserStateTransition(ParserInputSymbol::None, ParserStateType::PostValue)
                                                .SetStack(ParserStackSymbol::Array, ParserStackSymbol::Array )
                                        }));
    }
    
    unique_ptr<ParserState> CreateValueStringState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::ValueString, 
                                        {
                                            ParserStateTransition(ParserInputSymbol::DoubleQuote, ParserStateType::PostValue)                                                
                                                                    .SetValueAction(ParserValueAction::PushPop, JValueType::String),
                                                
                                            ParserStateTransition(ParserInputSymbol::Backslash, ParserStateType::EscapeChar)
                                                                    .SetStack(ParserStackSymbol::ValueString, ParserStackSymbol::None)
                                                                    .SetCharDestination(ParserCharDestination::Value), 
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
                                                                    .SetStack(ParserStackSymbol::PropertyString, ParserStackSymbol::None)
                                                                    .SetCharDestination(ParserCharDestination::Value), 
                                        },
                                        ParserStateTransition(ParserInputSymbol::None, ParserStateType::PropertyString)
                                                                .SetCharDestination(ParserCharDestination::Name) ));
    }

    unique_ptr<ParserState> CreatePostPropertyState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::PostProperty, 
                                        {
                                            { ParserInputSymbol::Whitespace, ParserStateType::PostProperty },    
                                            { ParserInputSymbol::Colon, ParserStateType::Value },

                                        }, { ParserInputSymbol::None, ParserStateType::Error }));
    }

    unique_ptr<ParserState> CreateValueState()
    {
         return unique_ptr<ParserState>(new ParserState(ParserStateType::Value, 
                                        {                                            
                                            { ParserInputSymbol::Whitespace, ParserStateType::Value },                                            
                                            { ParserInputSymbol::DoubleQuote, ParserStateType::ValueString },      
                                                                      
                                            ParserStateTransition( ParserInputSymbol::Number, ParserStateType::Int )
                                                .SetCharDestination(ParserCharDestination::Value), 
                                                                      
                                            ParserStateTransition( ParserInputSymbol::Dash, ParserStateType::Int )
                                                .SetCharDestination(ParserCharDestination::Value),
                                            
                                            ParserStateTransition( ParserInputSymbol::AlphaF, ParserStateType::False )      
                                                .SetStack(ParserStackSymbol::AlphaF, ParserStackSymbol::None)                                     
                                                .SetCharDestination(ParserCharDestination::Value),

                                            ParserStateTransition( ParserInputSymbol::AlphaT, ParserStateType::True )   
                                                .SetStack(ParserStackSymbol::AlphaT, ParserStackSymbol::None)          
                                                .SetCharDestination(ParserCharDestination::Value),

                                            ParserStateTransition( ParserInputSymbol::AlphaN, ParserStateType::Null )
                                                .SetStack(ParserStackSymbol::AlphaN, ParserStackSymbol::None),            
                                            
                                            ParserStateTransition(ParserInputSymbol::OpenBracket, ParserStateType::Value)
                                                .SetStack(ParserStackSymbol::None, ParserStackSymbol::Array)
                                                .SetValueAction(ParserValueAction::Push, JValueType::Array),

                                            ParserStateTransition( ParserInputSymbol::CloseBracket, ParserStateType::PostValue)
                                                .SetStack(ParserStackSymbol::Array, ParserStackSymbol::None)
                                                .SetValueAction(ParserValueAction::Pop, {}),

                                            ParserStateTransition(ParserInputSymbol::OpenBrace, ParserStateType::Object)
                                                .SetStack(ParserStackSymbol::None, ParserStackSymbol::Object)
                                                .SetValueAction(ParserValueAction::Push, JValueType::Object)

                                        }, { ParserInputSymbol::None, ParserStateType::Error }));
    }
    
    unique_ptr<ParserState> CreatePostValueState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::PostValue, 
                                        {
                                            { ParserInputSymbol::Whitespace, ParserStateType::PostValue },     
                                                
                                            ParserStateTransition( ParserInputSymbol::Comma, ParserStateType::Value)
                                                .SetStack(ParserStackSymbol::Array, ParserStackSymbol::Array ),
                                                
                                            ParserStateTransition( ParserInputSymbol::Comma, ParserStateType::Object)
                                                .SetStack(ParserStackSymbol::Object, ParserStackSymbol::Object ),

                                            ParserStateTransition( ParserInputSymbol::CloseBracket, ParserStateType::PostValue)
                                                .SetStack(ParserStackSymbol::Array, ParserStackSymbol::None)
                                                .SetValueAction(ParserValueAction::Pop, {}),

                                            ParserStateTransition( ParserInputSymbol::CloseBrace, ParserStateType::PostObject)
                                                .SetStack(ParserStackSymbol::Object, ParserStackSymbol::None)
                                                .SetValueAction(ParserValueAction::Pop, {})

                                        }, { ParserInputSymbol::None, ParserStateType::Error }));
    }


    unique_ptr<ParserState> CreateIntState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::Int, 
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

                                        }, { ParserInputSymbol::None, ParserStateType::Error }));
    }
    
    unique_ptr<ParserState> CreatePostIntParentState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::PostIntParent, 
                                        {   
                                            ParserStateTransition( ParserInputSymbol::None, ParserStateType::PostValue)
                                                .SetStack(ParserStackSymbol::Array, ParserStackSymbol::None)
                                                .SetValueAction(ParserValueAction::Pop, {}),

                                            ParserStateTransition( ParserInputSymbol::None, ParserStateType::PostObject)
                                                .SetStack(ParserStackSymbol::Object, ParserStackSymbol::None)
                                                .SetValueAction(ParserValueAction::Pop, {})

                                        }, { ParserInputSymbol::None, ParserStateType::Error }));
    }

    unique_ptr<ParserState> CreateDoubleState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::Double, 
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
                                            
                                        }, { ParserInputSymbol::None, ParserStateType::Error }));
    }

    unique_ptr<ParserState> CreateExponentSignState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::ExponentSign, 
                                        { 
                                            ParserStateTransition(ParserInputSymbol::Plus, ParserStateType::ExponentDigits)
                                                .SetCharDestination(ParserCharDestination::Value),

                                            ParserStateTransition(ParserInputSymbol::Dash, ParserStateType::ExponentDigits)
                                                .SetCharDestination(ParserCharDestination::Value) 

                                        }, { ParserInputSymbol::None, ParserStateType::Error }));
    }

    unique_ptr<ParserState> CreateExponentDigitsState()
    {  
        return unique_ptr<ParserState>(new ParserState(ParserStateType::ExponentDigits, 
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

                                        }, { ParserInputSymbol::None, ParserStateType::Error }));
    }

    unique_ptr<ParserState> CreateTrueState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::True, 
                                        { 
                                            ParserStateTransition(ParserInputSymbol::AlphaR, ParserStateType::True)
                                                .SetStack(ParserStackSymbol::AlphaT, ParserStackSymbol::AlphaR)
                                                .SetCharDestination(ParserCharDestination::Value),

                                            ParserStateTransition(ParserInputSymbol::AlphaU, ParserStateType::True)
                                                .SetStack(ParserStackSymbol::AlphaR, ParserStackSymbol::AlphaU)
                                                .SetCharDestination(ParserCharDestination::Value),

                                            ParserStateTransition(ParserInputSymbol::AlphaE, ParserStateType::PostValue)
                                                .SetStack(ParserStackSymbol::AlphaU, ParserStackSymbol::None)
                                                .SetCharDestination(ParserCharDestination::Value) 
                                                .SetValueAction(ParserValueAction::PushPop, JValueType::Boolean)

                                        }, { ParserInputSymbol::None, ParserStateType::Error }));
    }
    
    unique_ptr<ParserState> CreateFalseState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::False, 
                                        { 
                                            ParserStateTransition(ParserInputSymbol::AlphaA, ParserStateType::False)
                                                .SetStack(ParserStackSymbol::AlphaF, ParserStackSymbol::AlphaA)
                                                .SetCharDestination(ParserCharDestination::Value),

                                            ParserStateTransition(ParserInputSymbol::AlphaL, ParserStateType::False)
                                                .SetStack(ParserStackSymbol::AlphaA, ParserStackSymbol::AlphaL)
                                                .SetCharDestination(ParserCharDestination::Value),

                                            ParserStateTransition(ParserInputSymbol::AlphaS, ParserStateType::False)
                                                .SetStack(ParserStackSymbol::AlphaL, ParserStackSymbol::AlphaS)
                                                .SetCharDestination(ParserCharDestination::Value),

                                            ParserStateTransition(ParserInputSymbol::AlphaE, ParserStateType::PostValue)
                                                .SetStack(ParserStackSymbol::AlphaS, ParserStackSymbol::None)
                                                .SetCharDestination(ParserCharDestination::Value) 
                                                .SetValueAction(ParserValueAction::PushPop, JValueType::Boolean)

                                        }, { ParserInputSymbol::None, ParserStateType::Error }));
    }
  
    unique_ptr<ParserState> CreateNullState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::Null, 
                                        { 
                                            ParserStateTransition(ParserInputSymbol::AlphaU, ParserStateType::Null)
                                                .SetStack(ParserStackSymbol::AlphaN, ParserStackSymbol::AlphaU),

                                            ParserStateTransition(ParserInputSymbol::AlphaL, ParserStateType::Null)
                                                .SetStack(ParserStackSymbol::AlphaU, ParserStackSymbol::AlphaL),
                                                
                                            ParserStateTransition(ParserInputSymbol::AlphaL, ParserStateType::PostValue)
                                                .SetStack(ParserStackSymbol::AlphaL, ParserStackSymbol::None)
                                                .SetValueAction(ParserValueAction::PushPop, JValueType::Null)

                                        }, { ParserInputSymbol::None, ParserStateType::Error }));
    }    
}
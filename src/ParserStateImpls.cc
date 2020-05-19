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
            CreatePostValueState()
        };

        unordered_map<ParserStateType, unique_ptr<ParserState>> states_map;

        for (auto& state : states)
        {
            auto state_type = state->GetStateType();
            states_map.emplace(state_type, std::move(state));
        }

        return std::move(states_map);
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
                                                
                                            { ParserInputSymbol::Backslash, ParserStateType::EscapeChar }, 
                                        },
                                         ParserStateTransition(ParserInputSymbol::None, ParserStateType::ValueString)
                                                                .SetCharDestination(ParserCharDestination::Value) ));
    }
    
    unique_ptr<ParserState> CreatePropertyStringState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::PropertyString, 
                                        {
                                            { ParserInputSymbol::DoubleQuote, ParserStateType::PostProperty },                                                
                                            { ParserInputSymbol::Backslash, ParserStateType::EscapeChar }, 
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
                                            { ParserInputSymbol::AlphaF, ParserStateType::False },                                            
                                            { ParserInputSymbol::AlphaT, ParserStateType::True },                                            
                                            { ParserInputSymbol::AlphaN, ParserStateType::Null },                                            
                                            { ParserInputSymbol::Number, ParserStateType::Int },

                                            ParserStateTransition(ParserInputSymbol::OpenBracket, ParserStateType::Array)
                                                .SetStack(ParserStackSymbol::None, ParserStackSymbol::Array)
                                                .SetValueAction(ParserValueAction::Push, JValueType::Array),

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
}
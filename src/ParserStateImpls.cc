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
    unique_ptr<ParserState> CreatePostPropertyState();
    unique_ptr<ParserState> CreatePostValueState();

    unordered_map<ParserStateType, unique_ptr<ParserState>> CreateStatesMap()
    {
        unique_ptr<ParserState> states[] {
            CreateStartState(),
            CreateFinishState(),
            CreateRootObjectState(),
            CreateObjectState(),
            CreatePostObjectState(),
            CreatePostPropertyState(),
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
                                            { ParserInputSymbol::None, ParserStateType::RootObject,
                                                ParserStackSymbol::None, ParserStackSymbol::Start }
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

                                            { ParserInputSymbol::OpenBrace, ParserStateType::Object,
                                                ParserStackSymbol::None, ParserStackSymbol::Object,
                                                ParserValueAction::Push, JValueType::Object }
                                        },
                                        { ParserInputSymbol::None, ParserStateType::Error } ));
    }

    unique_ptr<ParserState> CreateObjectState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::Object, 
                                        {
                                            { ParserInputSymbol::Whitespace, ParserStateType::Object },

                                            { ParserInputSymbol::CloseBrace, ParserStateType::PostObject,
                                                ParserStackSymbol::Object, ParserStackSymbol::None,
                                                ParserValueAction::Pop, JValueType::Null },
                                                
                                            { ParserInputSymbol::DoubleQuote, ParserStateType::String,
                                                ParserStackSymbol::None, ParserStackSymbol::Property }
                                        },
                                        { ParserInputSymbol::None, ParserStateType::Error } ));
    }
    
    unique_ptr<ParserState> CreatePostObjectState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::PostObject, 
                                        {
                                            { ParserInputSymbol::None, ParserStateType::Finish,
                                                ParserStackSymbol::Start, ParserStackSymbol::None },

                                            { ParserInputSymbol::None, ParserStateType::PostValue,
                                                ParserStackSymbol::Object, ParserStackSymbol::Object },

                                            { ParserInputSymbol::None, ParserStateType::PostValue,
                                                ParserStackSymbol::Array, ParserStackSymbol::Array }
                                        }));
    }

    unique_ptr<ParserState> CreatePostPropertyState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::PostProperty, 
                                        {
                                            { ParserInputSymbol::Whitespace, ParserStateType::PostProperty },     
                                                
                                            { ParserInputSymbol::Colon, ParserStateType::Value,
                                                ParserStackSymbol::Property, ParserStackSymbol::None },

                                        }, { ParserInputSymbol::None, ParserStateType::Error }));
    }

    
    unique_ptr<ParserState> CreatePostValueState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::PostValue, 
                                        {
                                            { ParserInputSymbol::Whitespace, ParserStateType::PostValue },     
                                                
                                            { ParserInputSymbol::Comma, ParserStateType::Value,
                                                ParserStackSymbol::Array, ParserStackSymbol::Array },
                                                
                                            { ParserInputSymbol::Comma, ParserStateType::Object,
                                                ParserStackSymbol::Object, ParserStackSymbol::Object },

                                            { ParserInputSymbol::CloseBracket, ParserStateType::PostValue,
                                                ParserStackSymbol::Array, ParserStackSymbol::None,
                                                ParserCharDestination::None,
                                                ParserValueAction::Pop, JValueType::Null },

                                            { ParserInputSymbol::CloseBrace, ParserStateType::PostObject,
                                                ParserStackSymbol::Object, ParserStackSymbol::None,
                                                ParserCharDestination::None,
                                                ParserValueAction::Pop, JValueType::Null }

                                        }, { ParserInputSymbol::None, ParserStateType::Error }));
    }
}
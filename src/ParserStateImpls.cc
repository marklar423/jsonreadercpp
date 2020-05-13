#include "ParserStateImpls.h"

#include <vector>
#include <initializer_list>

#include "ParserStateTransition.h"

using std::make_unique;
using std::unique_ptr;
using std::vector;
using std::unordered_map;
using std::initializer_list;

namespace jsoncpp 
{    
    unique_ptr<ParserState> CreateStartState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::Start, 
                                        {
                                            { ParserInputSymbol::None, ParserStateType::RootObject,
                                                ParserStackSymbol::None, ParserStackSymbol::Start }
                                        }));
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

    unordered_map<ParserStateType, unique_ptr<ParserState>> CreateStatesMap()
    {
        initializer_list<unique_ptr<ParserState>> states {
            CreateStartState(),
            CreateRootObjectState()
        };

        unordered_map<ParserStateType, unique_ptr<ParserState>> states_map;

        for (auto& state : states)
        {
            auto state_type = state->GetStateType();
            states_map.emplace(state_type, std::move(state));
        }

        return std::move(states_map);
    }   
}
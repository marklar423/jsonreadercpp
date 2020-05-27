#include "jsoncpp/states/ParserStateImpls.h"

#include <vector>
#include <array>

#include "jsoncpp/ParserStateTransition.h"

#include "jsoncpp/states/BoolAndNull.h"
#include "jsoncpp/states/Number.h"
#include "jsoncpp/states/Object.h"
#include "jsoncpp/states/Start.h"
#include "jsoncpp/states/String.h"
#include "jsoncpp/states/ValueAndArray.h"

using std::unique_ptr;
using std::vector;
using std::unordered_map;
using std::array;

namespace jsoncpp::states
{   

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
            CreateEscapeCharState(), 
            CreateUnicodeValueState(), 
            CreateUnicodePropertyState(),
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
}
#include "jsonreadercpp/states/ParserStateImpls.h"

#include <vector>
#include <array>

#include "jsonreadercpp/statemachine/ParserStateTransition.h"

#include "jsonreadercpp/states/BoolAndNull.h"
#include "jsonreadercpp/states/Number.h"
#include "jsonreadercpp/states/Object.h"
#include "jsonreadercpp/states/Start.h"
#include "jsonreadercpp/states/String.h"
#include "jsonreadercpp/states/ValueAndArray.h"

using std::unique_ptr;
using std::vector;
using std::unordered_map;
using std::array;

namespace jsonreadercpp::states
{   

    unordered_map<ParserStateType, ParserState> CreateStatesMap()
    {
        ParserState states[] {
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

        unordered_map<ParserStateType, ParserState> states_map;

        for (auto& state : states)
        {
            auto state_type = state.GetStateType();
            states_map.emplace(state_type, std::move(state));
        }

        return states_map;
    }   
}
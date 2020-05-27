#ifndef JSONCPP_STATES_VALUE_AND_ARRAY_H
#define JSONCPP_STATES_VALUE_AND_ARRAY_H

#include <unordered_map>
#include <memory>

#include "jsoncpp/ParserStatesSymbols.h"
#include "jsoncpp/ParserState.h"

namespace jsoncpp::states
{       
    std::unique_ptr<ParserState> CreateValueState();
    std::unique_ptr<ParserState> CreatePostValueState();    
}


#endif
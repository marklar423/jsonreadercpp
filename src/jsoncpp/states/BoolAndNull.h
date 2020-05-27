#ifndef JSONCPP_STATES_BOOL_AND_NULL_H
#define JSONCPP_STATES_BOOL_AND_NULL_H

#include <unordered_map>
#include <memory>

#include "jsoncpp/ParserStatesSymbols.h"
#include "jsoncpp/ParserState.h"

namespace jsoncpp::states
{   
    std::unique_ptr<ParserState> CreateTrueState();
    std::unique_ptr<ParserState> CreateFalseState();
    std::unique_ptr<ParserState> CreateNullState();
}


#endif
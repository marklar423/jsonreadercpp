#ifndef JSONCPP_STATES_NUMBER_H
#define JSONCPP_STATES_NUMBER_H

#include <unordered_map>
#include <memory>

#include "jsoncpp/ParserStatesSymbols.h"
#include "jsoncpp/ParserState.h"

namespace jsoncpp::states
{       
    std::unique_ptr<ParserState> CreateIntState();
    std::unique_ptr<ParserState> CreatePostIntParentState();
    std::unique_ptr<ParserState> CreateDoubleState();
    std::unique_ptr<ParserState> CreateExponentSignState();    
    std::unique_ptr<ParserState> CreateExponentDigitsState();
}


#endif
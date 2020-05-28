#ifndef JSONREADERCPP_STATES_NUMBER_H
#define JSONREADERCPP_STATES_NUMBER_H

#include <unordered_map>
#include <memory>

#include "jsonreadercpp/ParserStatesSymbols.h"
#include "jsonreadercpp/ParserState.h"

namespace jsonreadercpp::states
{       
    std::unique_ptr<ParserState> CreateIntState();
    std::unique_ptr<ParserState> CreatePostIntParentState();
    std::unique_ptr<ParserState> CreateDoubleState();
    std::unique_ptr<ParserState> CreateExponentSignState();    
    std::unique_ptr<ParserState> CreateExponentDigitsState();
}


#endif
#ifndef JSONREADERCPP_STATES_BOOL_AND_NULL_H
#define JSONREADERCPP_STATES_BOOL_AND_NULL_H

#include <unordered_map>
#include <memory>

#include "jsonreadercpp/ParserStatesSymbols.h"
#include "jsonreadercpp/ParserState.h"

namespace jsonreadercpp::states
{   
    std::unique_ptr<ParserState> CreateTrueState();
    std::unique_ptr<ParserState> CreateFalseState();
    std::unique_ptr<ParserState> CreateNullState();
}


#endif
#ifndef JSONREADERCPP_STATES_VALUE_AND_ARRAY_H
#define JSONREADERCPP_STATES_VALUE_AND_ARRAY_H

#include <unordered_map>
#include <memory>

#include "jsonreadercpp/ParserStatesSymbols.h"
#include "jsonreadercpp/ParserState.h"

namespace jsonreadercpp::states
{       
    std::unique_ptr<ParserState> CreateValueState();
    std::unique_ptr<ParserState> CreatePostValueState();    
}


#endif
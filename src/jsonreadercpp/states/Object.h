#ifndef JSONREADERCPP_STATES_OBJECT_H
#define JSONREADERCPP_STATES_OBJECT_H

#include <unordered_map>
#include <memory>

#include "jsonreadercpp/statemachine/ParserStatesSymbols.h"
#include "jsonreadercpp/statemachine/ParserState.h"

namespace jsonreadercpp::states
{       
    std::unique_ptr<ParserState> CreateObjectState();
    std::unique_ptr<ParserState> CreatePostObjectState();
    std::unique_ptr<ParserState> CreatePostPropertyState();    
}


#endif
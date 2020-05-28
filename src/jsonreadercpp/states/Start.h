#ifndef JSONREADERCPP_STATES_START_H
#define JSONREADERCPP_STATES_START_H

#include <unordered_map>
#include <memory>

#include "jsonreadercpp/ParserStatesSymbols.h"
#include "jsonreadercpp/ParserState.h"

namespace jsonreadercpp::states
{   
    std::unique_ptr<ParserState> CreateStartState();
    std::unique_ptr<ParserState> CreateFinishState();
    std::unique_ptr<ParserState> CreateRootObjectState();
}

#endif
#ifndef JSONREADERCPP_STATES_START_H
#define JSONREADERCPP_STATES_START_H

#include <unordered_map>
#include <memory>

#include "jsonreadercpp/statemachine/ParserStatesSymbols.h"
#include "jsonreadercpp/statemachine/ParserState.h"

namespace jsonreadercpp::states
{   
    ParserState CreateStartState();
    ParserState CreateFinishState();
    ParserState CreateRootObjectState();
}

#endif
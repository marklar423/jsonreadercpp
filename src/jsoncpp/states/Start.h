#ifndef JSONCPP_STATES_START_H
#define JSONCPP_STATES_START_H

#include <unordered_map>
#include <memory>

#include "jsoncpp/ParserStatesSymbols.h"
#include "jsoncpp/ParserState.h"

namespace jsoncpp::states
{   
    std::unique_ptr<ParserState> CreateStartState();
    std::unique_ptr<ParserState> CreateFinishState();
    std::unique_ptr<ParserState> CreateRootObjectState();
}

#endif
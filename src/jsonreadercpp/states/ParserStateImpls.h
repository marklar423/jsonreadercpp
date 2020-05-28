#ifndef JSONREADERCPP_PARSERSTATEROOT_H
#define JSONREADERCPP_PARSERSTATEROOT_H

#include <unordered_map>
#include <memory>

#include "jsonreadercpp/statemachine/ParserStatesSymbols.h"
#include "jsonreadercpp/statemachine/ParserState.h"

namespace jsonreadercpp::states
{
    std::unordered_map<ParserStateType, std::unique_ptr<ParserState>> CreateStatesMap();
}

#endif
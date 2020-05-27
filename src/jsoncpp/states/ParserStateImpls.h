#ifndef JSONCPP_PARSERSTATEROOT_H
#define JSONCPP_PARSERSTATEROOT_H

#include <unordered_map>
#include <memory>

#include "jsoncpp/ParserStatesSymbols.h"
#include "jsoncpp/ParserState.h"

namespace jsoncpp::states
{
    std::unordered_map<ParserStateType, std::unique_ptr<ParserState>> CreateStatesMap();
}

#endif
#ifndef JSONCPP_PARSERSTATEROOT_H
#define JSONCPP_PARSERSTATEROOT_H

#include <unordered_map>
#include <memory>

#include "ParserStatesSymbols.h"
#include "ParserState.h"

namespace jsoncpp 
{
    std::unordered_map<ParserStateType, std::unique_ptr<ParserState>> CreateStatesMap();
}

#endif
#ifndef JSONREADERCPP_STATES_BOOL_AND_NULL_H
#define JSONREADERCPP_STATES_BOOL_AND_NULL_H

#include <unordered_map>
#include <memory>

#include "jsonreadercpp/statemachine/ParserStatesSymbols.h"
#include "jsonreadercpp/statemachine/ParserState.h"

namespace jsonreadercpp::states
{   
    ParserState CreateTrueState();
    ParserState CreateFalseState();
    ParserState CreateNullState();
}


#endif
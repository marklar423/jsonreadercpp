#ifndef JSONREADERCPP_STATES_VALUE_AND_ARRAY_H
#define JSONREADERCPP_STATES_VALUE_AND_ARRAY_H

#include <unordered_map>
#include <memory>

#include "jsonreadercpp/statemachine/ParserStatesSymbols.h"
#include "jsonreadercpp/statemachine/ParserState.h"

namespace jsonreadercpp::states
{       
    ParserState CreateValueState();
    ParserState CreatePostValueState();    
}


#endif
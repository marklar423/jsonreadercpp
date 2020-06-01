#ifndef JSONREADERCPP_STATES_OBJECT_H
#define JSONREADERCPP_STATES_OBJECT_H

#include <unordered_map>
#include <memory>

#include "jsonreadercpp/statemachine/ParserStatesSymbols.h"
#include "jsonreadercpp/statemachine/ParserState.h"

namespace jsonreadercpp::states
{       
    ParserState CreateObjectState();
    ParserState CreatePostObjectState();
    ParserState CreatePostPropertyState();    
}


#endif
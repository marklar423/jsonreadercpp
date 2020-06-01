#ifndef JSONREADERCPP_STATES_NUMBER_H
#define JSONREADERCPP_STATES_NUMBER_H

#include <unordered_map>
#include <memory>

#include "jsonreadercpp/statemachine/ParserStatesSymbols.h"
#include "jsonreadercpp/statemachine/ParserState.h"

namespace jsonreadercpp::states
{       
    ParserState CreateIntState();
    ParserState CreatePostIntParentState();
    ParserState CreateDoubleState();
    ParserState CreateExponentSignState();    
    ParserState CreateExponentDigitsState();
}


#endif
#ifndef JSONREADERCPP_STATES_STRING_H
#define JSONREADERCPP_STATES_STRING_H

#include <unordered_map>
#include <memory>

#include "jsonreadercpp/statemachine/ParserStatesSymbols.h"
#include "jsonreadercpp/statemachine/ParserState.h"

namespace jsonreadercpp::states
{       
    ParserState CreateValueStringState();
    ParserState CreatePropertyStringState();
    ParserState CreateEscapeCharState();    
    ParserState CreateUnicodeValueState();
    ParserState CreateUnicodePropertyState();
}


#endif
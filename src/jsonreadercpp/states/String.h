#ifndef JSONREADERCPP_STATES_STRING_H
#define JSONREADERCPP_STATES_STRING_H

#include <unordered_map>
#include <memory>

#include "jsonreadercpp/ParserStatesSymbols.h"
#include "jsonreadercpp/ParserState.h"

namespace jsonreadercpp::states
{       
    std::unique_ptr<ParserState> CreateValueStringState();
    std::unique_ptr<ParserState> CreatePropertyStringState();
    std::unique_ptr<ParserState> CreateEscapeCharState();    
    std::unique_ptr<ParserState> CreateUnicodeValueState();
    std::unique_ptr<ParserState> CreateUnicodePropertyState();
}


#endif
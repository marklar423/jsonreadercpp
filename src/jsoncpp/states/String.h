#ifndef JSONCPP_STATES_STRING_H
#define JSONCPP_STATES_STRING_H

#include <unordered_map>
#include <memory>

#include "jsoncpp/ParserStatesSymbols.h"
#include "jsoncpp/ParserState.h"

namespace jsoncpp::states
{       
    std::unique_ptr<ParserState> CreateValueStringState();
    std::unique_ptr<ParserState> CreatePropertyStringState();
    std::unique_ptr<ParserState> CreateEscapeCharState();    
    std::unique_ptr<ParserState> CreateUnicodeValueState();
    std::unique_ptr<ParserState> CreateUnicodePropertyState();
}


#endif
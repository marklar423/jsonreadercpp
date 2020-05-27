#ifndef JSONCPP_STATES_OBJECT_H
#define JSONCPP_STATES_OBJECT_H

#include <unordered_map>
#include <memory>

#include "jsoncpp/ParserStatesSymbols.h"
#include "jsoncpp/ParserState.h"

namespace jsoncpp::states
{       
    std::unique_ptr<ParserState> CreateObjectState();
    std::unique_ptr<ParserState> CreatePostObjectState();
    std::unique_ptr<ParserState> CreatePostPropertyState();    
}


#endif
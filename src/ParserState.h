#ifndef JSONCPP_PARSERSTATE_H
#define JSONCPP_PARSERSTATE_H


#include "JValue.h"
#include "ParserStatesSymbols.h"

namespace jsoncpp 
{   
    class ParserState
    {
        public:
        
            virtual ~ParserState() = default;
    };

}

#endif
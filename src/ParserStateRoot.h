#ifndef JSONCPP_PARSERSTATEROOT_H
#define JSONCPP_PARSERSTATEROOT_H

#include "ParserState.h"

namespace jsoncpp {
    class ParserStateRoot : public  ParserState {
        public:
            ParserStateType ProcessChar(char c) override;
            ~ParserStateRoot() = default;
    };
}

#endif
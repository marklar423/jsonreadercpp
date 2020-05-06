#ifndef JSONCPP_PARSERSTATEROOT_H
#define JSONCPP_PARSERSTATEROOT_H

#include "ParserState.h"

namespace jsoncpp {

    class ParserStateRoot : public  ParserState {
        public:
            ParserStateType ProcessChar(char c, ParserStateData& data) override;
            ~ParserStateRoot() = default;
    };

    class ParserStateObjectProperty : public ParserState {
        public:
            ParserStateType ProcessChar(char c, ParserStateData& data) override;
            ~ParserStateObjectProperty() = default;

        private:
            bool found_start_quote_ = false;
            bool found_end_quote_ = false;
            bool found_colon_ = true;

            void ResetInternalState() { found_start_quote_ = found_end_quote_ = found_colon_ = false; }
    };
}

#endif
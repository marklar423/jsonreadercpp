#ifndef JSONCPP_PARSERSTATE_H
#define JSONCPP_PARSERSTATE_H

namespace jsoncpp {
    
    enum class ParserStateType {
        Same,
        Error,
        PopEnd,
        Root,
        PushObjectProperty,
    };

    class ParserState {

        public:
            virtual ParserStateType ProcessChar(char c) = 0;

            bool IsWhiteSpace(char c)
            {
                return (c == ' ' || c == '\r' || c == '\r' || c == '\t');
            }

            virtual ~ParserState() = default;
    };

}

#endif
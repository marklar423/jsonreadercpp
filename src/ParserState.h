#ifndef JSONCPP_PARSERSTATE_H
#define JSONCPP_PARSERSTATE_H

#include <sstream>
#include <stack>
#include <memory>

#include "JValue.h"

namespace jsoncpp {
    
    enum class ParserStateType 
    {
        Same,
        Error,
        Root,
        ObjectProperty,
        ArrayElement,
        StringValue,
        IntValue,
        DoubleValue,
        NullValue,
        TrueValue,
        FalseValue        
    };
    
    struct ParserStateData 
    {
        std::stringstream property_name;
        std::stringstream scalar_value;
        std::stack<std::unique_ptr<JValue>> value_stack;

        ParserStateData() : property_name(), scalar_value(), value_stack() {}
    };

    class ParserState
    {
        public:

            virtual ParserStateType ProcessChar(char c, ParserStateData& data) = 0;

            bool IsWhiteSpace(char c)
            {
                return (c == ' ' || c == '\r' || c == '\r' || c == '\t');
            }

            virtual ~ParserState() = default;
    };

}

#endif
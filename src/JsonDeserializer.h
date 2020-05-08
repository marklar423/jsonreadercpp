#ifndef JSONCPP_JSONDESERIALIZER_H_
#define JSONCPP_JSONDESERIALIZER_H_

#include <unordered_map>
#include <memory>
#include <sstream>
#include <stack>

#include "JValue.h"
#include "ParserState.h"

namespace jsoncpp 
{

    class JsonDeserializer 
    {
        public:
            JsonDeserializer();

            std::unique_ptr<JValue> ParseJsonString(std::istringstream& input);


        private:
            std::unordered_map<ParserStateType, std::unique_ptr<ParserState>> states_;
            std::stringstream property_name_;
            std::stringstream scalar_value_;
            std::stack<std::unique_ptr<JValue>> value_stack_;

            constexpr static bool IsWhiteSpace(const char c) { return (c == ' ' || c == '\r' || c == '\r' || c == '\t'); }
    };
}
#endif
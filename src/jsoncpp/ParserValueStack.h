#ifndef JSONCPP_PARSERVALUESTACK_H_
#define JSONCPP_PARSERVALUESTACK_H_

#include <stack>
#include <string>
#include <sstream>
#include <optional>

#include "jsoncpp/JValue.h"
#include "jsoncpp/ParserStatesSymbols.h"

namespace jsoncpp
{
    class ParserValueStack
    {
        public:
            void AccumulateInput(char input_char, ParserCharDestination destination, ParserStateType current_state_type);
            void PushJValue(JValueType type);
            void PopJValue();
            std::optional<JValue> RemoveRootValue();
            
            size_t GetSize() const { return value_stack_.size(); }

        private:
            std::stack<std::pair<std::string, JValue>> value_stack_;
            std::stringstream property_name_;
            std::stringstream scalar_value_;  

            static const int kMaxEscapeChars = 4; //usually 1, but for unicode can be 4
            
            char escape_chars[kMaxEscapeChars];
            int escape_char_index = 0;

            char TranslateEscapeChar(char escaped);
    };
}

#endif
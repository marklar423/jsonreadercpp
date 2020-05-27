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

            //JSON allows up to 4 digits in the Unicode code point,
            static const int kMaxUnicodeDigits = 4;
            
            //collected unicode digits, in the order it appears in the JSON string (big endian)
            char unicode_digits_[kMaxUnicodeDigits];
            int unicode_digit_index_ = 0;
            ParserCharDestination unicode_destination_;

            char ProcessEscapeCharInput(char input_char, ParserCharDestination destination, ParserStateType current_state_type);

            char TranslateEscapeChar(char escaped);
            std::string TranslatUnicodeCodePoint();
    };
}

#endif
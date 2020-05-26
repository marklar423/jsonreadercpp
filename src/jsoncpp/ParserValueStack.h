#ifndef JSONCPP_PARSERVALUESTACK_H_
#define JSONCPP_PARSERVALUESTACK_H_

#include <stack>
#include <string>
#include <sstream>
#include <optional>

#include "JValue.h"
#include "ParserStatesSymbols.h"

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

            char TranslateEscapeChar(char escaped);
    };
}

#endif
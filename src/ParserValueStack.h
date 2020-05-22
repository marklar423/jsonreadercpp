#ifndef JSONCPP_PARSERVALUESTACK_H_
#define JSONCPP_PARSERVALUESTACK_H_

#include <stack>
#include <memory>
#include <string>
#include <sstream>

#include "JValue.h"
#include "ParserStatesSymbols.h"

namespace jsoncpp
{
    class ParserValueStack
    {
        public:
            void AccumulateInput(char input_char, ParserCharDestination destination);
            void PushJValue(JValueType type);
            void PopJValue();
            std::unique_ptr<JValue> RemoveRootValue();
            
            size_t GetSize() const { return value_stack_.size(); }

        private:
            std::stack<std::pair<std::string, std::unique_ptr<JValue>>> value_stack_;
            std::stringstream property_name_;
            std::stringstream scalar_value_;            
    };
}

#endif
#ifndef JSONREADERCPP_STATEMACHINE_PARSERVALUESTACK_H_
#define JSONREADERCPP_STATEMACHINE_PARSERVALUESTACK_H_

#include <stack>
#include <string>

#include "jsonreadercpp/JValue.h"
#include "jsonreadercpp/statemachine/ParserStatesSymbols.h"

namespace jsonreadercpp
{
    class ParserValueStack
    {
        public:
            void AccumulateInput(char input_char, ParserCharDestination destination, ParserStateType current_state_type);
            void PushJValue(JValueType type);
            void PopJValue();
            JValue RemoveRootValue();
            
            size_t GetSize() const { return value_stack_.size(); }

        private:
            std::stack<std::pair<std::string, JValue>> value_stack_;
            std::string property_name_;
            std::string scalar_value_;  
            
            //collected unicode digits
            char32_t unicode_code_point_ = 0;
            ParserCharDestination unicode_destination_;

            char ProcessEscapeCharInput(char input_char, ParserCharDestination destination, ParserStateType current_state_type);

            //Translate a character into the corresponding escape char,
            //i.e. 'n' to '\n', 't' to '\t', etc
            char TranslateEscapeChar(char escaped);

            //Collects hex codes into unicode_code_point_,
            //in the order it appears in the JSON string (big endian)
            void CollectUnicodeCodePoint(char input_char);
            std::string TranslatUnicodeCodePoint();
    };
}

#endif
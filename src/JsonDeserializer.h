#ifndef JSONCPP_JSONDESERIALIZER_H_
#define JSONCPP_JSONDESERIALIZER_H_

#include <unordered_map>
#include <memory>
#include <sstream>
#include <stack>
#include <utility>
#include <string>

#include "JValue.h"
#include "ParserState.h"
#include "ParserStatesSymbols.h"

namespace jsoncpp 
{

    class JsonDeserializer 
    {
        public:
            JsonDeserializer(bool debug_output);
            JsonDeserializer() : JsonDeserializer(false) {}

            std::unique_ptr<JValue> ParseJsonString(std::istringstream& input);


        private:
            std::unordered_map<ParserStateType, std::unique_ptr<ParserState>> states_;
            std::stringstream property_name_;
            std::stringstream scalar_value_;
            std::stack<std::pair<std::string, std::unique_ptr<JValue>>> value_stack_;
            std::stack<ParserStackSymbol> state_machine_stack_;
            bool debug_output_;

            ParserStateType ProcessState(std::istringstream& input, const std::unique_ptr<ParserState>& state, bool *finished_input, char* processed_char);
            const ParserStateTransition& GetNextTransition(std::istringstream& input, const std::unique_ptr<ParserState>& state, bool *finished_input, char* processed_char);
            void PushNewValue(JValueType type);
            void PopNewValue();
    };
}
#endif
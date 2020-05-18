#ifndef JSONCPP_JSONDESERIALIZER_H_
#define JSONCPP_JSONDESERIALIZER_H_

#include <unordered_map>
#include <memory>
#include <sstream>
#include <utility>

#include "JValue.h"
#include "ParserState.h"
#include "ParserStatesSymbols.h"
#include "ParserValueStack.h"
#include "ParserMachineStack.h"

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
            bool debug_output_;
            ParserValueStack value_stack_;
            ParserMachineStack machine_stack_;

            ParserStateType ProcessState(std::istringstream& input, const std::unique_ptr<ParserState>& state, bool *finished_input, char* processed_char);
            const ParserStateTransition& GetNextTransition(std::istringstream& input, const std::unique_ptr<ParserState>& state, bool *finished_input, char* processed_char);
            
    };
}
#endif
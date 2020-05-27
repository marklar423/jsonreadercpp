#ifndef JSONCPP_JSONDESERIALIZER_H_
#define JSONCPP_JSONDESERIALIZER_H_

#include <optional>
#include <istream>

#include "jsoncpp/JValue.h"
#include "jsoncpp/ParserStatesSymbols.h"
#include "jsoncpp/ParserValueStack.h"
#include "jsoncpp/ParserMachineStack.h"
#include "jsoncpp/ParserStatesManager.h"

namespace jsoncpp 
{

    class JsonDeserializer 
    {
        public:
            JsonDeserializer(bool debug_output = false);

            std::optional<JValue> ParseJsonString(std::istream& input);

        private:
            bool debug_output_;            
            ParserStatesManager states_manager_;
            ParserValueStack value_stack_;
            ParserMachineStack machine_stack_;

            ParserStatesManager::NextTransition ProcessState(std::istream& input, ParserStateType current_state_type);
            
    };
}
#endif
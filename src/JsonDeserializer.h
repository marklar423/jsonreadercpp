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
#include "ParserStatesManager.h"

namespace jsoncpp 
{

    class JsonDeserializer 
    {
        public:
            JsonDeserializer(bool debug_output = false);

            std::unique_ptr<JValue> ParseJsonString(std::istream& input);

        private:
            bool debug_output_;            
            ParserStatesManager states_manager_;
            ParserValueStack value_stack_;
            ParserMachineStack machine_stack_;

            ParserStatesManager::NextTransition ProcessState(std::istream& input, ParserStateType current_state_type);
            
    };
}
#endif
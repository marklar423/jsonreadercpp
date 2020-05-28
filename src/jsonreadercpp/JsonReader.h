#ifndef JSONREADERCPP_JSONDESERIALIZER_H_
#define JSONREADERCPP_JSONDESERIALIZER_H_

#include <optional>
#include <istream>

#include "jsonreadercpp/JValue.h"
#include "jsonreadercpp/ParserStatesSymbols.h"
#include "jsonreadercpp/ParserValueStack.h"
#include "jsonreadercpp/ParserMachineStack.h"
#include "jsonreadercpp/ParserStatesManager.h"

namespace jsonreadercpp 
{

    class JsonReader 
    {
        public:
            JsonReader(bool debug_output = false);

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
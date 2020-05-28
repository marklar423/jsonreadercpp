#ifndef JSONREADERCPP_JSONDESERIALIZER_H_
#define JSONREADERCPP_JSONDESERIALIZER_H_

#include <optional>
#include <iostream>

#include "jsonreadercpp/JValue.h"
#include "jsonreadercpp/statemachine/ParserStatesSymbols.h"
#include "jsonreadercpp/statemachine/ParserStatesManager.h"
#include "jsonreadercpp/statemachine/ParserValueStack.h"
#include "jsonreadercpp/statemachine/ParserMachineStack.h"

namespace jsonreadercpp 
{

    class JsonReader 
    {
        public:
            JsonReader(std::ostream& error_output_stream = std::cerr, 
                        std::ostream& debug_output_stream = std::cout, bool debug_output = false);

            std::optional<JValue> ParseJsonString(std::istream& input);

        private:
            std::ostream& error_output_stream_;
            std::ostream& debug_output_stream_;
            bool debug_output_;            

            ParserStatesManager states_manager_;

            ParserStatesManager::NextTransition ProcessState(std::istream& input, ParserStateType current_state_type,
                                                                ParserValueStack& value_stack, ParserMachineStack& machine_stack);
            
    };
}
#endif
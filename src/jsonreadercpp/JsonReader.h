#ifndef JSONREADERCPP_JSONDESERIALIZER_H_
#define JSONREADERCPP_JSONDESERIALIZER_H_

#include <optional>
#include <iostream>

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
            JsonReader(std::ostream& error_output_stream = std::cerr, 
                        std::ostream& debug_output_stream = std::cout, bool debug_output = false);

            std::optional<JValue> ParseJsonString(std::istream& input);

        private:
            std::ostream& error_output_stream_;
            std::ostream& debug_output_stream_;
            bool debug_output_;            

            ParserStatesManager states_manager_;
            ParserValueStack value_stack_;
            ParserMachineStack machine_stack_;

            ParserStatesManager::NextTransition ProcessState(std::istream& input, ParserStateType current_state_type);
            
    };
}
#endif
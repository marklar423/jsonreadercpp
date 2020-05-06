#include "JsonDeserializer.h"

#include <iostream>

#include "ParserStateImpls.h"

using std::string;
using std::unique_ptr;

namespace jsoncpp 
{
    JsonDeserializer::JsonDeserializer() : states_() {
        states_[ParserStateType::Root] = std::make_unique<ParserStateRoot>();
    }

    unique_ptr<JValue> JsonDeserializer::ParseJsonString(std::istringstream& input) 
    {
        ParserStateData data {};
        
        auto state_type = ParserStateType::Root;
        char c;

        while (input >> c) {
            if (states_.find(state_type) == states_.end())
            {
                std::cerr << "Unable to find state!\n";
                exit(1);
            }
            else
            {
                auto& state = states_[state_type];
                auto last_state_type = state_type;
                state_type = state->ProcessChar(c, data);
                
                if (state_type == ParserStateType::Same)
                {
                    state_type = last_state_type;
                }            
                else if (state_type == ParserStateType::Error)
                {
                    std::cerr << "Error parsing!\n";
                    exit(1);
                }
            }
        }

        unique_ptr<JValue> result(nullptr);

        if (data.value_stack.size() > 1)
        {
            std::cerr << "Error parsing! Unexpected end of input, JSON isn't complete\n";
            exit(1);
        }
        else if (data.value_stack.size() == 1)
        {
            result = std::move(data.value_stack.top());
            data.value_stack.pop(); //not really needed but a good habit
        }

        return result;
    }

}
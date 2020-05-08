#include "JsonDeserializer.h"

#include <iostream>
#include <string>

#include "ParserStateImpls.h"

using std::string;
using std::unique_ptr;

namespace jsoncpp 
{
    JsonDeserializer::JsonDeserializer() 
        : states_(), property_name_(), scalar_value_(), value_stack_() 
    {        
    }

    unique_ptr<JValue> JsonDeserializer::ParseJsonString(std::istringstream& input) 
    {        
        
        auto state_type = ParserStateType::Start;
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
                               
                if (state_type == ParserStateType::Error)
                {
                    std::cerr << "Error parsing!\n";
                    exit(1);
                }
            }
        }

        unique_ptr<JValue> result(nullptr);

        if (value_stack_.size() > 1)
        {
            std::cerr << "Error parsing! Unexpected end of input, JSON isn't complete\n";
            exit(1);
        }
        else if (value_stack_.size() == 1)
        {
            result = std::move(value_stack_.top());
            value_stack_.pop(); //not really needed but a good habit
        }

        return result;
    }

}
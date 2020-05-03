#include "JsonDeserializer.h"

#include <iostream>

#include "ParserStateRoot.h"

using std::string;
using std::unique_ptr;

namespace jsoncpp 
{
    JsonDeserializer::JsonDeserializer() : states_() {
        states_[ParserStateType::Root] = std::make_unique<ParserStateRoot>();
    }

    unique_ptr<JValue> JsonDeserializer::ParseJsonString(std::istringstream& input) 
    {
        auto root = std::make_unique<JValue>(new JValue( JValueType::Object ));
        
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
                state_type = state->ProcessChar(c);
                
                if (state_type == ParserStateType::Same)
                {
                    state_type = last_state_type;
                }            
                else if (state_type == ParserStateType::Error)
                {
                    std::cerr << "Error parsing!\n";
                    exit(1);
                }
                else if (state_type == ParserStateType::PopEnd)
                {
                    return root;
                }
            }
        }

        return root;
    }

}
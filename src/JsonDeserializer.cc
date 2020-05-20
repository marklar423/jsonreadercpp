#include "JsonDeserializer.h"

#include <iostream>

#include "ParserStateImpls.h"

using std::string;
using std::pair;
using std::unique_ptr;

namespace jsoncpp 
{
    JsonDeserializer::JsonDeserializer(bool debug_output)
        : states_manager_(), value_stack_(),  machine_stack_(), debug_output_(debug_output)
    {
        
    } 

    unique_ptr<JValue> JsonDeserializer::ParseJsonString(std::istream& input) 
    {        
        auto current_state_type = ParserStateType::Start;
        bool finished_input = false;
        int char_num = 0;
        char processed_char = '\0';
        
        input >> std::noskipws;

        while (!finished_input) 
        {   
            if (debug_output_)
                std::cout << '[' << char_num << ']' << processed_char << " => " << ParserStateTypeName(current_state_type) << "\n";

            if (current_state_type == ParserStateType::Error)
            {
                std::cerr << "Error parsing! Position:" << char_num << "\n";
                exit(1);
            }
            else
            {        
                auto next_transition_container = ProcessState(input, current_state_type);

                processed_char = next_transition_container.processed_char;
                current_state_type = next_transition_container.transition.next_state;
                finished_input = next_transition_container.finished_input;

                if (processed_char != '\0') char_num++;
            }
        }

        unique_ptr<JValue> result(nullptr);

        if (value_stack_.GetSize() > 1)
        {
            std::cerr << "Error parsing! Unexpected end of input, JSON isn't complete\n";
            exit(1);
        }
        else
        {
            result = value_stack_.RemoveRootValue();
        }

        return result;
    }

    ParserStatesManager::NextTransition JsonDeserializer::ProcessState(std::istream& input, ParserStateType current_state_type)    
    {        
        //get next state
        auto stack_symbol = (this->machine_stack_.GetSize() > 0) ? 
                            this->machine_stack_.GetTop() : ParserStackSymbol::None;   

        auto next_transition_container = states_manager_.GetNextTransition(current_state_type, input, stack_symbol);
        const auto& next_transition = next_transition_container.transition;

        if (!next_transition_container.finished_input)
        {
            //stack actions
            machine_stack_.PopPush(next_transition.stack_pop, next_transition.stack_push);

            //input actions
            value_stack_.AccumulateInput(next_transition_container.processed_char, next_transition.char_destination);

            //JValue actions
            if (next_transition.value_action == ParserValueAction::Push || next_transition.value_action == ParserValueAction::PushPop)
            {
                value_stack_.PushNewValue(next_transition.value_push_type.value());
            }

            if (next_transition.value_action == ParserValueAction::Pop || next_transition.value_action == ParserValueAction::PushPop)
            {
                value_stack_.PopNewValue();
            }
        }

        return next_transition_container;
    } 
}
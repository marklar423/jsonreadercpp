#include "jsonreadercpp/JsonReader.h"

using std::string;
using std::pair;
using std::optional;

namespace jsonreadercpp 
{
    JsonReader::JsonReader(std::ostream& error_output_stream /*= std::cerr*/, 
                        std::ostream& debug_output_stream /*= std::cout*/, bool debug_output /*= false*/)
        : states_manager_(), value_stack_(),  machine_stack_(), 
           error_output_stream_(error_output_stream), 
           debug_output_stream_(debug_output_stream), debug_output_(debug_output)
    {        
    } 

    optional<JValue> JsonReader::ParseJsonString(std::istream& input) 
    {        
        auto current_state_type = ParserStateType::Start;
        
        bool finished_input = false, has_error = false;
        char processed_char = '\0';

        int line_num = 1;
        int char_num = 0;
        int line_char_start = 0;
        
        input >> std::noskipws;

        while (!finished_input && !has_error) 
        {   
            if (debug_output_)
                debug_output_stream_ << '[' << char_num << ']' << processed_char << " => " << ParserStateTypeName(current_state_type) << "\n";

            if (current_state_type == ParserStateType::Error)
            {
                error_output_stream_ << "Error: Unexpected \"" << processed_char << "\""
                                     << " at Line:" << line_num << ", Line Position:" << (char_num - line_char_start) 
                                     << ", Global Position:" << char_num << "\n";                
                has_error = true;
            }
            else
            {        
                auto next_transition_container = ProcessState(input, current_state_type);

                processed_char = next_transition_container.processed_char;
                current_state_type = next_transition_container.transition.next_state;
                finished_input = next_transition_container.finished_input;

                if (processed_char != '\0') char_num++;
                if (processed_char == '\n') 
                {
                    line_num++;
                    line_char_start = char_num;
                }
            }
        }

        optional<JValue> result;

        if (!has_error)
        {
            if (value_stack_.GetSize() > 1)
            {
                error_output_stream_ << "Error: Unexpected end of input, JSON isn't complete\n";            
            }
            else
            {
                result = std::move(value_stack_.RemoveRootValue());
            }
        }

        return result;
    }

    ParserStatesManager::NextTransition JsonReader::ProcessState(std::istream& input, ParserStateType current_state_type)    
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
            value_stack_.AccumulateInput(next_transition_container.processed_char, next_transition.char_destination, current_state_type);

            //JValue actions
            if (next_transition.value_action == ParserValueAction::Push || next_transition.value_action == ParserValueAction::PushPop)
            {
                value_stack_.PushJValue(next_transition.value_push_type.value());
            }

            if (next_transition.value_action == ParserValueAction::Pop || next_transition.value_action == ParserValueAction::PushPop)
            {
                value_stack_.PopJValue();
            }
        }

        return next_transition_container;
    } 
}
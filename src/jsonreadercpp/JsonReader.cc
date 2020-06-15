#include "jsonreadercpp/JsonReader.h"

using std::string;
using std::pair;
using std::optional;

namespace jsonreadercpp 
{
    JsonReader::JsonReader(std::ostream& error_output_stream /*= std::cerr*/, 
                        std::ostream& debug_output_stream /*= std::cout*/, bool debug_output /*= false*/)
        :  states_manager_(),
            error_output_stream_(error_output_stream), 
           debug_output_stream_(debug_output_stream), debug_output_(debug_output)
    {        
    } 

    optional<JValue> JsonReader::ParseJsonString(std::istream& input) 
    {        
        //set up state machine
        auto current_state_type = ParserStateType::Start;

        ParserValueStack value_stack;
        ParserMachineStack machine_stack;
        
        //loop and status vars
        char processed_char = '\0';
        bool finished_input = false, has_error = false;

        int line_num = 1, char_num = 0, line_char_start = 0;
        
        while (!finished_input && !has_error) 
        {   
            if (debug_output_)
                debug_output_stream_ << '[' << char_num << ']' << processed_char << " => " << ParserStateTypeName(current_state_type) << "\n";
            
            if (current_state_type == ParserStateType::Error)
            {
                //the error state was encountered - there was a parsing error
                error_output_stream_ << "Error: Unexpected \"" << processed_char << "\""
                                     << " at Line:" << line_num << ", Line Position:" << (char_num - line_char_start) 
                                     << ", Global Position:" << char_num << "\n";                
                has_error = true;
            }
            else
            {        
                //(potentially) read next char and get next state 
                auto next_transition_container = ProcessState(input, current_state_type, value_stack, machine_stack);

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
            if (value_stack.GetSize() > 1)
            {
                error_output_stream_ << "Error: Unexpected end of input, JSON isn't complete\n";            
            }
            else if (value_stack.GetSize() == 1)
            {
                result = value_stack.RemoveRootValue();
            }
        }

        return result;
    }

    ParserStatesManager::NextTransition JsonReader::ProcessState(std::istream& input, ParserStateType current_state_type,
                                                                    ParserValueStack& value_stack, ParserMachineStack& machine_stack)    
    {        
        //get next state
        auto stack_symbol = (machine_stack.GetSize() > 0) ? machine_stack.GetTop() : ParserStackSymbol::None;   

        //(potentially) read next char and get the transition 
        auto next_transition_container = states_manager_.GetNextTransition(current_state_type, input, stack_symbol);
        const auto& next_transition = next_transition_container.transition;

        if (!next_transition_container.finished_input)
        {
            //stack actions
            machine_stack.PopPush(next_transition.stack_pop, next_transition.stack_push);

            //input actions
            value_stack.AccumulateInput(next_transition_container.processed_char, next_transition.char_destination, current_state_type);

            //JValue actions
            if (next_transition.value_action == ParserValueAction::Push || next_transition.value_action == ParserValueAction::PushPop)
            {
                value_stack.PushJValue(next_transition.value_push_type.value());
            }

            if (next_transition.value_action == ParserValueAction::Pop || next_transition.value_action == ParserValueAction::PushPop)
            {
                value_stack.PopJValue();
            }
        }

        return next_transition_container;
    } 


    std::istream& operator>>(std::istream& input, JValue& value)
    {
        if (input)
        {
            JsonReader reader;

            if (auto parsed_value = reader.ParseJsonString(input); parsed_value.has_value())
                value = std::move(parsed_value.value());
                
            else
                input.setstate(std::ios_base::failbit);            
        }

        return input;
    }
}
#include "JsonDeserializer.h"

#include <iostream>

#include "ParserStateImpls.h"

using std::string;
using std::pair;
using std::unique_ptr;

namespace jsoncpp 
{
    JsonDeserializer::JsonDeserializer(bool debug_output)
        : states_(std::move(jsoncpp::CreateStatesMap())), 
            value_stack_(),  machine_stack_(), debug_output_(debug_output)
    {
        
    } 

    unique_ptr<JValue> JsonDeserializer::ParseJsonString(std::istringstream& input) 
    {        
        auto current_state_type = ParserStateType::Start;
        bool finished_input = false;
        int char_num = 0;
        char processed_char = '\0';

        while (!finished_input) 
        {   
            if (debug_output_)
                std::cout << '[' << char_num << ']' << processed_char << " => " << ParserStateTypeName(current_state_type) << "\n";

            const auto& current_state_iter = states_.find(current_state_type);

            if (current_state_type == ParserStateType::Error)
            {
                std::cerr << "Error parsing! Position:" << char_num << "\n";
                exit(1);
            }
            else if (current_state_iter == states_.end())
            {
                std::cerr << "Unable to find state " << ParserStateTypeName(current_state_type) << "\n";
                exit(1);
            }
            else
            {        
                const auto& current_state_ptr = current_state_iter->second;
                processed_char = '\0';
                current_state_type = ProcessState(input, current_state_ptr, &finished_input, &processed_char);

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

    ParserStateType JsonDeserializer::ProcessState(std::istringstream& input, const unique_ptr<ParserState>& state, 
                                                        bool *finished_input, char* processed_char)    
    {        
        //get next state
        const auto& next_transition = GetNextTransition(input, state, finished_input, processed_char);

        if (!(*finished_input))
        {
            //stack actions
            machine_stack_.PopPush(next_transition.stack_pop, next_transition.stack_push);

            //input actions
            value_stack_.AccumulateInput(*processed_char, next_transition.char_destination);

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

        return next_transition.next_state;
    }

    const ParserStateTransition& JsonDeserializer::GetNextTransition(std::istringstream& input, const std::unique_ptr<ParserState>& state, 
                                                                        bool *finished_input, char* processed_char)
    {
        //order of operations: None, None -> * | None, X -> * | X, None -> * | X, Y -> *
        auto stack_symbol = (this->machine_stack_.GetSize() > 0) ? 
                                this->machine_stack_.GetTop() : ParserStackSymbol::None;        

        if (state->HasTransition(ParserInputSymbol::None, ParserStackSymbol::None))
        {
            //None, None -> *
            return state->GetTransition(ParserInputSymbol::None, ParserStackSymbol::None);
        }
        else if (stack_symbol != ParserStackSymbol::None && state->HasTransition(ParserInputSymbol::None, stack_symbol))                            
        {
            //None, X -> *
            return state->GetTransition(ParserInputSymbol::None, stack_symbol);
        }
        else
        {
            char c = '\0';
            if (input >> std::noskipws >> c)
            {              
                *processed_char = c;
                ParserInputSymbol input_symbol = jsoncpp::CharToInputSymbol(c);

                //X, None -> *
                if (state->HasTransition(input_symbol, ParserStackSymbol::None))
                    return state->GetTransition(input_symbol, ParserStackSymbol::None);

                //X, Y -> *
                else if (state->HasTransition(input_symbol, stack_symbol))
                    return state->GetTransition(input_symbol, stack_symbol);

                else
                    return state->GetElseTransition();                
            }
            else
            {
                //no more input to read
                *finished_input = true;

                //there should be no more states, but we need to return something, sooo
                return state->GetElseTransition();                
            }            
        } 
    }    
}
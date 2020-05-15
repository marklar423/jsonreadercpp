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
            property_name_(), scalar_value_(), value_stack_(),
            debug_output_(debug_output)
    {
        
    } 

    unique_ptr<JValue> JsonDeserializer::ParseJsonString(std::istringstream& input) 
    {        
        auto state_type = ParserStateType::Start;
        bool finished_input = false;
        int char_num = 0;
        char processed_char = '\0';

        while (!finished_input) 
        {   
            if (debug_output_)
                std::cout << '[' << char_num << ']' << processed_char << " => " << ParserStateTypeName(state_type) << "\n";

            if (state_type == ParserStateType::Error)
            {
                std::cerr << "Error parsing! Position:" << char_num << "\n";
                exit(1);
            }
            else if (states_.find(state_type) == states_.end())
            {
                std::cerr << "Unable to find state " << ParserStateTypeName(state_type) << "\n";
                exit(1);
            }
            else
            {        
                const auto& state = states_[state_type];
                processed_char = '\0';
                state_type = ProcessState(input, state, &finished_input, &processed_char);

                if (processed_char != '\0') char_num++;
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
            result = std::move(value_stack_.top().second);
            value_stack_.pop(); //not really needed but a good habit
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
            if (next_transition.stack_pop != next_transition.stack_push)
            {
                if (next_transition.stack_pop != ParserStackSymbol::None)
                    this->state_machine_stack_.pop();
                    
                if (next_transition.stack_push != ParserStackSymbol::None)
                    this->state_machine_stack_.push(next_transition.stack_push);
            }

            //input actions
            if ((*processed_char) != '\0' && next_transition.char_destination != ParserCharDestination::None)
            {
                if (next_transition.char_destination == ParserCharDestination::Name)
                    this->property_name_ << (*processed_char);
                else if (next_transition.char_destination == ParserCharDestination::Value)
                    this->scalar_value_ << (*processed_char);
            }

            //JValue actions
            if (next_transition.value_action == ParserValueAction::Push || next_transition.value_action == ParserValueAction::PushPop)
            {
                PushNewValue(next_transition.value_push_type.value());
            }

            if (next_transition.value_action == ParserValueAction::Pop || next_transition.value_action == ParserValueAction::PushPop)
            {
                PopNewValue();
            }
        }

        return next_transition.next_state;
    }

    const ParserStateTransition& JsonDeserializer::GetNextTransition(std::istringstream& input, const std::unique_ptr<ParserState>& state, 
                                                                        bool *finished_input, char* processed_char)
    {
        //order of operations: None, None -> * | None, X -> * | X, None -> * | X, Y -> *
        auto stack_symbol = (this->state_machine_stack_.size() > 0) ? 
                                this->state_machine_stack_.top() : ParserStackSymbol::None;        

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

    void JsonDeserializer::PushNewValue(JValueType type)
    {
        JValue* new_value;

        if (type == JValueType::Array || type == JValueType::Object || type == JValueType::Null)
        {
            new_value = new JValue(type);
        }
        else
        {
            string accumulated_chars = this->scalar_value_.str();

            if (type == JValueType::String)
                new_value = new JValue(accumulated_chars);
            else if (type == JValueType::Int)
                new_value = new JValue(std::stoi(accumulated_chars));
            else if (type == JValueType::Double)
                new_value = new JValue(std::stod(accumulated_chars));
            else if (type == JValueType::Boolean)
                new_value = new JValue(accumulated_chars == "true");
        }

        unique_ptr<JValue> new_value_ptr(new_value);

        //add the new value to the top of the stack
        this->value_stack_.emplace(this->property_name_.str(), std::move(new_value_ptr));
        
        //clear the accumulated values
        this->property_name_.str("");
        this->scalar_value_.str("");
    }

    void JsonDeserializer::PopNewValue()
    {
        if (this->value_stack_.size() > 1) //root value?
        {
            pair<string, unique_ptr<JValue>> top_value(std::move(this->value_stack_.top()));
            this->value_stack_.pop();

            auto& parent_pair = this->value_stack_.top();            
            auto& parent_value = parent_pair.second;

            if (parent_value->GetValueType() == JValueType::Array)
            {
                parent_value->AddArrayChild(std::move(top_value.second));
            }
            else if (parent_value->GetValueType() == JValueType::Object)
            {
                parent_value->AddObjectChild(top_value.first, std::move(top_value.second));
            }
        }
    }    
}
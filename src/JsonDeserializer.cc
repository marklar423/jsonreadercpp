#include "JsonDeserializer.h"

#include <iostream>

#include "ParserStateImpls.h"

using std::string;
using std::pair;
using std::unique_ptr;

namespace jsoncpp 
{
    JsonDeserializer::JsonDeserializer() 
        : states_(std::move(jsoncpp::CreateStatesMap())), 
            property_name_(), scalar_value_(), value_stack_() 
    {        
    }

    unique_ptr<JValue> JsonDeserializer::ParseJsonString(std::istringstream& input) 
    {        
        auto state_type = ParserStateType::Start;
        bool has_input = true;
        char c;
        int char_num = 0;

        while (has_input) 
        {
            if (states_.find(state_type) == states_.end())
            {
                std::cerr << "Unable to find state!\n";
                exit(1);
            }
            else
            {                               
                if (state_type == ParserStateType::Error)
                {
                    std::cerr << "Error parsing! Char " << char_num << "\n";
                    exit(1);
                }
                else
                {
                    const auto& state = states_[state_type];
                    c = '\0';

                    if (!state->IsNoneInput())                    
                    {
                        //only read input if the state specifies
                        if (!(input >> c)) has_input = false;
                        else char_num++;
                    }

                    if (has_input)
                    {
                        state_type = ProcessState(c, state);
                    }
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
            result = std::move(value_stack_.top().second);
            value_stack_.pop(); //not really needed but a good habit
        }

        return result;
    }

    ParserStateType JsonDeserializer::ProcessState(char input, const unique_ptr<ParserState>& state)
    {
        ParserInputSymbol input_symbol = jsoncpp::CharToInputSymbol(input);
        ParserStackSymbol stack_symbol = ParserStackSymbol::None;
        
        //get the top of the symbol stack, if needed
        if (!state->IsNoneStackPop())
        {
            if (this->state_machine_stack_.size() == 0)
                return ParserStateType::Error;
            else
                stack_symbol = this->state_machine_stack_.top();
        }

        //get next state
        const auto& next_transition = state->GetTransition(input_symbol, stack_symbol);

        //stack actions
        if (next_transition.stack_pop != next_transition.stack_push)
        {
            if (next_transition.stack_pop != ParserStackSymbol::None)
                this->state_machine_stack_.pop();
                
            if (next_transition.stack_push != ParserStackSymbol::None)
                this->state_machine_stack_.push(next_transition.stack_push);
        }

        //input actions
        if (input_symbol != ParserInputSymbol::None && next_transition.char_destination != ParserCharDestination::None)
        {
            if (next_transition.char_destination == ParserCharDestination::Name)
                this->property_name_ << input;
            else if (next_transition.char_destination == ParserCharDestination::Value)
                this->scalar_value_ << input;
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

        return next_transition.next_state;
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
        this->value_stack_.emplace(this->property_name_.str(), std::move(new_value));
        
        //clear the accumulated values
        this->property_name_.str("");
        this->scalar_value_.str("");
    }

    void JsonDeserializer::PopNewValue()
    {
        pair<string, unique_ptr<JValue>> top_value(std::move(this->value_stack_.top()));
        this->value_stack_.pop();

        if (this->value_stack_.size() > 0)
        {
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
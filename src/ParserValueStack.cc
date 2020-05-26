#include "ParserValueStack.h"


using std::string;
using std::pair;
using std::optional;

namespace jsoncpp
{
    void ParserValueStack::AccumulateInput(char input_char, ParserCharDestination destination, ParserStateType current_state_type)
    {
        if (current_state_type == ParserStateType::EscapeChar)
        {            
            input_char = TranslateEscapeChar(input_char);
        }

        if (input_char != '\0')
        {          
            if (destination == ParserCharDestination::Name)
                this->property_name_ << input_char;
            else if (destination == ParserCharDestination::Value)
                this->scalar_value_ << input_char;            
        }
    }

    void ParserValueStack::PushJValue(JValueType type)
    {
        optional<JValue> new_value;

        if (type == JValueType::Array || type == JValueType::Object || type == JValueType::Null)
        {
            new_value.emplace(type);
        }
        else
        {
            string accumulated_chars = this->scalar_value_.str();

            if (type == JValueType::String)
                new_value.emplace(accumulated_chars);
            else if (type == JValueType::Number)
                new_value.emplace(std::stod(accumulated_chars));
            else if (type == JValueType::Boolean)
                new_value.emplace(accumulated_chars == "true");
        }

        //add the new value to the top of the stack
        this->value_stack_.emplace(this->property_name_.str(), std::move(new_value.value()));
        
        //clear the accumulated values
        this->property_name_.str("");
        this->scalar_value_.str("");
    }

    void ParserValueStack::PopJValue()
    {
        if (this->value_stack_.size() > 1) //root value?
        {
            pair<string, JValue> top_value(std::move(this->value_stack_.top()));
            this->value_stack_.pop();

            auto& parent_pair = this->value_stack_.top();            
            auto& parent_value = parent_pair.second;

            if (parent_value.GetValueType() == JValueType::Array)
            {
                parent_value.AddArrayChild(std::move(top_value.second));
            }
            else if (parent_value.GetValueType() == JValueType::Object)
            {
                parent_value.AddObjectChild(std::move(top_value.first), std::move(top_value.second));
            }
        }
    }

    
    optional<JValue> ParserValueStack::RemoveRootValue()
    {
        optional<JValue> result;

        if (value_stack_.size() == 1)
        {
            result.emplace(std::move(value_stack_.top().second));
            value_stack_.pop();
        }

        return result;
    }

    char ParserValueStack::TranslateEscapeChar(char escaped)
    {
        switch (escaped)
        {
            case 'b': return '\b';
            case 'f': return '\f';
            case 'n': return '\n';
            case 'r': return '\r';
            case 't': return '\t';
        }

        return escaped;
    }
}
#include "ParserValueStack.h"

using std::string;
using std::unique_ptr;
using std::pair;

namespace jsoncpp
{
    void ParserValueStack::AccumulateInput(char input_char, ParserCharDestination destination)
    {
        if (input_char != '\0' && destination != ParserCharDestination::None)
        {
            if (destination == ParserCharDestination::Name)
                this->property_name_ << input_char;
            else if (destination == ParserCharDestination::Value)
                this->scalar_value_ << input_char;
        }
    }


    void ParserValueStack::PushNewValue(JValueType type)
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

    void ParserValueStack::PopNewValue()
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

    
    unique_ptr<JValue> ParserValueStack::RemoveRootValue()
    {
        unique_ptr<JValue> result(nullptr);

        if (value_stack_.size() == 1)
        {
            result = std::move(value_stack_.top().second);
            value_stack_.pop(); //not really needed but a good habit
        }

        return result;
    }
}
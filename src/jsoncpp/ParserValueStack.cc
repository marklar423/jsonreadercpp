#include "jsoncpp/ParserValueStack.h"

#include <clocale>
#include <cstdlib>
#include <ios>

using std::string;
using std::pair;
using std::optional;

namespace jsoncpp
{
    void ParserValueStack::AccumulateInput(char input_char, ParserCharDestination destination, ParserStateType current_state_type)
    {
        input_char = ProcessEscapeCharInput(input_char, destination, current_state_type);

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

    char ParserValueStack::ProcessEscapeCharInput(char input_char, ParserCharDestination destination, ParserStateType current_state_type)
    {
        if (current_state_type == ParserStateType::EscapeChar)
        {            
            input_char = (input_char == 'u') ? '\0' : TranslateEscapeChar(input_char);
        }
        else if (current_state_type == ParserStateType::UnicodeValue || current_state_type == ParserStateType::UnicodeProperty)
        {
            //collect unicode code point for later
            if (this->unicode_digit_index_ < this->kMaxUnicodeDigits)
            {
                this->unicode_digits_[this->unicode_digit_index_] = input_char;
                this->unicode_digit_index_++;
            }

            input_char = '\0';
            unicode_destination_ = destination;
        }
        else if (this->unicode_digit_index_ > 0)
        {
            //we have a previously collected unicode code point, save it now            
            if (this->unicode_destination_ == ParserCharDestination::Name)
                this->property_name_ << TranslatUnicodeCodePoint();
            else if (this->unicode_destination_ == ParserCharDestination::Value)
                this->scalar_value_ << TranslatUnicodeCodePoint();  

            this->unicode_digit_index_ = 0;
        }

        return input_char;
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

    std::string ParserValueStack::TranslatUnicodeCodePoint()
    {
        //reset the conversion state
        std::wctomb(nullptr, 0); 
                        
        wchar_t code_point = 0; 

        //combine the unicode digits into a single hex number
        for (int i = 0; i < this->unicode_digit_index_; i++)
        {
            char val = this->unicode_digits_[i];

            std::stringstream ss;
            ss << val;
            
            //convert the hex char to a number
            unsigned char hex_num;
            ss >> std::hex >> hex_num;

            //each hex digit represents 1/2 a byte, so shift by 4
            code_point = (code_point << 4) | (hex_num & 0x0F);
        }

        std::string utf_chars(MB_CUR_MAX, '\0');
        int num_bytes = std::wctomb(&utf_chars[0], code_point);        
        return utf_chars;
    }
}
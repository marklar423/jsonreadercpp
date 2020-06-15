#include "jsonreadercpp/statemachine/ParserValueStack.h"

#include <clocale>
#include <cstdlib>
#include <ios>
#include <cuchar>
#include <memory>

using std::string;
using std::pair;
using std::optional;

namespace jsonreadercpp
{
    void ParserValueStack::AccumulateInput(char input_char, ParserCharDestination destination, ParserStateType current_state_type)
    {
        input_char = ProcessEscapeCharInput(input_char, destination, current_state_type);

        if (input_char != '\0')
        {          
            if (destination == ParserCharDestination::Name)
                this->property_name_ += input_char;
            else if (destination == ParserCharDestination::Value)
                this->scalar_value_ += input_char;            
        }
    }

    void ParserValueStack::PushJValue(JValueType type)
    {
        JValue new_value; //default is JValueType::Null

        if (type == JValueType::Object)
        {
            new_value = JValue(std::unordered_map<std::string, std::unique_ptr<JValue>>{});
        }
        else if (type == JValueType::Array)
        {
            new_value = JValue(std::vector<JValue>{});
        }
        else
        {
            if (type == JValueType::String)
                new_value = JValue(std::move(scalar_value_));
            else if (type == JValueType::Number)
                new_value = JValue(std::stod(std::move(scalar_value_)));
            else if (type == JValueType::Boolean)
                new_value = JValue(scalar_value_ == "true");
        }

        //add the new value to the top of the stack
        this->value_stack_.emplace(std::move(property_name_), std::move(new_value));
        
        //clear the accumulated values
        this->property_name_ = string{};
        this->scalar_value_ = string{};
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
                parent_value.AsArray().emplace_back(std::move(top_value.second));

            else if (parent_value.GetValueType() == JValueType::Object)
                parent_value.AsObject().insert({ std::move(top_value.first), std::make_unique<JValue>(std::move(top_value.second)) });
        }
    }

    
    JValue ParserValueStack::RemoveRootValue()
    {
        JValue result(std::move(value_stack_.top().second));
        value_stack_.pop();
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
            this->CollectUnicodeCodePoint(input_char);
            
            input_char = '\0';
            this->unicode_destination_ = destination;            
        }
        else if (this->unicode_code_point_ > 0)
        {
            //we have a previously collected unicode code point, save it now            
            if (this->unicode_destination_ == ParserCharDestination::Name)
                this->property_name_ += TranslatUnicodeCodePoint();
            else if (this->unicode_destination_ == ParserCharDestination::Value)
                this->scalar_value_ += TranslatUnicodeCodePoint();  

            this->unicode_code_point_ = 0;
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

    void ParserValueStack::CollectUnicodeCodePoint(char input_char)
    {
        //convert the hex char to a number       
        unsigned char hex_num;
        
        if (input_char >= '0' && input_char <= '9')
        {
            hex_num = (input_char - '0');
        }
        else
        {
            switch (input_char)
            {
                case 'a': case 'A': hex_num = 10; break;
                case 'b': case 'B': hex_num = 11; break;
                case 'c': case 'C': hex_num = 12; break;
                case 'd': case 'D': hex_num = 13; break;
                case 'e': case 'E': hex_num = 14; break;
                case 'f': case 'F': hex_num = 15; break;
            }
        }
        

        //each hex digit represents 1/2 a byte, so shift by 4
        this->unicode_code_point_ = (this->unicode_code_point_ << 4) | (hex_num & 0x0F);
    }

    std::string ParserValueStack::TranslatUnicodeCodePoint()
    {
        std::mbstate_t state;
        string utf_chars(MB_CUR_MAX, char{});

        size_t num_bytes = std::c32rtomb(utf_chars.data(), unicode_code_point_, &state);
        utf_chars.resize(num_bytes);

        return utf_chars;
    }
}
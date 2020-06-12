#include "jsonreadercpp/statemachine/ParserValueStack.h"

#include <clocale>
#include <cstdlib>
#include <ios>

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
                this->property_name_ << input_char;
            else if (destination == ParserCharDestination::Value)
                this->scalar_value_ << input_char;            
        }
    }

    void ParserValueStack::PushJValue(JValueType type)
    {
        JValue new_value; //default is JValueType::Null

        if (type == JValueType::Object)
        {
            new_value = JValue(std::unordered_map<std::string, JValue>{});
        }
        else if (type == JValueType::Array)
        {
            new_value = JValue(std::vector<JValue>{});
        }
        else
        {
            string accumulated_chars = this->scalar_value_.str();

            if (type == JValueType::String)
                new_value = JValue(accumulated_chars);
            else if (type == JValueType::Number)
                new_value = JValue(std::stod(accumulated_chars));
            else if (type == JValueType::Boolean)
                new_value = JValue(accumulated_chars == "true");
        }

        //add the new value to the top of the stack
        this->value_stack_.emplace(this->property_name_.str(), std::move(new_value));
        
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
                parent_value.AsArray().emplace_back(std::move(top_value.second));

            else if (parent_value.GetValueType() == JValueType::Object)
                parent_value.AsObject().insert(std::move(top_value));
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
                this->property_name_ << TranslatUnicodeCodePoint();
            else if (this->unicode_destination_ == ParserCharDestination::Value)
                this->scalar_value_ << TranslatUnicodeCodePoint();  

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
        std::stringstream ss;
        ss << input_char;

        //convert the hex char to a number       
        unsigned char hex_num;
        ss >> std::hex >> hex_num;

        //each hex digit represents 1/2 a byte, so shift by 4
        this->unicode_code_point_ = (this->unicode_code_point_ << 4) | (hex_num & 0x0F);
    }

    std::string ParserValueStack::TranslatUnicodeCodePoint()
    {
        //reset the conversion state
        std::wctomb(nullptr, 0); 

        std::string utf_chars(MB_CUR_MAX, '\0');
        int num_bytes = std::wctomb(&utf_chars[0], unicode_code_point_);        

        return utf_chars;
    }
}
#include "ParserStateImpls.h"

#include <optional>

using std::optional;

namespace jsoncpp 
{
    ParserStateType ParserStateRoot::ProcessChar(char c, ParserStateData& data)
    {
        ParserStateType result = ParserStateType::Error;

        if (this->IsWhiteSpace(c)) 
        {
            result = ParserStateType::Same;
        }
        else if (c == '{')
        {
            data.value_stack.push(std::make_unique<JValue>(new JValue(JValueType::Object)));
            result = ParserStateType::ObjectProperty;
        }

        return result;
    }

    ParserStateType ParserStateObjectProperty::ProcessChar(char c, ParserStateData& data)
    {
        ParserStateType result = ParserStateType::Error;

        if (this->found_start_quote_)
        {

        }
        else if (this->found_end_quote_)
        {

        }
        else if (this->found_colon_)
        {
            optional<JValueType> new_type;

            if (c == '{')
            {                
                new_type = JValueType::Object;
                result = ParserStateType::ObjectProperty;
            }
            else if (c == '[')
            {
                new_type = JValueType::Array;
                result = ParserStateType::ArrayElement;
            }
            else if (c == '\"')
            {
                new_type = JValueType::String;
                result = ParserStateType::StringValue;
            }            
            else if (std::isdigit(c))
            {
                //start with int; if we detect a '.' switch to double mode
                new_type = JValueType::Int;
                result = ParserStateType::IntValue;
            }
            else if (c == 'n')
            {
                new_type = JValueType::Null;
                result = ParserStateType::NullValue;
            }  
            else if (c == 't')
            {
                new_type = JValueType::Boolean;
                result = ParserStateType::TrueValue;
            }  
            else if (c == 'f')
            {
                new_type = JValueType::Boolean;
                result = ParserStateType::FalseValue;
            }  

            if (new_type)
            {
                data.value_stack.push(std::make_unique<JValue>(new JValue(new_type.value())));
                this->ResetInternalState();
            }
        }

        return result;
    }
}
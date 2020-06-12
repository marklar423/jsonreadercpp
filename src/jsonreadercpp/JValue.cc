#include "jsonreadercpp/JValue.h"

#include <stdexcept>

using std::string;

namespace jsonreadercpp 
{    
    string JValue::AsString() const
    {
        switch (this->value_type_)
        {
            case JValueType::String:  return std::get<string>(this->value_) ;
            case JValueType::Number:  return std::to_string(AsNumber());
            case JValueType::Boolean: return AsBoolean() ? "true" : "false";
            case JValueType::Null:    return "";

            //default: throw std::runtime_error("This JValue is not a scalar value and therefore not convertable to string");
        }
    }        
}
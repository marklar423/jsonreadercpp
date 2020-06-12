#ifndef JSONREADERCPP_JVALUE_H_
#define JSONREADERCPP_JVALUE_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <iterator>
#include <variant>
#include <optional>

namespace jsonreadercpp 
{

    enum class JValueType 
    {
        String,
        Number,
        Boolean,
        Null,
        Object,
        Array
    };

    class JValue 
    {
        public:        
            //create a null value
            JValue()  : JValue(JValueType::Null, {}) {}

            //create an object
            explicit JValue(std::unordered_map<std::string, JValue> children) : JValue(JValueType::Object, {children}) {}

            //create an array
            explicit JValue(std::vector<JValue> children) : JValue(JValueType::Array, {children}) {}

            //create a scalar value
            explicit JValue(std::string value) : JValue(JValueType::String, {value}) {}
            explicit JValue(double value)      : JValue(JValueType::Number, {value}) {}
            explicit JValue(bool value)        : JValue(JValueType::Boolean, {value}) {}

            //copy
            JValue(const JValue& other) = default;
            JValue& operator=(const JValue& other) = default;
            
            //move
            JValue(JValue&& other) = default;
            JValue& operator=(JValue&& other) = default;

            //this converts scalar values to a string as well 
            std::string AsString()  const;            
            double      AsNumber()  const { return std::get<double>(this->value_); }
            bool        AsBoolean() const { return std::get<bool>(this->value_);   }

            auto& AsObject() { return std::get<object_t>(value_); }
            auto& AsArray()  { return std::get<array_t>(value_); }
      
            const auto& AsObjectConst() const { return std::get<object_t>(value_); }
            const auto& AsArrayConst()  const { return std::get<array_t>(value_); }

            JValueType GetValueType() const noexcept { return value_type_; }      
            bool IsScalarValue() const noexcept { return !(value_type_ == JValueType::Object || value_type_ == JValueType::Array); }

            size_t GetNumberOfChildren() const noexcept
            { 
                if (value_type_ == JValueType::Object) return AsObjectConst().size();
                else if (value_type_ == JValueType::Array) return AsArrayConst().size(); 
                return 0;
            }
            
            //get an array child by index
            const JValue& operator[](size_t index) const noexcept { return  AsArrayConst()[index]; }
            //get an object property by name
            const JValue& operator[](const std::string& name) const { return AsObjectConst().at(name); }

        private:
            using object_t = std::unordered_map<std::string, JValue>;
            using array_t  = std::vector<JValue>;
            using value_t  = std::variant<std::monostate, std::string, double, bool, object_t, array_t>;

            value_t value_;
            JValueType value_type_;
    };
}

#endif
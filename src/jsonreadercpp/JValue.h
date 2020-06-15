#ifndef JSONREADERCPP_JVALUE_H_
#define JSONREADERCPP_JVALUE_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <iterator>
#include <variant>
#include <optional>
#include <memory>

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
            JValue()  : JValue(value_t{}) {}

            //create an object
            explicit JValue(std::unordered_map<std::string, std::unique_ptr<JValue>> children) : JValue(value_t{std::move(children)}) {}

            //create an array
            explicit JValue(std::vector<JValue> children) : JValue(value_t{std::move(children)}) {}

            //create a scalar value
            explicit JValue(std::string value) : JValue(value_t{std::move(value)}) {}
            explicit JValue(double value)      : JValue(value_t{value}) {}
            explicit JValue(bool value)        : JValue(value_t{value}) {}

            //copy
            JValue(const JValue& other) = default;
            JValue& operator=(const JValue& other) = default;
            
            //move
            JValue(JValue&& other) = default;
            JValue& operator=(JValue&& other) = default;

            //converts scalar values to a string. JValueType::Null values convert to a blank string
            std::string ConvertToString()  const;           

            //value getters
            const std::string& AsString() const { return std::get<std::string>(this->value_); }

            double AsNumber()  const { return std::get<double>(this->value_); }
            bool   AsBoolean() const { return std::get<bool>(this->value_);   }

            auto& AsObject() { return std::get<object_t>(value_); }
            auto& AsArray()  { return std::get<array_t>(value_); }
      
            const auto& AsObjectConst() const { return std::get<object_t>(value_); }
            const auto& AsArrayConst()  const { return std::get<array_t>(value_); }

            //metadata
            JValueType GetValueType()  const noexcept { return GetVIndexJValueType(value_.index()); }      
            bool       IsScalarValue() const noexcept { return !(GetValueType() == JValueType::Object || GetValueType() == JValueType::Array); }

            size_t GetNumberOfChildren() const noexcept
            { 
                auto value_type = GetValueType();
                if (value_type == JValueType::Object) return AsObjectConst().size();
                else if (value_type == JValueType::Array) return AsArrayConst().size(); 
                return 0;
            }
            
            //get an array child by index
            const JValue& operator[](size_t index) const noexcept { return  AsArrayConst()[index]; }
            //get an object property by name
            const JValue& operator[](const std::string& name) const { return *(AsObjectConst().at(name)); }

        private:
            using object_t = std::unordered_map<std::string, std::unique_ptr<JValue>>;
            using array_t  = std::vector<JValue>;
            using value_t  = std::variant<std::monostate, std::string, double, bool, object_t, array_t>;
            
            value_t value_;

            JValue(value_t value) : value_(std::move(value)) { }
                        
            static constexpr JValueType GetVIndexJValueType(size_t vindex)
            {
                switch (vindex)
                {
                    case 0: return JValueType::Null;
                    case 1: return JValueType::String;
                    case 2: return JValueType::Number;
                    case 3: return JValueType::Boolean;
                    case 4: return JValueType::Object;
                    case 5: return JValueType::Array;
                }
            }
    };
}

#endif
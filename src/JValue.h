#ifndef JSONCPP_JVALUE_H_
#define JSONCPP_JVALUE_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <iterator>
#include <variant>
#include <optional>

namespace jsoncpp 
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
            //create an object, array, or null
            JValue(JValueType value_type) : JValue(value_type, {}) {}

            //create a scalar value
            JValue(std::string value) : JValue(JValueType::String, {value}) {}
            JValue(double value)      : JValue(JValueType::Number, {value}) {}
            JValue(bool value)        : JValue(JValueType::Boolean, {value}) {}

            JValue(JValue&& other) = default;
            JValue& operator=(JValue&& other) = default;

            JValue Clone() const { return JValue(*this); }

            std::optional<std::string> GetStringValue() const;
            std::optional<double>      GetNumberValue() const;
            std::optional<bool>        GetBooleanValue() const;

            std::string GetName() const { return name_; }            
            JValueType  GetValueType() const { return value_type_; }             
            size_t      GetNumberOfChildren() const { return children_.size(); }
            const auto& GetChildren() const { return children_; }
            bool        HasProperty(const std::string& name) { return children_name_indexes_.find(name) != children_name_indexes_.end(); }
            
            //returns false if this parent object is not an array
            bool AddArrayChild(JValue value);
            
            //returns true if no property with this name already exists, false otherwise
            bool AddObjectChild(std::string name, JValue value);

            //returns true if the element exists, false otherwise
            bool RemoveChild(size_t index);
            //returns true if the element exists, false otherwise
            bool RemoveChild(const std::string& name);

            //get the nth child element, either object property or array item
            const JValue& operator[](size_t index) const { return children_.at(index); }
            //get an object property by name
            const JValue& operator[](const std::string& name) const { return this[children_name_indexes_.at(name)]; }
            
            const auto begin() const { return children_.begin(); }
            const auto end()   const { return children_.end(); }

        private:
            JValue(JValueType value_type, std::variant<std::string, double, bool> value);

            //a copy is recursive and copies all children; it's very expensive
            //force the user to use the clone() method so that it doesn't happen by accident
            JValue(const JValue& other);
            JValue& operator=(const JValue& other);

            std::vector<JValue> CopyChildren(const JValue& other);

            std::string name_;
            JValueType value_type_;
            std::vector<JValue> children_;
            std::unordered_map<std::string, size_t> children_name_indexes_;
            std::variant<std::string, double, bool> value_;
    };
}

#endif
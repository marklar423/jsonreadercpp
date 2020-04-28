#ifndef JSONCPP_JVALUE_H_
#define JSONCPP_JVALUE_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <iterator>
#include <variant>
#include <optional>

namespace jsoncpp 
{

    enum class JValueType {
        String,
        Int,
        Double,
        Boolean,
        Null,
        Object,
        Array
    };

    class JValue {
        public:        
            JValue();

            /*//warning - a copy is recursive and copies all children
            //it's very expensive
            explicit JValue(const JValue& other);*/

            //can only be moved        
            JValue(JValue&& other) = default;

            std::optional<std::string> GetStringValue() const;
            std::optional<int>    GetIntValue() const;
            std::optional<double> GetDoubleValue() const;
            std::optional<bool>   GetBooleanValue() const;

            std::string GetName() const { return name_; }
            JValueType  GetValueType() const { return value_type_; }             
            size_t      GetSize() const { return children_.size(); }
            const auto& GetChildren() const { return children_; }
            bool        HasProperty(std::string name) { return children_name_indexes_.find(name) != children_name_indexes_.end(); }

            //returns true if the element exists, false otherwise
            bool RemoveChild(size_t index);
            //returns true if the element exists, false otherwise
            bool RemoveChild(std::string name);

            //get the nth child element, either object property or array item
            const JValue& operator[](size_t index) const { return *children_[index]; }
            //get an object property by name
            const JValue& operator[](const std::string& name) { return this[children_name_indexes_[name]]; }
            
            auto begin() const { return children_.begin(); }
            auto end() const { return children_.end(); }
        private:
            std::string name_;
            JValueType value_type_;
            std::vector<std::unique_ptr<JValue>> children_;
            std::unordered_map<std::string, size_t> children_name_indexes_;
            std::variant<std::string, int, double, bool> value_;
    };
}

#endif
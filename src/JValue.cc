#include "JValue.h"

using std::string;

namespace jsoncpp 
{    
    JValue::JValue() 
            : name_(""), 
                value_type_(JValueType::Null),
                children_(), 
                children_name_indexes_(), 
                value_()
    {
    }

    /*
    explicit JValue::JValue(const JValue& other)
                : children_()
            {
                name_ = other.name_;
                children_name_indexes_ = other.children_name_indexes_;
                value_type_ = other.value_type_;
                value_ = other.value_;
                
                for (auto& child : other.children_) 
                {
                    children_.push_back(std::make_unique(new JValue(*child)));
                }
            }*/

    std::optional<string> JValue::GetStringValue() const
    {
        switch (this->value_type_)
        {
            case JValueType::String:  return {std::get<string>(this->value_) };
            case JValueType::Int:     return {std::to_string(std::get<int>(this->value_))};
            case JValueType::Double:  return {std::to_string(std::get<double>(this->value_))};
            case JValueType::Boolean: return {std::get<bool>(this->value_) ? "true" : "false"};
            case JValueType::Null:    return {""};

            default: return {};     
        }
    }

    std::optional<int> JValue::GetIntValue() const
    {
        if (this->value_type_ == JValueType::Int)
            return { std::get<int>(this->value_) };
        return {}; 
    }

    std::optional<double> JValue::GetDoubleValue() const
    {
        switch (this->value_type_)
        {
            case JValueType::Int:     return { std::get<int>(this->value_) };
            case JValueType::Double:  return { std::get<double>(this->value_) };
            
            default: return {};     
        }
    }

    std::optional<bool> JValue::GetBooleanValue() const
    {
        if (this->value_type_ == JValueType::Boolean)
            return { std::get<bool>(this->value_) };
        return {}; 
    }

    bool JValue::RemoveChild(size_t index)
    {
        bool exists = false;
        if (index < this->children_.size())
        {
            exists = true;
            this->children_.erase(this->children_.begin() + index);
        }
        return exists;
    }

    bool JValue::RemoveChild(string name)
    {
        bool exists = false;

        auto kvp = this->children_name_indexes_.find(name);
        if (kvp != this->children_name_indexes_.end())
        {
            exists = true;
            this->RemoveChild(kvp->second);
            this->children_name_indexes_.erase(name);
        }

        return exists;
    }
}
#include "JValue.h"

using std::string;

namespace jsoncpp 
{    
    JValue::JValue(JValueType value_type, 
                    std::variant<std::string, double, bool> value) 
            : name_(""), 
                value_type_(value_type),
                children_(), 
                children_name_indexes_(),
                value_(value)
    {
    }

    std::optional<string> JValue::GetStringValue() const
    {
        switch (this->value_type_)
        {
            case JValueType::String:  return {std::get<string>(this->value_) };
            case JValueType::Number:  return {std::to_string(std::get<double>(this->value_))};
            case JValueType::Boolean: return {std::get<bool>(this->value_) ? "true" : "false"};
            case JValueType::Null:    return {""};

            default: return {};     
        }
    }

    std::optional<double> JValue::GetNumberValue() const
    {
        if (this->value_type_ == JValueType::Number)
            return { std::get<double>(this->value_) };

        return {};             
    }

    std::optional<bool> JValue::GetBooleanValue() const
    {
        if (this->value_type_ == JValueType::Boolean)
            return { std::get<bool>(this->value_) };
        return {}; 
    }

        
    bool JValue::AddArrayChild(JValue value)
    {
        bool success = false;

        if (this->value_type_ == JValueType::Array)
        {
            success = true;
            this->children_.emplace_back(std::move(value));
        }

        return success;
    }
    
    
    bool JValue::AddObjectChild(std::string name, JValue value)
    {
        bool success = false;

        if (this->value_type_ == JValueType::Object && name.length() > 0 && !this->HasProperty(name))
        {
            success = true;
            value.name_ = name;
            this->children_.emplace_back(std::move(value));
            this->children_name_indexes_[name] = this->children_.size() - 1;
        }

        return success;
    }

    bool JValue::RemoveChild(size_t index)
    {
        bool exists = false;
        if (index < this->children_.size())
        {
            exists = true;            
            string child_name = this->children_[index].name_;
            this->children_.erase(this->children_.begin() + index);

            if (this->children_name_indexes_.find(child_name) != this->children_name_indexes_.end())
                this->children_name_indexes_.erase(child_name);
        }
        return exists;
    }

    bool JValue::RemoveChild(const string& name)
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
#include "JsonDeserializer.h"

#include <iostream>
#include <memory>

#include "JValue.h"

using std::string;
using std::unique_ptr;

namespace jsoncpp 
{
    unique_ptr<JValue> JsonDeserializer::ParseJsonString(const string& json_text) 
    {
        auto root = std::make_unique<JValue>(new JValue(JValueType::Object));
                
        string copy_json = json_text;
        int max = copy_json.length() - 1;

        for (int i = 0; i <= max; i++) {
            copy_json[i] = json_text[max - i];
        }

        return root;
    }

}
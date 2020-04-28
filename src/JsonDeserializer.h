#ifndef JSONCPP_JSONDESERIALIZER_H_
#define JSONCPP_JSONDESERIALIZER_H_

#include <string>

#include "JValue.h"

namespace jsoncpp {

    class JsonDeserializer {

    public:
        std::unique_ptr<JValue> ParseJsonString(const std::string& json_text);


    private:


    };


}
#endif
#ifndef JSONCPP_JSONDESERIALIZER_H_
#define JSONCPP_JSONDESERIALIZER_H_

#include <string>
#include <unordered_map>
#include <memory>
#include <sstream>

#include "JValue.h"
#include "ParserState.h"

namespace jsoncpp {

    class JsonDeserializer {

    public:
        JsonDeserializer();

        std::unique_ptr<JValue> ParseJsonString(std::istringstream& input);


    private:
        std::unordered_map<ParserStateType, std::unique_ptr<ParserState>> states_;

    };


}
#endif
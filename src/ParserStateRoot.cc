#include "ParserStateRoot.h"

namespace jsoncpp 
{
    ParserStateType ParserStateRoot::ProcessChar(char c)
    {
        if (this->IsWhiteSpace(c)) 
            return ParserStateType::Same;

        if (c == '{')
            return ParserStateType::PushObjectProperty;

        return ParserStateType::Error;
    }
}
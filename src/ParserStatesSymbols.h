#ifndef JSONCPP_PARSERSTATESSYMBOLS_H_
#define JSONCPP_PARSERSTATESSYMBOLS_H_

namespace jsoncpp 
{
    enum class ParserStateType 
    {
        Error, 
        Start,
        Finish,
        RootObject,
        Object,
        PostObject,
        PostProperty,
        String,
        EscapeChar,
        Value,
        PostValue
    };

    enum class ParserInputSymbol
    {
        Whitespace,
        OpenBrace,
        CloseBrace,
        OpenBracket,
        CloseBracket,
        DoubleQuote,
        Comma,
        Colon,
        Backslash,
        AlphaB,
        AlphaF,
        AlphaN,
        AlphaR,
        AlphaT,
        AlphaU,
        Number
    };

    enum ParserStackSymbol
    {
        Start,
        OpenBrace,
        OpenBracket,
        Property
    };
}
#endif
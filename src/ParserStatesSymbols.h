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
        None,
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
        Number,
        Other
    };

    enum class ParserStackSymbol
    {
        None,
        Start,
        OpenBrace,
        OpenBracket,
        Property
    };

    enum class ParserCharDestination
    {
        None,
        Name,
        Value
    };

    enum class ParserValueAction
    {
        None,
        Push,
        Pop,
        PushPop
    };
}
#endif
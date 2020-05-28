#ifndef JSONREADERCPP_STATEMACHINE_PARSERSTATESSYMBOLS_H_
#define JSONREADERCPP_STATEMACHINE_PARSERSTATESSYMBOLS_H_

#include <string>

namespace jsonreadercpp 
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
        ValueString,
        PropertyString,
        EscapeChar,
        UnicodeValue,
        UnicodeProperty,
        Value,
        PostValue,
        True,
        False,
        Int,
        PostIntParent,
        Double,
        ExponentSign,
        ExponentDigits,
        Null
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
        Forwardslash,
        Dash,
        Plus,
        Period,
        AlphaA,
        AlphaB,
        AlphaC,
        AlphaD,
        AlphaE,
        AlphaECap,
        AlphaF,
        AlphaL,
        AlphaN,
        AlphaR,
        AlphaS,
        AlphaT,
        AlphaU,
        Number,
        Other
    };

    enum class ParserStackSymbol
    {
        None,
        Start,
        Object,
        Array,
        ValueString,
        PropertyString,
        First,
        Second,
        Third,
        Fourth
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

    constexpr ParserInputSymbol CharToInputSymbol(char input)
    {
        switch (input)
        {
            case '\0': return ParserInputSymbol::None;  
            case ' ':  
            case '\r':  
            case '\n':  
            case '\t': 
                return ParserInputSymbol::Whitespace;
            case '{': return ParserInputSymbol::OpenBrace;
            case '}': return ParserInputSymbol::CloseBrace;
            case '[': return ParserInputSymbol::OpenBracket;
            case ']': return ParserInputSymbol::CloseBracket;
            case '"': return ParserInputSymbol::DoubleQuote;
            case ',': return ParserInputSymbol::Comma;
            case ':': return ParserInputSymbol::Colon;
            case '\\': return ParserInputSymbol::Backslash;       
            case '/': return ParserInputSymbol::Forwardslash;            
            case '-': return ParserInputSymbol::Dash;                    
            case '+': return ParserInputSymbol::Plus;          
            case '.': return ParserInputSymbol::Period;
            case 'a': return ParserInputSymbol::AlphaA;
            case 'b': return ParserInputSymbol::AlphaB;
            case 'c': return ParserInputSymbol::AlphaC;
            case 'd': return ParserInputSymbol::AlphaD;
            case 'e': return ParserInputSymbol::AlphaE;        
            case 'E': return ParserInputSymbol::AlphaECap;
            case 'f': return ParserInputSymbol::AlphaF;
            case 'l': return ParserInputSymbol::AlphaL;
            case 'n': return ParserInputSymbol::AlphaN;
            case 'r': return ParserInputSymbol::AlphaR;
            case 's': return ParserInputSymbol::AlphaS;
            case 't': return ParserInputSymbol::AlphaT;
            case 'u': return ParserInputSymbol::AlphaU;            
            case '0': 
            case '1': 
            case '2': 
            case '3': 
            case '4': 
            case '5': 
            case '6': 
            case '7': 
            case '8': 
            case '9': 
                return ParserInputSymbol::Number;   
            default:
                return ParserInputSymbol::Other;
        }
    }

    static std::string ParserStateTypeName(ParserStateType state_type)
    {
        switch (state_type)
        {
            case ParserStateType::Error: return "Error";
            case ParserStateType::Start: return "Start";
            case ParserStateType::Finish: return "Finish";
            case ParserStateType::RootObject: return "RootObject";
            case ParserStateType::Object: return "Object";
            case ParserStateType::PostObject: return "PostObject";
            case ParserStateType::PostProperty: return "PostProperty";
            case ParserStateType::PropertyString: return "PropertyString";
            case ParserStateType::ValueString: return "ValueString";
            case ParserStateType::EscapeChar: return "EscapeChar";
            case ParserStateType::UnicodeValue: return "UnicodeValue";
            case ParserStateType::UnicodeProperty: return "UnicodeProperty";
            case ParserStateType::Value: return "Value";
            case ParserStateType::PostValue: return "PostValue";       
            case ParserStateType::Int: return "Int";      
            case ParserStateType::PostIntParent: return "PostIntParent";
            case ParserStateType::Double: return "Double";
            case ParserStateType::True: return "True";       
            case ParserStateType::False: return "False"; 
            case ParserStateType::Null: return "Null"; 
        }

        return "[N/A]";
    }
}
#endif

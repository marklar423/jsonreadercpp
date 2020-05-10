#include "JsonDeserializer.h"

#include <iostream>
#include <string>

#include "ParserStateImpls.h"

using std::string;
using std::unique_ptr;

namespace jsoncpp 
{
    JsonDeserializer::JsonDeserializer() 
        : states_(), property_name_(), scalar_value_(), value_stack_() 
    {        
    }

    unique_ptr<JValue> JsonDeserializer::ParseJsonString(std::istringstream& input) 
    {        
        auto state_type = ParserStateType::Start;
        bool has_input = true;
        char c;
        int char_num = 0;

        while (has_input) 
        {
            if (states_.find(state_type) == states_.end())
            {
                std::cerr << "Unable to find state!\n";
                exit(1);
            }
            else
            {                               
                if (state_type == ParserStateType::Error)
                {
                    std::cerr << "Error parsing! Char " << char_num << "\n";
                    exit(1);
                }
                else
                {
                    const auto& state = states_[state_type];
                    c = '\0';

                    if (!state->IsNoneInput())                    
                    {
                        //only read input if the state specifies
                        if (!(input >> c)) has_input = false;
                        else char_num++;
                    }

                    if (has_input)
                    {
                        state_type = ProcessState(c, state);
                    }
                }                
            }
        }

        unique_ptr<JValue> result(nullptr);

        if (value_stack_.size() > 1)
        {
            std::cerr << "Error parsing! Unexpected end of input, JSON isn't complete\n";
            exit(1);
        }
        else if (value_stack_.size() == 1)
        {
            result = std::move(value_stack_.top());
            value_stack_.pop(); //not really needed but a good habit
        }

        return result;
    }

    ParserStateType JsonDeserializer::ProcessState(char input, const unique_ptr<ParserState>& state)
    {
        
    }

    ParserInputSymbol JsonDeserializer::ClassifyInput(char input)
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
            case 'b': return ParserInputSymbol::AlphaB;
            case 'f': return ParserInputSymbol::AlphaF;
            case 'n': return ParserInputSymbol::AlphaN;
            case 'r': return ParserInputSymbol::AlphaR;
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
}
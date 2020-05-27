#ifndef JSONCPP_PARSERSTATETRANSITION_H
#define JSONCPP_PARSERSTATETRANSITION_H

#include <optional>

#include "jsoncpp/JValue.h"
#include "jsoncpp/ParserStatesSymbols.h"

namespace jsoncpp 
{   
    struct ParserStateTransition
    {
        ParserInputSymbol input;
        ParserStateType next_state;
        ParserStackSymbol stack_pop;
        ParserStackSymbol stack_push;
        ParserCharDestination char_destination;
        ParserValueAction value_action;
        std::optional<JValueType> value_push_type;

        ParserStateTransition(ParserInputSymbol input, ParserStateType next_state) 
                              : input(input), next_state(next_state), 
                                stack_pop(ParserStackSymbol::None), stack_push(ParserStackSymbol::None),
                                char_destination(ParserCharDestination::None), 
                                value_action(ParserValueAction::None), value_push_type()
        {             
        }
                        
        ParserStateTransition& SetStack(ParserStackSymbol stack_pop, ParserStackSymbol stack_push) 
            { this->stack_pop = stack_pop; this->stack_push = stack_push; return *this; }

        ParserStateTransition& SetCharDestination(ParserCharDestination char_destination) 
            { this->char_destination = char_destination; return *this; }

        ParserStateTransition& SetValueAction(ParserValueAction value_action, std::optional<JValueType> value_push_type) 
            { this->value_action = value_action; this->value_push_type = value_push_type; return *this; }
    };

}

#endif
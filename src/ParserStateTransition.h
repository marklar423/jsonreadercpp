#ifndef JSONCPP_PARSERSTATETRANSITION_H
#define JSONCPP_PARSERSTATETRANSITION_H

#include <optional>

#include "JValue.h"
#include "ParserStatesSymbols.h"

namespace jsoncpp 
{   
    struct ParserStateTransition
    {
        ParserInputSymbol input;
        ParserStateType next_state;
        ParserStackSymbol stack_push;
        ParserStackSymbol stack_pop;
        ParserCharDestination char_destination;
        ParserValueAction value_action;
        std::optional<JValueType> value_pop_type;

        ParserStateTransition(ParserInputSymbol input, ParserStateType next_state);

        ParserStateTransition(ParserInputSymbol input, ParserStateType next_state, 
                                ParserStackSymbol stack_push, ParserStackSymbol stack_pop);

        ParserStateTransition(ParserInputSymbol input, ParserStateType next_state, 
                                ParserStackSymbol stack_push, ParserStackSymbol stack_pop,
                                ParserCharDestination char_destination,
                                ParserValueAction value_action,
                                JValueType value_pop_type);
    };

}

#endif
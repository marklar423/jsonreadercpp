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
        ParserStackSymbol stack_pop;
        ParserStackSymbol stack_push;
        ParserCharDestination char_destination;
        ParserValueAction value_action;
        std::optional<JValueType> value_push_type;

        ParserStateTransition(ParserInputSymbol input, ParserStateType next_state);

        ParserStateTransition(ParserInputSymbol input, ParserStateType next_state, 
                                ParserStackSymbol stack_pop, ParserStackSymbol stack_push);

        ParserStateTransition(ParserInputSymbol input, ParserStateType next_state, 
                                ParserStackSymbol stack_pop, ParserStackSymbol stack_push,
                                ParserCharDestination char_destination);
                                
        ParserStateTransition(ParserInputSymbol input, ParserStateType next_state, 
                                ParserStackSymbol stack_pop, ParserStackSymbol stack_push,
                                ParserValueAction value_action, JValueType value_push_type);

        ParserStateTransition(ParserInputSymbol input, ParserStateType next_state, 
                                ParserStackSymbol stack_pop, ParserStackSymbol stack_push,
                                ParserCharDestination char_destination,
                                ParserValueAction value_action,
                                JValueType value_push_type);


        ParserStateTransition(const ParserStateTransition& rhs) = default;
        ParserStateTransition& operator=(const ParserStateTransition& rhs) = default;

        ParserStateTransition(ParserStateTransition&& rsh) = default;        
        ParserStateTransition& operator=(ParserStateTransition&& rhs) = default;

        ~ParserStateTransition() = default;        
    };

}

#endif
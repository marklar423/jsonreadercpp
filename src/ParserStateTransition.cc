#include "ParserStateTransition.h"

namespace jsoncpp
{
    ParserStateTransition::ParserStateTransition(ParserInputSymbol input, ParserStateType next_state) 
        : ParserStateTransition(input, next_state, 
                                ParserStackSymbol::None, ParserStackSymbol::None,
                                ParserCharDestination::None, ParserValueAction::None,
                                {})  
    {             
    }

    ParserStateTransition::ParserStateTransition(ParserInputSymbol input, ParserStateType next_state, 
                            ParserStackSymbol stack_push, ParserStackSymbol stack_pop) 
        : ParserStateTransition(input, next_state, 
                                stack_push, stack_pop,
                                ParserCharDestination::None, ParserValueAction::None,
                                {})  
    {             
    }

    ParserStateTransition::ParserStateTransition(ParserInputSymbol input, ParserStateType next_state, 
                            ParserStackSymbol stack_push, ParserStackSymbol stack_pop,
                            ParserCharDestination char_destination,
                            ParserValueAction value_action,
                            JValueType value_pop_type) 
        : input(input), next_state(next_state), 
            stack_push(stack_push), stack_pop(stack_pop),
            char_destination(char_destination), value_action(value_action),
            value_pop_type({value_pop_type})  
    {             
    }
}
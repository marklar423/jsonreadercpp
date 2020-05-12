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
                            ParserStackSymbol stack_pop, ParserStackSymbol stack_push) 
        : ParserStateTransition(input, next_state, 
                                stack_pop, stack_push,
                                ParserCharDestination::None, ParserValueAction::None,
                                {})  
    {             
    }

    ParserStateTransition::ParserStateTransition(ParserInputSymbol input, ParserStateType next_state, 
                            ParserStackSymbol stack_pop, ParserStackSymbol stack_push,
                            ParserCharDestination char_destination,
                            ParserValueAction value_action,
                            JValueType value_push_type) 
        : input(input), next_state(next_state), 
            stack_pop(stack_pop), stack_push(stack_push), 
            char_destination(char_destination), value_action(value_action),
            value_push_type({value_push_type})  
    {             
    }
}
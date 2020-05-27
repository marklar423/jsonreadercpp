
#include "jsoncpp/ParserMachineStack.h"

namespace jsoncpp
{
    void ParserMachineStack::PopPush(ParserStackSymbol stack_pop, ParserStackSymbol stack_push)
    {
        if (stack_pop != stack_push)
        {
            if (stack_pop != ParserStackSymbol::None)
                this->state_machine_stack_.pop();
                
            if (stack_push != ParserStackSymbol::None)
                this->state_machine_stack_.push(stack_push);
        }
    }
}
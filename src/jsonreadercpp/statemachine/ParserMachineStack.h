#ifndef JSONREADERCPP_STATEMACHINE_PARSERMACHINESTACK_H_
#define JSONREADERCPP_STATEMACHINE_PARSERMACHINESTACK_H_

#include <stack>

#include "jsonreadercpp/statemachine/ParserStatesSymbols.h"

namespace jsonreadercpp
{
    class ParserMachineStack
    {
        public:
            void PopPush(ParserStackSymbol stack_pop, ParserStackSymbol stack_push);

            ParserStackSymbol GetTop() const { return state_machine_stack_.top(); }
            size_t GetSize() const { return state_machine_stack_.size(); }

        private:
            std::stack<ParserStackSymbol> state_machine_stack_;
    };
}

#endif
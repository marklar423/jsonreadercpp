#ifndef JSONCPP_PARSERMACHINESTACK_H_
#define JSONCPP_PARSERMACHINESTACK_H_

#include <stack>

#include "jsoncpp/ParserStatesSymbols.h"

namespace jsoncpp
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
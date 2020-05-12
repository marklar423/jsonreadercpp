#ifndef JSONCPP_PARSERSTATE_H
#define JSONCPP_PARSERSTATE_H

#include <unordered_map>

#include "ParserStateTransition.h"
#include "JValue.h"

namespace jsoncpp 
{ 
    class ParserState
    {
        public:
            using TransitionLookup = std::unordered_map<ParserInputSymbol, std::unordered_map<ParserStackSymbol, ParserStateTransition>>;

            ParserState(ParserStateType type);

            ParserStateType GetStateType() const { return state_type_; }
            
            const TransitionLookup&      GetTransitions()    const { return transitions_; };
            const ParserStateTransition& GetElseTransition() const { return else_transition_; };

            const ParserStateTransition& GetTransition(ParserInputSymbol input_symbol, ParserStackSymbol stack_symbol) const;

            bool IsNoneInput()    const { return is_none_input_; };
            bool IsNoneStackPop() const { return is_none_stack_pop_; };

            //returns false if transition is a None input or stack pop, 
            //and other transitions exist that aren't a None input/stack.
            //Also returns false in vice versa - transition input/stack isn't None
            //but this state has a transition that is None input/stack.
            bool AddTransition(const ParserStateTransition& transition);

            virtual ~ParserState() = default;

        private:
            ParserStateType state_type_;
            bool is_none_input_;
            bool is_none_stack_pop_;
            bool CanAddTransition(ParserInputSymbol input, ParserStackSymbol stack_pop);

        protected:
            TransitionLookup transitions_;
            ParserStateTransition else_transition_;

    };

}

#endif
#ifndef JSONCPP_PARSERSTATE_H
#define JSONCPP_PARSERSTATE_H

#include <unordered_map>
#include <initializer_list>

#include "ParserStatesSymbols.h"
#include "ParserStateTransition.h"
#include "JValue.h"

namespace jsoncpp 
{ 
    class ParserState
    {
        public:
            using TransitionLookup = std::unordered_map<ParserInputSymbol, std::unordered_map<ParserStackSymbol, ParserStateTransition>>;

            ParserState(ParserStateType type, 
                        std::initializer_list<ParserStateTransition> transitions = {},
                        ParserStateTransition else_transition = {ParserInputSymbol::None, ParserStateType::Error});

            ParserStateType GetStateType() const { return state_type_; }
            
            const TransitionLookup&      GetTransitions()    const { return transitions_; };
            const ParserStateTransition& GetElseTransition() const { return else_transition_; };

            bool HasTransition(ParserInputSymbol input_symbol, ParserStackSymbol stack_symbol) const;

            const ParserStateTransition& GetTransition(ParserInputSymbol input_symbol, ParserStackSymbol stack_symbol) const
            {
                return transitions_.at(input_symbol).at(stack_symbol);
            }

            const ParserStateTransition& GetTransitionOrElse(ParserInputSymbol input_symbol, ParserStackSymbol stack_symbol) const
            {
                return HasTransition(input_symbol, stack_symbol) ? 
                            GetTransition(input_symbol, stack_symbol) : else_transition_;
            }

            void AddTransition(const ParserStateTransition& transition) { transitions_[transition.input].emplace(transition.stack_pop, transition); }

        private:
            ParserStateType state_type_;

        protected:
            TransitionLookup transitions_;
            ParserStateTransition else_transition_;

    };

}

#endif
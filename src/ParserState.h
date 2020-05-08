#ifndef JSONCPP_PARSERSTATE_H
#define JSONCPP_PARSERSTATE_H

#include <optional>
#include <unordered_map>

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

        ParserStateTransition(ParserInputSymbol input, ParserStateType next_state) 
            : input(input), next_state(next_state), 
                stack_push(ParserStackSymbol::None), stack_pop(ParserStackSymbol::None),
                char_destination(ParserCharDestination::None), value_action(ParserValueAction::None),
                value_pop_type({})  { }

        ParserStateTransition(ParserInputSymbol input, ParserStateType next_state, 
                                ParserStackSymbol stack_push, ParserStackSymbol stack_pop) 
            : input(input), next_state(next_state), 
                stack_push(stack_push), stack_pop(stack_pop),
                char_destination(ParserCharDestination::None), value_action(ParserValueAction::None),
                value_pop_type({})  { }

        ParserStateTransition(ParserInputSymbol input, ParserStateType next_state, 
                                ParserStackSymbol stack_push, ParserStackSymbol stack_pop,
                                ParserCharDestination char_destination,
                                ParserValueAction value_action,
                                JValueType value_pop_type) 
            : input(input), next_state(next_state), 
                stack_push(stack_push), stack_pop(stack_pop),
                char_destination(char_destination), value_action(value_action),
                value_pop_type({value_pop_type})  { }
    };

    class ParserState
    {
        public:
            ParserState(ParserStateType type)
                : state_type_(type), transitions_(), else_transition_(ParserInputSymbol::None, ParserStateType::Error)
            {
            }

            ParserStateType GetStateType() { return state_type_; }
            
            virtual const std::unordered_map<ParserInputSymbol, ParserStateTransition>& GetTransitions() { return transitions_; };

            virtual const ParserStateTransition& GetElseTransition() { return else_transition_; };

            virtual ~ParserState() = default;

        protected:
            ParserStateType state_type_;
            std::unordered_map<ParserInputSymbol, ParserStateTransition> transitions_;
            ParserStateTransition else_transition_;


    };

}

#endif
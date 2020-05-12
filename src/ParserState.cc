#include "ParserState.h"


namespace jsoncpp
{

    ParserState::ParserState(ParserStateType type)
        : state_type_(type), 
            transitions_(), 
            else_transition_(ParserInputSymbol::None, ParserStateType::Error),
            is_none_input_(false),
            is_none_stack_pop_(false)
    {
    }

    const ParserStateTransition& ParserState::GetTransition(ParserInputSymbol input_symbol, ParserStackSymbol stack_symbol) const
    {
        auto find_input = this->transitions_.find(input_symbol);
        if (find_input != this->transitions_.end())
        {
            auto& stack_map = find_input->second;
            auto find_stack = stack_map.find(stack_symbol);
            if (find_stack != stack_map.end())
                return find_stack->second;
        }

        return this->else_transition_;
    }

    bool ParserState::CanAddTransition(ParserInputSymbol input, ParserStackSymbol stack_pop)
    {
        bool success = true;

        if (input == ParserInputSymbol::None)
        {
            if (!is_none_input_)
            {
                if (transitions_.size() == 0)
                    is_none_input_ = true;
                else
                    success = false;
            }
        }
        else
        {
            success = !is_none_input_;
        }

        if (success && stack_pop == ParserStackSymbol::None)
        {
            if (!is_none_stack_pop_)
            {
                if (transitions_[input].size() == 0)
                    is_none_stack_pop_ = true;
                else
                    success = false;
            }
        }
        else
        {
            success = !is_none_stack_pop_;
        }

        return success;
    }


    bool ParserState::AddTransition(const ParserStateTransition& transition)
    {
        bool success = CanAddTransition(transition.input, transition.stack_pop);

        if (success)
        {
            transitions_[transition.input].emplace(transition.stack_pop, transition);
        }

        return success;
    }

}
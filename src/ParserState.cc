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


    bool ParserState::AddTransition(const ParserStateTransition& transition)
    {
        bool success = true;

        if (transition.input == ParserInputSymbol::None)
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

        if (success && transition.stack_pop == ParserStackSymbol::None)
        {
            if (!is_none_stack_pop_)
            {
                if (transitions_[transition.input].size() == 0)
                    is_none_stack_pop_ = true;
                else
                    success = false;
            }
        }
        else
        {
            success = !is_none_stack_pop_;
        }

        if (success)
        {
            transitions_[transition.input].emplace(transition.stack_pop, transition);
        }

        return success;
    }

}
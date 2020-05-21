#include "ParserState.h"


namespace jsoncpp
{
    ParserState::ParserState(ParserStateType type, 
                        std::initializer_list<ParserStateTransition> transitions /*= {}*/,
                        ParserStateTransition else_transition /*= {ParserInputSymbol::None, ParserStateType::Error}*/)
        : state_type_(type), transitions_(), else_transition_(else_transition)
    {
        for (auto& transition : transitions)
        {
            this->AddTransition(transition);
        }
    }

    bool ParserState::HasTransition(ParserInputSymbol input_symbol, ParserStackSymbol stack_symbol) const
    {
        bool found = false;

        auto find_input = this->transitions_.find(input_symbol);
        if (find_input != this->transitions_.end())
        {
            auto& stack_map = find_input->second;
            auto find_stack = stack_map.find(stack_symbol);
            found = (find_stack != stack_map.end());
        }

        return found;
    }
}
#include "jsonreadercpp/statemachine/ParserState.h"


namespace jsonreadercpp
{
    
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
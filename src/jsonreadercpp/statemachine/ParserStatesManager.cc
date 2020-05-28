
#include <iostream>

#include "jsonreadercpp/statemachine/ParserStatesManager.h"

namespace jsonreadercpp
{    
    ParserStatesManager::NextTransition ParserStatesManager::GetNextTransition(ParserStateType current_state_type, std::istream& input, ParserStackSymbol stack_top) const
    {
        //order of operations: None, None -> * | None, X -> * | X, None -> * | X, Y -> *        
        const auto& current_state_iter = states_.find(current_state_type);

        if (current_state_iter == states_.end())
        {
            std::cerr << "Unable to find state " << ParserStateTypeName(current_state_type) << "\n";
            exit(1);
        }
        else
        {        
            const auto& current_state = *current_state_iter->second;

            if (current_state.HasTransition(ParserInputSymbol::None, ParserStackSymbol::None))
            {
                //None, None -> *
                return { current_state.GetTransition(ParserInputSymbol::None, ParserStackSymbol::None) };
            }
            else if (stack_top != ParserStackSymbol::None && current_state.HasTransition(ParserInputSymbol::None, stack_top))                            
            {
                //None, X -> *
                return { current_state.GetTransition(ParserInputSymbol::None, stack_top) };
            }
            else
            {
                char c = '\0';
                if (input >> c)
                {              
                    ParserInputSymbol input_symbol = jsonreadercpp::CharToInputSymbol(c);

                    //X, None -> *
                    if (current_state.HasTransition(input_symbol, ParserStackSymbol::None))
                        return { current_state.GetTransition(input_symbol, ParserStackSymbol::None), false, c };

                    //X, Y -> *
                    else if (current_state.HasTransition(input_symbol, stack_top))
                        return { current_state.GetTransition(input_symbol, stack_top), false, c };

                    else
                        return { current_state.GetElseTransition(), false, c };
                }
                else
                {
                    //no more input to read
                    //there should be no more states, but we need to return something, sooo
                    return { current_state.GetElseTransition(), true, '\0' };
                }            
            }
        }
    }
}
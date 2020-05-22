#ifndef JSONCPP_PARSERSTATESMANAGER_H
#define JSONCPP_PARSERSTATESMANAGER_H

#include <unordered_map>
#include <istream>
#include <memory>

#include "ParserStatesSymbols.h"
#include "ParserState.h"
#include "ParserStateImpls.h"

namespace jsoncpp 
{
    class ParserStatesManager
    {
        public:            
            struct NextTransition
            {
                const ParserStateTransition& transition;
                bool finished_input;
                char processed_char;

                NextTransition(const ParserStateTransition& transition, bool finished_input = false, char processed_char = '\0')
                    : transition(transition), finished_input(finished_input), processed_char(processed_char) {}
            };

            ParserStatesManager() : states_(jsoncpp::CreateStatesMap()) {}

            NextTransition GetNextTransition(ParserStateType current_state_type, std::istream& input, ParserStackSymbol stack_top);

        private:
            std::unordered_map<ParserStateType, std::unique_ptr<ParserState>> states_;

    };
}

#endif
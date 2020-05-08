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
            ParserState(ParserStateType type);

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
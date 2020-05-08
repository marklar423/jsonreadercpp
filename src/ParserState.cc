#include "ParserState.h"


namespace jsoncpp
{

    ParserState::ParserState(ParserStateType type)
        : state_type_(type), transitions_(), else_transition_(ParserInputSymbol::None, ParserStateType::Error)
    {
    }

}
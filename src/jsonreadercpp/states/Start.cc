
#include "jsonreadercpp/states/Start.h"

using std::make_unique;
using std::unique_ptr;

namespace jsonreadercpp::states
{  

    ParserState CreateStartState()
    {
        return ParserState(ParserStateType::Start, 
                                        {
                                            ParserStateTransition(ParserInputSymbol::None, ParserStateType::RootObject)
                                                                    .SetStack(ParserStackSymbol::None, ParserStackSymbol::Start)
                                        });
    }

    ParserState CreateFinishState()
    {
       return ParserState(ParserStateType::Finish, 
                                        {
                                            { ParserInputSymbol::Whitespace, ParserStateType::Finish }
                                        },
                                        { ParserInputSymbol::None, ParserStateType::Error });
      
    }

    ParserState CreateRootObjectState()
    {
        return ParserState(ParserStateType::RootObject, 
                                        {
                                            { ParserInputSymbol::Whitespace, ParserStateType::RootObject },

                                            ParserStateTransition(ParserInputSymbol::OpenBrace, ParserStateType::Object)
                                                                   .SetStack(ParserStackSymbol::None, ParserStackSymbol::Object)
                                                                   .SetValueAction(ParserValueAction::Push, JValueType::Object)
                                        },
                                        { ParserInputSymbol::None, ParserStateType::Error } );
    }
}
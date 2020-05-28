
#include "jsonreadercpp/states/Start.h"

using std::make_unique;
using std::unique_ptr;

namespace jsonreadercpp::states
{  

    unique_ptr<ParserState> CreateStartState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::Start, 
                                        {
                                            ParserStateTransition(ParserInputSymbol::None, ParserStateType::RootObject)
                                                                    .SetStack(ParserStackSymbol::None, ParserStackSymbol::Start)
                                        }));
    }

    unique_ptr<ParserState> CreateFinishState()
    {
       return unique_ptr<ParserState>(new ParserState(ParserStateType::Finish, 
                                        {
                                            { ParserInputSymbol::Whitespace, ParserStateType::Finish }
                                        },
                                        { ParserInputSymbol::None, ParserStateType::Error }));
      
    }

    unique_ptr<ParserState> CreateRootObjectState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::RootObject, 
                                        {
                                            { ParserInputSymbol::Whitespace, ParserStateType::RootObject },

                                            ParserStateTransition(ParserInputSymbol::OpenBrace, ParserStateType::Object)
                                                                   .SetStack(ParserStackSymbol::None, ParserStackSymbol::Object)
                                                                   .SetValueAction(ParserValueAction::Push, JValueType::Object)
                                        },
                                        { ParserInputSymbol::None, ParserStateType::Error } ));
    }
}
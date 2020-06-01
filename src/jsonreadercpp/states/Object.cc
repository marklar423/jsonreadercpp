#include "jsonreadercpp/states/Object.h"

using std::make_unique;
using std::unique_ptr;

namespace jsonreadercpp::states
{  
    ParserState CreateObjectState()
    {
        return ParserState(ParserStateType::Object, 
                                        {
                                            { ParserInputSymbol::Whitespace, ParserStateType::Object },

                                            ParserStateTransition(ParserInputSymbol::CloseBrace, ParserStateType::PostObject)
                                                                    .SetStack(ParserStackSymbol::Object, ParserStackSymbol::None)
                                                                    .SetValueAction(ParserValueAction::Pop, {}),
                                                
                                            { ParserInputSymbol::DoubleQuote, ParserStateType::PropertyString }
                                        },
                                        { ParserInputSymbol::None, ParserStateType::Error } );
    }
    
    ParserState CreatePostObjectState()
    {
        return ParserState(ParserStateType::PostObject, 
                                        {
                                            ParserStateTransition(ParserInputSymbol::None, ParserStateType::Finish)
                                                .SetStack(ParserStackSymbol::Start, ParserStackSymbol::None ),

                                            ParserStateTransition(ParserInputSymbol::None, ParserStateType::PostValue)
                                                .SetStack(ParserStackSymbol::Object, ParserStackSymbol::Object ),

                                            ParserStateTransition(ParserInputSymbol::None, ParserStateType::PostValue)
                                                .SetStack(ParserStackSymbol::Array, ParserStackSymbol::Array )
                                        });
    }
    


    ParserState CreatePostPropertyState()
    {        
        return ParserState(ParserStateType::PostProperty, 
                                        {
                                            { ParserInputSymbol::Whitespace, ParserStateType::PostProperty },    
                                            { ParserInputSymbol::Colon, ParserStateType::Value },

                                        }, { ParserInputSymbol::None, ParserStateType::Error });
    }
}
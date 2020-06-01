#include "jsonreadercpp/states/ValueAndArray.h"

using std::make_unique;
using std::unique_ptr;

namespace jsonreadercpp::states
{  
    ParserState CreateValueState()
    {
         return ParserState(ParserStateType::Value, 
                                        {                                            
                                            { ParserInputSymbol::Whitespace, ParserStateType::Value },                                            
                                            { ParserInputSymbol::DoubleQuote, ParserStateType::ValueString },      
                                                                      
                                            ParserStateTransition( ParserInputSymbol::Number, ParserStateType::Int )
                                                .SetCharDestination(ParserCharDestination::Value), 
                                                                      
                                            ParserStateTransition( ParserInputSymbol::Dash, ParserStateType::Int )
                                                .SetCharDestination(ParserCharDestination::Value),
                                            
                                            ParserStateTransition( ParserInputSymbol::AlphaF, ParserStateType::False )      
                                                .SetStack(ParserStackSymbol::None, ParserStackSymbol::First)  
                                                .SetCharDestination(ParserCharDestination::Value),

                                            ParserStateTransition( ParserInputSymbol::AlphaT, ParserStateType::True )   
                                                .SetStack(ParserStackSymbol::None, ParserStackSymbol::First)
                                                .SetCharDestination(ParserCharDestination::Value),

                                            ParserStateTransition( ParserInputSymbol::AlphaN, ParserStateType::Null )
                                                .SetStack(ParserStackSymbol::None, ParserStackSymbol::First),            
                                            
                                            ParserStateTransition(ParserInputSymbol::OpenBracket, ParserStateType::Value)
                                                .SetStack(ParserStackSymbol::None, ParserStackSymbol::Array)
                                                .SetValueAction(ParserValueAction::Push, JValueType::Array),

                                            ParserStateTransition( ParserInputSymbol::CloseBracket, ParserStateType::PostValue)
                                                .SetStack(ParserStackSymbol::Array, ParserStackSymbol::None)
                                                .SetValueAction(ParserValueAction::Pop, {}),

                                            ParserStateTransition(ParserInputSymbol::OpenBrace, ParserStateType::Object)
                                                .SetStack(ParserStackSymbol::None, ParserStackSymbol::Object)
                                                .SetValueAction(ParserValueAction::Push, JValueType::Object)

                                        }, { ParserInputSymbol::None, ParserStateType::Error });
    }
    
    ParserState CreatePostValueState()
    {
        return ParserState(ParserStateType::PostValue, 
                                        {
                                            { ParserInputSymbol::Whitespace, ParserStateType::PostValue },     
                                                
                                            ParserStateTransition( ParserInputSymbol::Comma, ParserStateType::Value)
                                                .SetStack(ParserStackSymbol::Array, ParserStackSymbol::Array ),
                                                
                                            ParserStateTransition( ParserInputSymbol::Comma, ParserStateType::Object)
                                                .SetStack(ParserStackSymbol::Object, ParserStackSymbol::Object ),

                                            ParserStateTransition( ParserInputSymbol::CloseBracket, ParserStateType::PostValue)
                                                .SetStack(ParserStackSymbol::Array, ParserStackSymbol::None)
                                                .SetValueAction(ParserValueAction::Pop, {}),

                                            ParserStateTransition( ParserInputSymbol::CloseBrace, ParserStateType::PostObject)
                                                .SetStack(ParserStackSymbol::Object, ParserStackSymbol::None)
                                                .SetValueAction(ParserValueAction::Pop, {})

                                        }, { ParserInputSymbol::None, ParserStateType::Error });
    }
    

}
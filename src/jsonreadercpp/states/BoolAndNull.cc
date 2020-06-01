
#include "jsonreadercpp/states/BoolAndNull.h"

using std::make_unique;
using std::unique_ptr;

namespace jsonreadercpp::states
{  
    ParserState CreateTrueState()
    {
        return ParserState(ParserStateType::True, 
                                        { 
                                            ParserStateTransition(ParserInputSymbol::AlphaR, ParserStateType::True)
                                                .SetStack(ParserStackSymbol::First, ParserStackSymbol::Second)
                                                .SetCharDestination(ParserCharDestination::Value),

                                            ParserStateTransition(ParserInputSymbol::AlphaU, ParserStateType::True)
                                                .SetStack(ParserStackSymbol::Second, ParserStackSymbol::Third)
                                                .SetCharDestination(ParserCharDestination::Value),

                                            ParserStateTransition(ParserInputSymbol::AlphaE, ParserStateType::PostValue)
                                                .SetStack(ParserStackSymbol::Third, ParserStackSymbol::None)
                                                .SetCharDestination(ParserCharDestination::Value) 
                                                .SetValueAction(ParserValueAction::PushPop, JValueType::Boolean)

                                        }, { ParserInputSymbol::None, ParserStateType::Error });
    }
    
    ParserState CreateFalseState()
    {
        return ParserState(ParserStateType::False, 
                                        { 
                                            ParserStateTransition(ParserInputSymbol::AlphaA, ParserStateType::False)
                                                .SetStack(ParserStackSymbol::First, ParserStackSymbol::Second)
                                                .SetCharDestination(ParserCharDestination::Value),

                                            ParserStateTransition(ParserInputSymbol::AlphaL, ParserStateType::False)
                                                .SetStack(ParserStackSymbol::Second, ParserStackSymbol::Third)
                                                .SetCharDestination(ParserCharDestination::Value),

                                            ParserStateTransition(ParserInputSymbol::AlphaS, ParserStateType::False)
                                                .SetStack(ParserStackSymbol::Third, ParserStackSymbol::Fourth)
                                                .SetCharDestination(ParserCharDestination::Value),

                                            ParserStateTransition(ParserInputSymbol::AlphaE, ParserStateType::PostValue)
                                                .SetStack(ParserStackSymbol::Fourth, ParserStackSymbol::None)
                                                .SetCharDestination(ParserCharDestination::Value) 
                                                .SetValueAction(ParserValueAction::PushPop, JValueType::Boolean)

                                        }, { ParserInputSymbol::None, ParserStateType::Error });
    }
  
    ParserState CreateNullState()
    {
        return ParserState(ParserStateType::Null, 
                                        { 
                                            ParserStateTransition(ParserInputSymbol::AlphaU, ParserStateType::Null)
                                                .SetStack(ParserStackSymbol::First, ParserStackSymbol::Second),

                                            ParserStateTransition(ParserInputSymbol::AlphaL, ParserStateType::Null)
                                                .SetStack(ParserStackSymbol::Second, ParserStackSymbol::Third),
                                                
                                            ParserStateTransition(ParserInputSymbol::AlphaL, ParserStateType::PostValue)
                                                .SetStack(ParserStackSymbol::Third, ParserStackSymbol::None)
                                                .SetValueAction(ParserValueAction::PushPop, JValueType::Null)

                                        }, { ParserInputSymbol::None, ParserStateType::Error });
    }    
    

}
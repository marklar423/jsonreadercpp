
#include "jsoncpp/states/BoolAndNull.h"

using std::make_unique;
using std::unique_ptr;

namespace jsoncpp::states
{  
    unique_ptr<ParserState> CreateTrueState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::True, 
                                        { 
                                            ParserStateTransition(ParserInputSymbol::AlphaR, ParserStateType::True)
                                                .SetStack(ParserStackSymbol::AlphaT, ParserStackSymbol::AlphaR)
                                                .SetCharDestination(ParserCharDestination::Value),

                                            ParserStateTransition(ParserInputSymbol::AlphaU, ParserStateType::True)
                                                .SetStack(ParserStackSymbol::AlphaR, ParserStackSymbol::AlphaU)
                                                .SetCharDestination(ParserCharDestination::Value),

                                            ParserStateTransition(ParserInputSymbol::AlphaE, ParserStateType::PostValue)
                                                .SetStack(ParserStackSymbol::AlphaU, ParserStackSymbol::None)
                                                .SetCharDestination(ParserCharDestination::Value) 
                                                .SetValueAction(ParserValueAction::PushPop, JValueType::Boolean)

                                        }, { ParserInputSymbol::None, ParserStateType::Error }));
    }
    
    unique_ptr<ParserState> CreateFalseState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::False, 
                                        { 
                                            ParserStateTransition(ParserInputSymbol::AlphaA, ParserStateType::False)
                                                .SetStack(ParserStackSymbol::AlphaF, ParserStackSymbol::AlphaA)
                                                .SetCharDestination(ParserCharDestination::Value),

                                            ParserStateTransition(ParserInputSymbol::AlphaL, ParserStateType::False)
                                                .SetStack(ParserStackSymbol::AlphaA, ParserStackSymbol::AlphaL)
                                                .SetCharDestination(ParserCharDestination::Value),

                                            ParserStateTransition(ParserInputSymbol::AlphaS, ParserStateType::False)
                                                .SetStack(ParserStackSymbol::AlphaL, ParserStackSymbol::AlphaS)
                                                .SetCharDestination(ParserCharDestination::Value),

                                            ParserStateTransition(ParserInputSymbol::AlphaE, ParserStateType::PostValue)
                                                .SetStack(ParserStackSymbol::AlphaS, ParserStackSymbol::None)
                                                .SetCharDestination(ParserCharDestination::Value) 
                                                .SetValueAction(ParserValueAction::PushPop, JValueType::Boolean)

                                        }, { ParserInputSymbol::None, ParserStateType::Error }));
    }
  
    unique_ptr<ParserState> CreateNullState()
    {
        return unique_ptr<ParserState>(new ParserState(ParserStateType::Null, 
                                        { 
                                            ParserStateTransition(ParserInputSymbol::AlphaU, ParserStateType::Null)
                                                .SetStack(ParserStackSymbol::AlphaN, ParserStackSymbol::AlphaU),

                                            ParserStateTransition(ParserInputSymbol::AlphaL, ParserStateType::Null)
                                                .SetStack(ParserStackSymbol::AlphaU, ParserStackSymbol::AlphaL),
                                                
                                            ParserStateTransition(ParserInputSymbol::AlphaL, ParserStateType::PostValue)
                                                .SetStack(ParserStackSymbol::AlphaL, ParserStackSymbol::None)
                                                .SetValueAction(ParserValueAction::PushPop, JValueType::Null)

                                        }, { ParserInputSymbol::None, ParserStateType::Error }));
    }    
    

}
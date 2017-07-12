//
// Created by lee on 17/7/5.
//

#ifndef VISITOR_CATCHALL_H
#define VISITOR_CATCHALL_H

#include "Visitor.h"

// DefaultCatchAll
// Default implementation of CatchAll function
template<typename R, typename Visited>
struct DefaultCatchAll
{
    static R OnUnknownVisitor(Visited&, BaseVisitor&)
    {return R();}
};

#endif //VISITOR_CATCHALL_H

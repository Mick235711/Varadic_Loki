//
// Created by lee on 17/7/5.
//

#ifndef VISITOR_VISITABLE_H
#define VISITOR_VISITABLE_H

#include "CatchAll.h"
#include "Visitor.h"

// BaseVisitable
// base class for all Visitable
template
    <
        typename R = void,
        template<typename, typename> class CatchAll = DefaultCatchAll
    >
class BaseVisitable
{
public:
    typedef R ReturnType;
    virtual ~BaseVisitable() = default;
    virtual ReturnType Accept(BaseVisitor&) = 0;

protected: // give access only to the hierarchy.
    template<typename T>
    static ReturnType AcceptImpl(T& visited, BaseVisitor& guest)
    {
        // Apply the Acyclic Visitor
        if (Visitor<T, R>* p = dynamic_cast<Visitor<T, R>*>(&guest))
        {
            return p->Visit(visited);
        }
        return CatchAll<R, T>::OnUnknownVisitor(visited, guest);
    }
};

// DEFINE_VISITABLE
// Put it in every concrete Visitable's public section
#define DEFINE_VISITABLE() \
    virtual ReturnType Accept(::BaseVisitor& guest) \
    {return AcceptImpl(*this, guest);}

#endif //VISITOR_VISITABLE_H

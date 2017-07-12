//
// Created by lee on 17/7/4.
//

#include "FunctorChainer.h"

template<typename Func1, typename R2, typename... P2>
FunctorChainer<Func1, R2, P2...>::FunctorChainer(func1_type func1, const func2_type& func2)
: m_fun1(func1), m_fun2(func2)
{}

template<typename Func1, typename R2, typename... P2>
FunctorChainer<Func1, R2, P2...>* FunctorChainer<Func1, R2, P2...>::Clone() const
{return new FunctorChainer(*this);}

template<typename Func1, typename R2, typename... P2>
R2 FunctorChainer<Func1, R2, P2...>::operator()(P2... args)
{
    m_fun1();
    return m_fun2(std::forward<P2>(args)...);
}

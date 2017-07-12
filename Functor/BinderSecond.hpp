//
// Created by lee on 17/6/23.
//

#include "BinderSecond.h"

template<typename R, typename P1, typename P2, typename... PR>
BinderSecond<R, P1, P2, PR...>::BinderSecond(const incoming_type& func, P2 bound)
    : m_func(func), m_bound(bound)
{}

template<typename R, typename P1, typename P2, typename... PR>
BinderSecond<R, P1, P2, PR...>* BinderSecond<R, P1, P2, PR...>::Clone() const
{
    return new BinderSecond(*this);
}

template<typename R, typename P1, typename P2, typename... PR>
R BinderSecond<R, P1, P2, PR...>::operator()(P1 arg1, PR... argr)
{
    return m_func(std::forward<P1>(arg1), m_bound, std::forward<PR>(argr)...);
}

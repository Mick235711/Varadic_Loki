//
// Created by lee on 17/6/23.
//

#include "BinderFirst.h"

template<typename R, typename P1, typename... PR>
BinderFirst<R, P1, PR...>::BinderFirst(const incoming_type& func, P1 bound)
: m_func(func), m_bound(bound)
{}

template<typename R, typename P1, typename... PR>
BinderFirst<R, P1, PR...>* BinderFirst<R, P1, PR...>::Clone() const
{
    return new BinderFirst(*this);
}

template<typename R, typename P1, typename... PR>
R BinderFirst<R, P1, PR...>::operator()(PR... argr)
{
    return m_func(m_bound, std::forward<PR>(argr)...);
}

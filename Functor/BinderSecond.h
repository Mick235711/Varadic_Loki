//
// Created by lee on 17/6/23.
//

#ifndef FUNCTOR_BINDERSECOND_H
#define FUNCTOR_BINDERSECOND_H

#include <type_traits>
#include "FunctorImpl.h"
#include "../TypeList/TypeList.h"
#include "Functor.h"

template<typename R, typename P1, typename P2, typename... PR>
class BinderSecond : public FunctorImpl<R, P1, PR...>
{
public:
    // typedefs
    typedef R                               result_type;
    typedef TL::TypeList<P1, P2, PR...>     param_list_type;
    template<int ind> using                 param = typename std::conditional<ind == 1, P1, typename TL::TypeAtNonStrict<param_list_type, ind - 1, TL::EmptyType>::type>::type;
    typedef P2                              bound_type;
    typedef Functor<R, P1, P2, PR...>       incoming_type;
    typedef Functor<R, P1, PR...>           outgoing_type;
    typedef BinderSecond<R, P1, P2, PR...>  class_type;

public:
    // functions
    BinderSecond() = default;
    BinderSecond(const incoming_type&, P2);
    BinderSecond* Clone() const;
    R operator()(P1, PR...);

private:
    // members
    incoming_type m_func;
    P2 m_bound;
};

template<typename R, typename Bound, typename P1, typename P2, typename... PR>
Functor<R, P1, PR...> BindSecond(const Functor<R, P1, P2, PR...>& func, Bound bound)
{
    typedef Functor<R, P1, PR...> outgoing_type;
    std::shared_ptr<typename outgoing_type::Impl> sp(new BinderSecond<R, P1, P2, PR...>(func, bound));
    return outgoing_type(sp);
}

#endif //FUNCTOR_BINDERSECOND_H

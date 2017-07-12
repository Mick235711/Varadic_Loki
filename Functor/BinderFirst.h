//
// Created by lee on 17/6/23.
//

#ifndef FUNCTOR_BINDERFIRST_H
#define FUNCTOR_BINDERFIRST_H

#include <type_traits>
#include "FunctorImpl.h"
#include "../TypeList/TypeList.h"
#include "Functor.h"

template<typename R, typename P1, typename... PR>
class BinderFirst : public FunctorImpl<R, PR...>
{
public:
    // typedefs
    typedef R                         result_type;
    typedef TL::TypeList<P1, PR...>   param_list_type;
    template<int ind> using           param = typename TL::TypeAtNonStrict<param_list_type, ind - 1, TL::EmptyType>::type;
    typedef P1                        bound_type;
    typedef Functor<R, P1, PR...>     incoming_type;
    typedef Functor<R, PR...>         outgoing_type;
    typedef BinderFirst<R, P1, PR...> class_type;
    
public:
    // functions
    BinderFirst() = default;
    BinderFirst(const incoming_type&, P1);
    BinderFirst* Clone() const;
    R operator()(PR...);
    
private:
    // members
    incoming_type m_func;
    P1 m_bound;
};

template<typename R, typename Bound, typename P1, typename... PR>
Functor<R, PR...> BindFirst(const Functor<R, P1, PR...>& func, Bound bound)
{
    typedef Functor<R, PR...> outgoing_type;
    std::shared_ptr<typename outgoing_type::Impl> sp(new BinderFirst<R, P1, PR...>(func, bound));
    return outgoing_type(sp);
}

#endif //FUNCTOR_BINDERFIRST_H

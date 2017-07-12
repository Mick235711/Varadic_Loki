//
// Created by lee on 17/6/23.
//

#ifndef FUNCTOR_FUNCTORHANDLER_H
#define FUNCTOR_FUNCTORHANDLER_H

#include "FunctorImpl.h"
#include <type_traits>
#include "TypeList/TypeList.h"

template<typename Func, typename R, typename... P>
class FunctorHandler : public FunctorImpl<R, P...>
{
public:
    // typedefs
    typedef FunctorHandler<Func, R, P...>        class_type;
    typedef FunctorImpl<R, P...>                 parent_type;
    typedef typename std::decay<Func>::type      func_type;
    typedef R                                    result_type;
    typedef typename TL::TypeList<R, P...>::Tail param_list_type;
    template<int ind> using                      param = typename TL::TypeAtNonStrict<param_list_type, ind, TL::EmptyType>::type;
    
public:
    // functions
    FunctorHandler() = delete;
    FunctorHandler(const func_type&);
    FunctorHandler* Clone() const;
    R operator()(P...);

private:
    // members
    func_type m_func;
};


#endif //FUNCTOR_FUNCTORHANDLER_H

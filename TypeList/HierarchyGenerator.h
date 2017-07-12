#ifndef TYPELIST_HIERARCHYGENERATOR_H
#define TYPELIST_HIERARCHYGENERATOR_H

#include "TypeList.h"

namespace TL
{

    // Using TypeLists to generate class hierarchy
    // technique: template template parameter
    
    template<typename AtomicType, template<class> class Unit>
    struct GenScatterHierarchy : public Unit<AtomicType>
    {
    };
    template<typename Head, template<class> class Unit>
    struct GenScatterHierarchy<TL::TypeList<Head>, Unit>
    : public GenScatterHierarchy<Head, Unit>
    {
    };
    template<typename Head, typename... Tail, template<class> class Unit>
    struct GenScatterHierarchy<TL::TypeList<Head, Tail...>, Unit>
    : public GenScatterHierarchy<Head, Unit>,
      public GenScatterHierarchy<TL::TypeList<Tail...>, Unit>
    {
    };
    
    // utility functions
    template<typename T, typename TList, template<class> class Unit>
    Unit<T>& Field(GenScatterHierarchy<TList, Unit>& obj)
    {
        return obj;
    };
    template<typename T, typename TList, template<class> class Unit>
    const Unit<T>& Field(const GenScatterHierarchy<TList, Unit>& obj)
    {
        return obj;
    };
    
    template<typename Head, typename... Tail, template<class> class Unit>
    Unit<Head>& FieldHelper(GenScatterHierarchy<TL::TypeList<Head, Tail...>, Unit>& obj, Int2Type<0>)
    {
        GenScatterHierarchy<Head, Unit>& leftBase = obj;
        return leftBase;
    };
    template<int i, typename Head, typename... Tail, template<class> class Unit>
    Unit<typename TypeAt<TL::TypeList<Head, Tail...>, i>::type>& FieldHelper(
        GenScatterHierarchy<TL::TypeList<Head, Tail...>, Unit>& obj,
        Int2Type<i>)
    {
        GenScatterHierarchy<TL::TypeList<Tail...>, Unit>& rightBase = obj;
        return FieldHelper(rightBase, Int2Type<i - 1>());
    };
    template<typename Head, typename... Tail, template<class> class Unit>
    const Unit<Head>& FieldHelper(const GenScatterHierarchy<TL::TypeList<Head, Tail...>, Unit>& obj, Int2Type<0>)
    {
        GenScatterHierarchy<Head, Unit>& leftBase = obj;
        return leftBase;
    };
    template<int i, typename Head, typename... Tail, template<class> class Unit>
    const Unit<typename TypeAt<TL::TypeList<Head, Tail...>, i>::type>& FieldHelper(
        const GenScatterHierarchy<TL::TypeList<Head, Tail...>, Unit>& obj,
        Int2Type<i>)
    {
        GenScatterHierarchy<TL::TypeList<Tail...>, Unit>& rightBase = obj;
        return FieldHelper(rightBase, Int2Type<i - 1>());
    };
    
    template<int i, typename TList, template<class> class Unit>
    Unit<typename TL::TypeAt<TList, i>::type>& Field(
        GenScatterHierarchy<TList, Unit>& obj)
    {
        return FieldHelper(obj, Int2Type<i>());
    };
    template<int i, typename TList, template<class> class Unit>
    const Unit<typename TL::TypeAt<TList, i>::type>& Field(
        const GenScatterHierarchy<TList, Unit>& obj)
    {
        return FieldHelper(obj, Int2Type<i>());
    };
    
    template
    <
        typename T,
        template<class AtomicType, class Base> class Unit,
        typename Root = EmptyType
    >
    struct GenLinearHierarchy;
    template
    <
        typename T1,
        typename... T2,
        template<class, class> class Unit,
        typename Root
    >
    struct GenLinearHierarchy<TL::TypeList<T1, T2...>, Unit, Root>
    : public Unit<T1, GenLinearHierarchy<TL::TypeList<T2...>, Unit, Root> >
    {
    };
    template
    <
        typename T1,
        template<class, class> class Unit,
        typename Root
    >
    struct GenLinearHierarchy<TL::TypeList<T1>, Unit, Root>
    : public Unit<T1, Root>
    {
    };
}
#endif //TYPELIST_HIERARCHYGENERATOR_H

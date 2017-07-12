#ifndef TYPELIST_H
#define TYPELIST_H

// TypeList implemented in variadic templates

#include "TypeManip.h"

namespace TL
{
    struct NullType;
    struct EmptyType {};
    
    template<typename H, typename... T> struct TypeList;
    
    template<typename H, typename... T>
    struct TypeList
    {
        typedef H Head;
        typedef TypeList<T...> Tail;
    };
    template<typename H>
    struct TypeList<H>
    {
        typedef H Head;
        typedef EmptyType Tail;
    };
    
    template<typename TList> struct Length;
    template<typename Head, typename... Tail>
    struct Length<TypeList<Head, Tail...> >
    {
        enum {value = sizeof...(Tail) + 1};
    };
    
    template<typename TList, int index> struct TypeAt;
    template<typename Head, typename... Tail>
    struct TypeAt<TypeList<Head, Tail...>, 0>
    {
        typedef Head type;
    };
    template<typename Head, typename... Tail, int index>
    struct TypeAt<TypeList<Head, Tail...>, index>
    {
        typedef typename TypeAt<TypeList<Tail...>, index - 1>::type type;
    };
    
    template<typename TList, int index, typename ErrorType = NullType> struct TypeAtNonStrict;
    template<typename Head, int index, typename ErrorType>
    struct TypeAtNonStrict<TypeList<Head>, index, ErrorType>
    {
        typedef ErrorType type;
    };
    template<typename Head, typename... Tail, typename ErrorType>
    struct TypeAtNonStrict<TypeList<Head, Tail...>, 0, ErrorType>
    {
        typedef Head type;
    };
    template<typename Head, typename... Tail, int index, typename ErrorType>
    struct TypeAtNonStrict<TypeList<Head, Tail...>, index, ErrorType>
    {
        typedef typename TypeAt<TypeList<Tail...>, index - 1>::type type;
    };
    
    template<typename TList, typename T> struct IndexOf;
    template<typename T>
    struct IndexOf<TypeList<T>, T>
    {
        enum {value = 0};
    };
    template<typename Head, typename T>
    struct IndexOf<TypeList<Head>, T>
    {
        enum {value = -1};
    };
    template<typename... Tail, typename T>
    struct IndexOf<TypeList<T, Tail...>, T>
    {
        enum {value = 0};
    };
    template<typename Head, typename... Tail, typename T>
    struct IndexOf<TypeList<Head, Tail...>, T>
    {
    private:
        enum {index = IndexOf<TypeList<Tail...>, T>::value};
        
    public:
        enum {value = index == -1 ? -1 : index + 1};
    };
    
    template<typename TList, typename T> struct Append;
    template<typename... T, typename Head2>
    struct Append<TypeList<T...>, Head2>
    {
        typedef TypeList<T..., Head2> type;
    };
    template<typename Head, typename... T2>
    struct Append<Head, TypeList<T2...> >
    {
        typedef TypeList<Head, T2...> type;
    };
    template<typename... T1, typename... T2>
    struct Append<TypeList<T1...>, TypeList<T2...> >
    {
        typedef TypeList<T1..., T2...> type;
    };
    
    template<typename TList, typename T> struct Erase;
    template<typename Head, typename T>
    struct Erase<TypeList<Head>, T>
    {
        typedef TypeList<Head> type;
    };
    template<typename... Tail, typename T>
    struct Erase<TypeList<T, Tail...>, T>
    {
        typedef TypeList<Tail...> type;
    };
    template<typename Head, typename... Tail, typename T>
    struct Erase<TypeList<Head, Tail...>, T>
    {
        typedef TypeList<Head, typename Erase<TypeList<Tail...>, T>::type> type;
    };
    
    template<typename TList, typename T> struct EraseAll;
    template<typename Head, typename T>
    struct EraseAll<TypeList<Head>, T>
    {
        typedef TypeList<Head> type;
    };
    template<typename... Tail, typename T>
    struct EraseAll<TypeList<T, Tail...>, T>
    {
        typedef typename EraseAll<TypeList<Tail...>, T>::type type;
    };
    template<typename Head, typename... Tail, typename T>
    struct EraseAll<TypeList<Head, Tail...>, T>
    {
        typedef TypeList<Head, typename EraseAll<TypeList<Tail...>, T>::type> type;
    };
    
    template<typename TList> struct RemoveDuplicates;
    template<typename Head>
    struct RemoveDuplicates<TypeList<Head> >
    {
        typedef TypeList<Head> type;
    };
    template<typename Head, typename... Tail>
    struct RemoveDuplicates<TypeList<Head, Tail...> >
    {
    private:
        typedef typename RemoveDuplicates<TypeList<Tail...> >::type dup;
        typedef typename Erase<dup, Head>::type nodup;
        
    public:
        typedef TypeList<Head, nodup> type;
    };
    
    template<typename TList, typename T, typename U> struct Replace;
    template<typename Head, typename T, typename U>
    struct Replace<TypeList<Head>, T, U>
    {
        typedef TypeList<Head> type;
    };
    template<typename... Tail, typename T, typename U>
    struct Replace<TypeList<T, Tail...>, T, U>
    {
        typedef TypeList<U, Tail...> type;
    };
    template<typename Head, typename... Tail, typename T, typename U>
    struct Replace<TypeList<Head, Tail...>, T, U>
    {
        typedef TypeList<Head, typename Replace<TypeList<Tail...>, T, U>::type > type;
    };
    
    template<typename TList, typename T, typename U> struct ReplaceAll;
    template<typename Head, typename T, typename U>
    struct ReplaceAll<TypeList<Head>, T, U>
    {
        typedef TypeList<Head> type;
    };
    template<typename... Tail, typename T, typename U>
    struct ReplaceAll<TypeList<T, Tail...>, T, U>
    {
        typedef TypeList<U, typename ReplaceAll<TypeList<Tail...>, T, U>::type > type;
    };
    template<typename Head, typename... Tail, typename T, typename U>
    struct ReplaceAll<TypeList<Head, Tail...>, T, U>
    {
        typedef TypeList<Head, typename ReplaceAll<TypeList<Tail...>, T, U>::type > type;
    };
    
    template<typename TList> struct Reverse;
    template<typename Head>
    struct Reverse<TypeList<Head> >
    {
        typedef TypeList<Head> type;
    };
    template<typename Head, typename... Tail>
    struct Reverse<TypeList<Head, Tail...> >
    {
        typedef typename Append<typename Reverse<TypeList<Tail...>>::type, Head>::type type;
    };
    
    template<typename TList, typename Base> struct MostDerived;
    template<typename Head, typename Base>
    struct MostDerived<TypeList<Head>, Base>
    {
        typedef typename Select<SUPERSUBCLASS(Base, Head), Head, Base>::type type;
    };
    template<typename Head, typename... Tail, typename Base>
    struct MostDerived<TypeList<Head, Tail...>, Base>
    {
    private:
        typedef typename MostDerived<TypeList<Tail...>, Base>::type mostdev;
        
    public:
        typedef typename Select<SUPERSUBCLASS(mostdev, Head), Head, mostdev>::type type;
    };
    
    template<typename TList> struct DerivedToFront;
    template<typename Head>
    struct DerivedToFront<TypeList<Head> >
    {
        typedef TypeList<Head> type;
    };
    template<typename Head, typename... Tail>
    struct DerivedToFront<TypeList<Head, Tail...> >
    {
    private:
        typedef typename MostDerived<TypeList<Tail...>, Head>::type mostdev;
        typedef typename Replace<TypeList<Tail...>, mostdev, Head>::type tail;
        
    public:
        typedef TypeList<mostdev, tail> type;
    };
}

#endif //TYPELIST_H

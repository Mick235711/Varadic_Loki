//
// Created by lee on 17/7/4.
//

#ifndef TYPELIST_TYPEMANIP_H
#define TYPELIST_TYPEMANIP_H

template<int v>
struct Int2Type
{
    enum {value = v};
};

template<typename T>
struct Type2Type
{
    typedef T OriginalType;
};


template<bool choose, typename T, typename U>
struct Select
{
    typedef T type;
};
template<typename T, typename U>
struct Select<false, T, U>
{
    typedef U type;
};

template<typename T, typename U>
struct IsSameType
{
    enum {value = false};
};
template<typename T>
struct IsSameType<T, T>
{
    enum {value = true};
};

namespace detail
{
    template<typename T, typename U>
    struct ConversionHelper
    {
        typedef char Small;
        class Big {char dummy[2];};
        static Small Test(U);
        static Big Test(...);
        static T MakeT();
    };
} // namespace detail

template<typename T, typename U>
class Conversion
{
    typedef detail::ConversionHelper<T, U> H;

public:
    enum {exists = sizeof(H::Test(H::MakeT())) == sizeof(typename H::Small)};
    enum {exists2Way = exists && Conversion<U, T>::exists};
    enum {sameType = false};
};
template<typename T>
class Conversion<T, T>
{
public:
    enum {exists = 1};
    enum {exists2Way = 1};
    enum {sameType = 1};
};
template<typename T>
class Conversion<void, T>
{
public:
    enum {exists = 0};
    enum {exists2Way = 0};
    enum {sameType = 0};
};
template<typename T>
class Conversion<T, void>
{
public:
    enum {exists = 0};
    enum {exists2Way = 0};
    enum {sameType = 0};
};
template<>
class Conversion<void, void>
{
public:
    enum {exists = 0};
    enum {exists2Way = 0};
    enum {sameType = 0};
};

template<typename T, typename U>
struct SuperSubclass
{
    enum {value = (Conversion<const volatile U *, const volatile T *>::exists &&
            !Conversion<const volatile T *, const volatile void *>::sameType)};
};
template<typename T, typename U>
struct SuperSubclassStrict
{
    enum {value = (Conversion<const volatile U *, const volatile T *>::exists &&
            !Conversion<const volatile T *, const volatile void *>::sameType &&
            !Conversion<const volatile T*, const volatile U*>::sameType)};
};

#define SUPERSUBCLASS(T, U) \
    (SuperSubclass<T, U>::value)

#define SUPERSUBCLASS_STRICT(T, U) \
    (SuperSubclassStrict<T, U>::value)

#endif //TYPELIST_TYPEMANIP_H

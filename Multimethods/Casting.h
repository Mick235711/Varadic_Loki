//
// Created by lee on 17/7/10.
//

#ifndef MULTIMETHODS_CASTING_H
#define MULTIMETHODS_CASTING_H

// StaticCaster
// Implementation of CastingPolicy using static_cast
template<typename To, typename From>
struct StaticCaster
{
    static To& Cast(From& obj)
    {
        return static_cast<To&>(obj);
    }
};

// DynamicCaster
// Implementation of CastingPolicy using dynamic_cast
template<typename To, typename From>
struct DynamicCaster
{
    static To& Cast(From& obj)
    {
        return dynamic_cast<To&>(obj);
    }
};

#endif //MULTIMETHODS_CASTING_H

//
// Created by lee on 17/7/5.
//

#ifndef FACTORY_CLONEFACTORY_H
#define FACTORY_CLONEFACTORY_H

#include "FactoryError.h"
#include "TypeInfo/TypeInfo.h"

// CloneFactory
// A generic cloning factory
template
    <
        typename AbstractProduct,
        typename ProductCreator = AbstractProduct* (*)(const AbstractProduct*),
        template<typename, typename>
            class FactoryErrorPolicy = DefaultFactoryError
    >
class CloneFactory : public FactoryErrorPolicy<TypeInfo, AbstractProduct>
{
public:
    bool Register(const TypeInfo& id, ProductCreator creator)
    {
        return m_associations.insert(typename IdToProductMap::value_type(id, creator)).second;
    }
    
    bool Unregister(const TypeInfo& id)
    {
        return m_associations.erase(id) == 1;
    }
    
    AbstractProduct* CreateObject(const AbstractProduct* model)
    {
        if (model == nullptr) return nullptr;
        
        typename IdToProductMap::iterator i = m_associations.find(typeid(*model));
        if (i != m_associations.end())
        {
            return (i->second)(model);
        }
        return this->OnUnknownType(typeid(*model));
    }

private:
    typedef AssocVector<TypeInfo, ProductCreator> IdToProductMap;
    IdToProductMap m_associations;
};

#endif //FACTORY_CLONEFACTORY_H

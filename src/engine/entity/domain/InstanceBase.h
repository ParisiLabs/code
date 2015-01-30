/*
 * InstanceBase.h
 *
 *  Created on: 18.01.2015
 *      Author: aschaeffer
 */

#ifndef SRC_ENGINE_ENTITY_DOMAIN_INSTANCEBASE_H_
#define SRC_ENGINE_ENTITY_DOMAIN_INSTANCEBASE_H_

#include "shared/uuid.h"

#include "../EntitySystemBase.h"
#include "AttributeBase.h"

class InstanceBase : public AttributeBase
{

    public:
        InstanceBase();
        InstanceBase(std::string uuid);
        virtual ~InstanceBase();

        std::string GetUuid();
        void SetUuid(std::string uuid);

    protected:
        std::string uuid;

    private:
        // Include the default reference counting implementation.
        IMPLEMENT_REFCOUNTING(InstanceBase);

};

#endif /* SRC_ENGINE_ENTITY_DOMAIN_INSTANCEBASE_H_ */
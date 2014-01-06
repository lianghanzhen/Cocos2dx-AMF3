//
//  Created by lianghanzhen on 12/30/13.
//

#include "CCDynamicObject.h"

CCRuntimeClass * CCRuntimeClass::firstClass = NULL;

CLASS_INIT::CLASS_INIT(CCRuntimeClass *newClass) {
    newClass->nextClass = CCRuntimeClass::firstClass;
    CCRuntimeClass::firstClass = newClass;
}

struct CCRuntimeClass CCDynamicObject::classCCDynamicObject = { "CCDynamicObject", CCDynamicObject::createObject, NULL };
static CLASS_INIT _initCCDynamicObject(&CCDynamicObject::classCCDynamicObject);
CCRuntimeClass *CCDynamicObject::getRuntimeClass() const {
    return &CCDynamicObject::classCCDynamicObject;
}

bool CCDynamicObject::isInstanceOf(const CCRuntimeClass *theClass) const {
    for (CCRuntimeClass *thisClass = getRuntimeClass(); thisClass != NULL; thisClass = thisClass->baseClass) {
        if (thisClass == theClass)
            return true;
    }
    return false;
}

CCDynamicObject *CCDynamicObject::createObject() {
    return new CCDynamicObject();
}

CCDynamicObject *CCRuntimeClass::createObject() {
    if (pfnCreateObject == NULL) {
        CCLOG("Error: Trying to create object which is not 'DECLARE_DYNAMIC_CREATE' : %s", className);
        return NULL;
    }
    return (*pfnCreateObject)();
}

CCRuntimeClass *CCRuntimeClass::load(const char *className) {
    for (CCRuntimeClass *thisClass = firstClass; thisClass != NULL; thisClass = thisClass->nextClass) {
        if (strcmp(className, thisClass->className) == 0)
            return thisClass;
    }
    CCLOG("Error: Class not found: %s", className);
    return NULL;
}

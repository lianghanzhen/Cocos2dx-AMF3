//
//  Created by lianghanzhen on 12/30/13.
//


#ifndef __CCDynamicObject_H_
#define __CCDynamicObject_H_

#include "cocos2d.h"

#define DECLARE_DYNAMIC(__CLASS_NAME__) \
public: \
    static CCRuntimeClass class##__CLASS_NAME__; \
    virtual CCRuntimeClass * getRuntimeClass() const;

#define IMPLEMENT_DYNAMIC(__CLASS_NAME__, __BASE_CLASS_NAME__) \
    _IMPLEMENT_RUNTIME_CLASS(__CLASS_NAME__, __BASE_CLASS_NAME__, NULL)

#define _IMPLEMENT_RUNTIME_CLASS(__CLASS_NAME__, __BASE_CLASS_NAME__, __CREATE_OBJECT__) \
    static char *_name##__CLASS_NAME__ = #__CLASS_NAME__; \
    CCRuntimeClass __CLASS_NAME__::class##__CLASS_NAME__ = { \
        _name##__CLASS_NAME__, __CREATE_OBJECT__, RUNTIME_CLASS(__BASE_CLASS_NAME__), NULL }; \
    static CLASS_INIT _init##__CLASS_NAME__(&__CLASS_NAME__::class##__CLASS_NAME__); \
    CCRuntimeClass * __CLASS_NAME__::getRuntimeClass() const { \
        return &__CLASS_NAME__::class##__CLASS_NAME__; \
    }

#define RUNTIME_CLASS(__CLASS_NAME__) \
    (&__CLASS_NAME__::class##__CLASS_NAME__)

#define DECLARE_DYNAMIC_CREATE(__CLASS_NAME__) \
    DECLARE_DYNAMIC(__CLASS_NAME__) \
    static CCDynamicObject * createObject();

#define IMPLEMENT_DYNAMIC_CREATE(__CLASS_NAME__, __BASE_CLASS_NAME__) \
    CCDynamicObject * __CLASS_NAME__::createObject() { \
        return new __CLASS_NAME__(); \
    } \
    _IMPLEMENT_RUNTIME_CLASS(__CLASS_NAME__, __BASE_CLASS_NAME__, __CLASS_NAME__::createObject)

class CCDynamicObject;

struct CCRuntimeClass {

    char *className;
    CCDynamicObject * (*pfnCreateObject)(); // NULL for abstract class
    CCRuntimeClass *baseClass;
    CCDynamicObject * createObject();
    static CCRuntimeClass *load(const char *className);

    // CCRuntimeClass objects linked together in simple list
    static CCRuntimeClass *firstClass;
    CCRuntimeClass *nextClass;

};

struct CLASS_INIT {

    CLASS_INIT(CCRuntimeClass *newClass);

};


class CCDynamicObject : public cocos2d::CCObject {

public:
    CCDynamicObject() {}
    ~CCDynamicObject() {}

    virtual CCRuntimeClass * getRuntimeClass() const;
    bool isInstanceOf(const CCRuntimeClass *theClass) const;

    static CCDynamicObject * createObject();

public:
    static CCRuntimeClass classCCDynamicObject;

};


#endif //__CCDynamicObject_H_

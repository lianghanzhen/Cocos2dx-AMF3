//
//  Created by lianghanzhen on 12/30/13.
//


#ifndef __AmfCodingCache_H_
#define __AmfCodingCache_H_

#include "cocos2d.h"
#include "AmfTraitsInfo.h"

class AmfRefCache : public cocos2d::CCObject {

public:
    ~AmfRefCache();
    bool init();
    CREATE_FUNC(AmfRefCache);
    void reset();

    int setStringReference(cocos2d::CCString *string);
    cocos2d::CCString * getStringReference(int index);
    int setObjectReference(cocos2d::CCObject *object);
    void replaceObjectReference(cocos2d::CCObject *oldObject, cocos2d::CCObject *newObject);
    cocos2d::CCObject * getObjectReference(int index);
    int setTraitsReference(AmfTraitsInfo *amfTraitsInfo);
    AmfTraitsInfo * getTraitsReference(int index);

private:
    AmfRefCache() : _stringRefArray(NULL), _objectRefArray(NULL), _traitsRefArray(NULL) {}

    cocos2d::CCArray *_stringRefArray;
    cocos2d::CCArray *_objectRefArray;
    cocos2d::CCArray *_traitsRefArray;

};


#endif //__AmfCodingCache_H_

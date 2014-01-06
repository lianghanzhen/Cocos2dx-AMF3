//
//  Created by lianghanzhen on 12/30/13.
//

#include "AmfRefCache.h"
#include "AmfUtil.h"

USING_NS_CC;

AmfRefCache::~AmfRefCache() {
    CC_SAFE_RELEASE_NULL(_stringRefArray);
    CC_SAFE_RELEASE_NULL(_objectRefArray);
    CC_SAFE_RELEASE_NULL(_traitsRefArray);
}

bool AmfRefCache::init() {
    return true;
}

void AmfRefCache::reset() {
    if (_stringRefArray) _stringRefArray->removeAllObjects();
    if (_objectRefArray) _objectRefArray->removeAllObjects();
    if (_traitsRefArray) _traitsRefArray->removeAllObjects();
}

int AmfRefCache::setStringReference(cocos2d::CCString *string) {
    if (!_stringRefArray) {
        _stringRefArray = CCArray::create();
        _stringRefArray->retain();
    }
    int ref = AmfUtil::getItemIndex(_stringRefArray, string);
    if (ref <= INVALID_INDEX)
        _stringRefArray->addObject(string);
    return ref;
}

cocos2d::CCString *AmfRefCache::getStringReference(int index) {
    if (_stringRefArray && index >= 0 && index < _stringRefArray->count())
        return dynamic_cast<CCString *>(_stringRefArray->objectAtIndex(index));
    CCLOG("AmfRefCache::getStringReference - Error : Undefined string reference #%d", index);
    return NULL;
}

int AmfRefCache::setObjectReference(cocos2d::CCObject *object) {
    if (!_objectRefArray) {
        _objectRefArray = CCArray::create();
        _objectRefArray->retain();
    }
    int ref = AmfUtil::getItemIndex(_objectRefArray, object);
    if (ref <= INVALID_INDEX)
        _objectRefArray->addObject(object);
    return ref;
}

void AmfRefCache::replaceObjectReference(cocos2d::CCObject *oldObject, cocos2d::CCObject *newObject) {
    if (!_objectRefArray) {
        _objectRefArray = CCArray::create();
        _objectRefArray->retain();
    }
    int ref = AmfUtil::getItemIndex(_objectRefArray, oldObject);
    if (ref <= INVALID_INDEX) {
        _objectRefArray->addObject(newObject);
    } else {
        _objectRefArray->replaceObjectAtIndex(ref, newObject);
    }
}

cocos2d::CCObject *AmfRefCache::getObjectReference(int index) {
    if (_objectRefArray && index >= 0 && index < _objectRefArray->count())
        return _objectRefArray->objectAtIndex(index);
    CCLOG("AmfRefCache::getObjectReference - Error : Undefined object reference #%d", index);
    return NULL;
}

int AmfRefCache::setTraitsReference(AmfTraitsInfo *amfTraitsInfo) {
    if (!_traitsRefArray) {
        _traitsRefArray = CCArray::create();
        _traitsRefArray->retain();
    }
    int ref = AmfUtil::getItemIndex(_traitsRefArray, amfTraitsInfo);
    if (ref <= INVALID_INDEX)
        _traitsRefArray->addObject(amfTraitsInfo);
    return ref;
}

AmfTraitsInfo *AmfRefCache::getTraitsReference(int index) {
    if (_traitsRefArray && index >= 0 && index < _traitsRefArray->count())
        return dynamic_cast<AmfTraitsInfo *>(_traitsRefArray->objectAtIndex(index));
    CCLOG("AmfRefCache::getTraitsReference - Error : Undefined traits reference #%d", index);
    return NULL;
}

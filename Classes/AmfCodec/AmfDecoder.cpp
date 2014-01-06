//
//  Created by lianghanzhen on 12/30/13.
//

#include "AmfDecoder.h"
#include "AmfEndiness.h"
#include "RTTI/CCDynamicObject.h"
#include "AmfCoding.h"

USING_NS_CC;

AmfDecoder::AmfDecoder(unsigned char *buffer, size_t length) {
    _cursor = AmfCursor::init(buffer, length);
    _refCache = NULL;
}

AmfDecoder * AmfDecoder::createWithData(unsigned char *data, size_t length) {
    return new AmfDecoder(data, length);
}

AmfDecoder::~AmfDecoder() {
    CC_SAFE_DELETE(_cursor);
    CC_SAFE_RELEASE_NULL(_refCache);
}

bool AmfDecoder::decodeBoolForKey(const char *key) {
    CCBool *value = dynamic_cast<CCBool *>(_objectToDeserialized->valueForKey(key));
    return value ? value->getValue() : false;
}

int AmfDecoder::decodeIntForKey(const char *key) {
    CCInteger *value = dynamic_cast<CCInteger *>(_objectToDeserialized->valueForKey(key));
    return value ? value->getValue() : 0;
}

float AmfDecoder::decodeFloatForKey(const char *key) {
    CCDouble *value = dynamic_cast<CCDouble *>(_objectToDeserialized->valueForKey(key));
    return value ? (float) value->getValue() : 0.0f;
}

double AmfDecoder::decodeDoubleForKey(const char *key) {
    CCDouble *value = dynamic_cast<CCDouble *>(_objectToDeserialized->valueForKey(key));
    return value ? value->getValue() : 0.0;
}

CCString * AmfDecoder::decodeStringForKey(const char *key) {
    return dynamic_cast<CCString *>(_objectToDeserialized->valueForKey(key));
}

CCDate * AmfDecoder::decodeDateForKey(const char *key) {
    return dynamic_cast<CCDate *>(_objectToDeserialized->valueForKey(key));
}

CCArray * AmfDecoder::decodeArrayForKey(const char *key) {
    return dynamic_cast<CCArray *>(_objectToDeserialized->valueForKey(key));
}

CCObject * AmfDecoder::decodeObjectForKey(const char *key) {
    return _objectToDeserialized->valueForKey(key);
}

bool AmfDecoder::decodeBool() {
    const size_t length = 1;
    _cursor->needBytes(length);
    AMF3Type type = (AMF3Type) _cursor->getByte(0);
    CCAssert(type == kAMF3TrueType || type == kAMF3FalseType, "Not Bool Type!!!");
    _cursor->consumeBytes(length);
    return type == kAMF3TrueType;
}

int AmfDecoder::decodeInt() {
    _checkType(kAMF3IntegerType, "Not Integer Type!!!");
    return this->_decodeInt();
}

float AmfDecoder::decodeFloat() {
    return (float) this->decodeDouble();
}

double AmfDecoder::decodeDouble() {
    _checkType(kAMF3DoubleType, "Not Double Type!!!");
    return this->_decodeDouble();
}

CCString * AmfDecoder::decodeString() {
    _checkType(kAMF3StringType, "Not String Type!!!");
    return this->_decodeString();
}

CCDate *AmfDecoder::decodeDate() {
    _checkType(kAMF3DateType, "Not Date Type!!!");
    return this->_decodeDate();
}

CCArray * AmfDecoder::decodeArray() {
    _checkType(kAMF3ArrayType, "Not Array Type!!!");
    return this->_decodeArray();
}

CCObject * AmfDecoder::decodeObject() {
    AMF3Type type = _getType();
    switch (type) {
        case kAMF3NullType:
        case kAMF3UndefinedType:
            return NULL;
        case kAMF3TrueType:
            return CCBool::create(true);
        case kAMF3FalseType:
            return CCBool::create(false);
        case kAMF3IntegerType:
            return CCInteger::create(this->_decodeInt());
        case kAMF3DoubleType:
            return CCDouble::create(this->_decodeDouble());
        case kAMF3StringType:
            return this->_decodeString();
        case kAMF3DateType:
            return this->_decodeDate();
        case kAMF3ArrayType:
            return this->_decodeArray();
        case kAMF3ObjectType:
            return this->_decodeAmfObject();
        default:
            CCLOG("Unsupported type: %d", type);
            return NULL;
    }
}

int AmfDecoder::_decodeInt() {
    return (_cursor->readU29() << 3) >> 3;
}

float AmfDecoder::_decodeFloat() {
    return (float) _decodeDouble();
}

double AmfDecoder::_decodeDouble() {
    const size_t length = 8;
    _cursor->needBytes(length);
    double result;
    memcpy(&result, _cursor->getBuffer(), length);
    REVERSE_IF_LITTLE_ENDIAN(&result, length);
    _cursor->consumeBytes(length);
    return result;
}

CCString * AmfDecoder::_decodeString() {
    unsigned int ref = (unsigned int) _cursor->readU29();
    if (this->_isReference(ref)) {
        return _refCache->getStringReference(ref >> 1);
    } else {
        unsigned int length = ref >> 1;
        if (length == 0)
            return CCStringMake("");
        CCString *string = CCStringMake(_cursor->readString(length));
        this->_setStringReference(string);
        return string;
    }
}

CCDate * AmfDecoder::_decodeDate() {
    unsigned int ref = (unsigned int) _cursor->readU29();
    if (this->_isReference(ref)) {
        return dynamic_cast<CCDate *>(_refCache->getObjectReference(ref >> 1));
    } else {
        CCDate *date = CCDate::create();
        date->setTime(_decodeDouble() / 1000.0);
        this->_setObjectReference(date);
        return date;
    }
}

CCArray * AmfDecoder::_decodeArray() {
    unsigned int ref = (unsigned int) _cursor->readU29();
    if (this->_isReference(ref)) {
        return dynamic_cast<CCArray *>(_refCache->getObjectReference(ref >> 1));
    } else {
        unsigned int length = ref >> 1;
        /* read empty string because there are no named key */
        _cursor->readU29();
        CCArray *array = CCArray::createWithCapacity(length);
        this->_setObjectReference(array);
        for (int i = 0; i < length; i++)
            array->addObject(this->decodeObject());
        return array;
    }
}

CCObject *AmfDecoder::_decodeAmfObject() {
    unsigned int ref = (unsigned int) _cursor->readU29();
    if (this->_isReference(ref)) {
        return _refCache->getObjectReference(ref >> 1);
    } else {
        AmfTraitsInfo *traits = this->_decodeTraits(ref);
        traits->retain();
        // do not support dynamic object
        if (traits->getClassName() && strlen(traits->getClassName()) > 0) {
            AmfObject *object = AmfObject::create();
            object->retain();
            this->_setObjectReference(object);
            object->setClassName(traits->getClassName());
            object->setIsExternalizable(traits->getIsExternalizable());
            CCObject *key;
            CCARRAY_FOREACH(traits->getProperties(), key) {
                    CCObject *value = this->decodeObject();
                    if (dynamic_cast<CCString *>(key)) {
                        object->setValueForKey(value, dynamic_cast<CCString *>(key)->getCString());
                    }
                }
            if (traits->getIsDynamic()) {
                for (CCString *k = this->_decodeString(); k && strlen(k->getCString()) > 0; k = this->_decodeString()) {
                    object->setValueForKey(this->decodeObject(), k->getCString());
                }
            }

            CCObject *resultObj = this->_decodeObject(object);
            this->_replaceObjectReference(object, resultObj);
            object->release();
            traits->release();
            return resultObj;
        }
        return NULL;
    }
}

AmfTraitsInfo *AmfDecoder::_decodeTraits(int ref) {
    if ((ref & 3) == 1) {
        return _refCache->getTraitsReference(ref >> 2);
    }

    AmfTraitsInfo *traits = AmfTraitsInfo::create();
    traits->retain();
    traits->setClassName(this->_decodeString()->getCString());
    traits->setIsDynamic((ref & 8) == 8);
    traits->setIsExternalizable((ref & 4) == 4);
    unsigned int count = (unsigned int) (ref >> 4);
    traits->setCount(count);
    for (int i = 0; i < count; i++) {
        traits->addProperty(this->_decodeString()->getCString());
    }
    _setTraitsReference(traits);
    traits->release();
    return traits;
}

cocos2d::CCObject *AmfDecoder::_decodeObject(AmfObject *object) {
    if (!object->getClassName())
        return object;

    std::string str = std::string(object->getClassName());
    const char *className = str.substr(str.find_last_of('.') + 1, str.length()).c_str();
    CCRuntimeClass *thisClass = CCRuntimeClass::load(className);
    CCAssert(thisClass, "Cannot find CCRuntimeClass!!!");
    AmfObject *lastObject = _objectToDeserialized;
    _objectToDeserialized = object;
    AmfCoding *resultObj = dynamic_cast<AmfCoding *>(thisClass->createObject());
    resultObj->initWithDecoder(this);
    _objectToDeserialized = lastObject;
    return resultObj;
}

bool AmfDecoder::_isReference(unsigned int ref) {
    return (ref & 1) == 0;
}

AMF3Type AmfDecoder::_getType() {
    const size_t length = 1;
    _cursor->needBytes(length);
    AMF3Type type = (AMF3Type) _cursor->getByte(0);
    _cursor->consumeBytes(length);
    return type;
}

void AmfDecoder::_checkType(AMF3Type type, const char *message) {
    CCAssert(_getType() == type, message);
}

void AmfDecoder::_setStringReference(CCString *string) {
    if (!_refCache) {
        _refCache = AmfRefCache::create();
        _refCache->retain();
    }
    _refCache->setStringReference(string);
}

int AmfDecoder::_setObjectReference(CCObject *object) {
    if (!_refCache) {
        _refCache = AmfRefCache::create();
        _refCache->retain();
    }
    return _refCache->setObjectReference(object);
}

void AmfDecoder::_replaceObjectReference(cocos2d::CCObject *oldObject, cocos2d::CCObject *newObject) {
    if (!_refCache) {
        _refCache = AmfRefCache::create();
        _refCache->retain();
    }
    _refCache->replaceObjectReference(oldObject, newObject);
}

void AmfDecoder::_setTraitsReference(AmfTraitsInfo *traits) {
    if (!_refCache) {
        _refCache = AmfRefCache::create();
        _refCache->retain();
    }
    _refCache->setTraitsReference(traits);
}

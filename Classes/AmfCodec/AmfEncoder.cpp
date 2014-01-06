//
//  Created by lianghanzhen on 12/29/13.
//

#include "AmfEncoder.h"
#include "Amf.h"
#include "AmfUtil.h"
#include "CCNull.h"

USING_NS_CC;

#define LENGTH(x) (x << 1 | 1)
#define EMPTY_STRING LENGTH(0)

AmfEncoder::AmfEncoder() : _objectToSerialized(NULL),  _serializedObjectStack(NULL), _refCache(NULL) {
    _buffer = AmfBuffer::init();
}

AmfEncoder::~AmfEncoder() {
    CC_SAFE_DELETE(_buffer);
    CC_SAFE_RELEASE_NULL(_objectToSerialized);
    CC_SAFE_RELEASE_NULL(_serializedObjectStack);
    CC_SAFE_RELEASE_NULL(_refCache);
}

AmfEncoder * AmfEncoder::create() {
    return new AmfEncoder();
}

void AmfEncoder::reset() {
    _buffer->reset();
    if (_objectToSerialized) _objectToSerialized->reset();
    if (_serializedObjectStack) _serializedObjectStack->removeAllObjects();
    if (_refCache) _refCache->reset();
}

char * AmfEncoder::getData() {
    return _buffer->getData();
}

unsigned int AmfEncoder::getDataLength() {
    return _buffer->getLength();
}

void AmfEncoder::encodeBoolForKey(bool value, const char *key) {
    _setSerializedValueForKey(CCBool::create(value), key);
}

void AmfEncoder::encodeIntForKey(int value, const char *key) {
    _setSerializedValueForKey(CCInteger::create(value), key);
}

void AmfEncoder::encodeFloatForKey(float value, const char *key) {
    _setSerializedValueForKey(CCFloat::create(value), key);
}

void AmfEncoder::encodeDoubleForKey(double value, const char *key) {
    _setSerializedValueForKey(CCDouble::create(value), key);
}

void AmfEncoder::encodeStringForKey(CCString *value, const char *key) {
    _setSerializedValueForKey(value, key);
}

void AmfEncoder::encodeDateForKey(CCDate *value, const char *key) {
    _setSerializedValueForKey(value, key);
}

void AmfEncoder::encodeArrayForKey(CCArray *value, const char *key) {
    _setSerializedValueForKey(value, key);
}

void AmfEncoder::encodeObjectForKey(CCObject *value, const char *key) {
    _setSerializedValueForKey(value, key);
}


void AmfEncoder::encodeBool(bool value) {
    if (!this->_addSerializedObject(CCBool::create(value)))
        _buffer->appendChar(value ? kAMF3TrueType : kAMF3FalseType);
}

void AmfEncoder::encodeInt(int value) {
    if (!this->_addSerializedObject(CCInteger::create(value))) {
        if (value < AMF3_MIN_INT || value > AMF3_MAX_INT) {
            encodeDouble(value);
        } else {
            _buffer->appendChar(kAMF3IntegerType);
            _buffer->appendU29(value);
        }
    }
}

void AmfEncoder::encodeFloat(float value) {
    if (!this->_addSerializedObject(CCFloat::create(value)))
        encodeDouble(value);
}

void AmfEncoder::encodeDouble(double value) {
    if (!this->_addSerializedObject(CCDouble::create(value))) {
        _buffer->appendChar(kAMF3DoubleType);
        _buffer->appendDouble(value);
    }
}

void AmfEncoder::encodeString(CCString *value) {
    if (value->length() <= 0 || !this->_addSerializedObject(value)) {
        _encodeString(value, true);
    }
}

void AmfEncoder::encodeDate(CCDate *value) {
    if (!this->_addSerializedObject(value)) {
        _buffer->appendChar(kAMF3DateType);
        if (this->_setObjectReference(value)) {
            _buffer->appendU29((0 << 1) | 1);
            _buffer->appendDouble(value->getTime() * 1000.0);
        }
    }
}

void AmfEncoder::encodeArray(CCArray *value) {
    if (!this->_addSerializedObject(value)) {
        _buffer->appendChar(kAMF3ArrayType);
        if (this->_setObjectReference(value)) {
            int len = value->count();
            _buffer->appendU29(LENGTH(len));
            /* Send an empty string to imply no named keys */
            _buffer->appendU29(EMPTY_STRING);
            CCObject *obj;
            CCARRAY_FOREACH(value, obj) {
                    this->encodeObject(obj);
                }
        }
    }
}

void AmfEncoder::encodeObject(CCObject *value) {
    if (dynamic_cast<CCNull *>(value)) {
        _buffer->appendChar(kAMF3NullType);
    } else if (dynamic_cast<CCBool *>(value)) {
        this->encodeBool(dynamic_cast<CCBool *>(value)->getValue());
    } else if (dynamic_cast<CCInteger *>(value)) {
        this->encodeInt(dynamic_cast<CCInteger *>(value)->getValue());
    } else if (dynamic_cast<CCFloat *>(value)) {
        this->encodeFloat(dynamic_cast<CCFloat *>(value)->getValue());
    } else if (dynamic_cast<CCDouble *>(value)) {
        this->encodeDouble(dynamic_cast<CCDouble *>(value)->getValue());
    } else if (dynamic_cast<CCString *>(value)) {
        this->encodeString(dynamic_cast<CCString *>(value));
    } else if (dynamic_cast<CCDate *>(value)) {
        this->encodeDate(dynamic_cast<CCDate *>(value));
    } else if (dynamic_cast<CCArray *>(value)) {
        this->encodeArray(dynamic_cast<CCArray *>(value));
    } else if (dynamic_cast<AmfObject *>(value)) {
        this->_encodeAmfObject(dynamic_cast<AmfObject *>(value));
    } else if (dynamic_cast<AmfCoding *>(value)) {
        if (!this->_addSerializedObject(value)) {
            this->_encodeCustomObject(dynamic_cast<AmfCoding *>(value));
        }
    } else {
        CCLOG("Unsupported Amf encoded type!!!");
    }
}

void AmfEncoder::_encodeCustomObject(AmfCoding *customObject) {
    AmfObject *lastObject = _objectToSerialized;
    _objectToSerialized = AmfObject::create();
    _objectToSerialized->retain();
    AmfObject *object = _objectToSerialized;
    if (!_serializedObjectStack) {
        _serializedObjectStack = CCArray::create();
        _serializedObjectStack->retain();
    }
    _serializedObjectStack->addObject(_objectToSerialized);

    object->setClassName(customObject->getClassName());
    customObject->encodeWithEncoder(this);

    _objectToSerialized->release();
    _objectToSerialized = lastObject;
    _serializedObjectStack->removeLastObject();
    if (!lastObject)
        this->encodeObject(object);
}

void AmfEncoder::_encodeString(cocos2d::CCString *value, bool encodeType) {
    if (encodeType) _buffer->appendChar(kAMF3StringType);

    size_t len = value->length();
    if (len <= 0) { // empty string
        _buffer->appendU29(EMPTY_STRING);
    } else {
        if (len > AMF3_MAX_STR_LEN)
            len = AMF3_MAX_STR_LEN;
        if (_setStringReference(value)) {
            _buffer->appendU29(LENGTH(len));
            _buffer->append(value->getCString(), len);
        }
    }
}

void AmfEncoder::_encodeAmfObject(AmfObject *value) {
    _buffer->appendChar(kAMF3ObjectType);
    if (this->_setObjectReference(value)) {
        AmfTraitsInfo *amfTraitsInfo = AmfTraitsInfo::create();
        amfTraitsInfo->retain();
        const char *className = value->getClassName();
        amfTraitsInfo->setClassName(className);
        amfTraitsInfo->setIsDynamic(className == NULL || strlen(className) == 0);
        amfTraitsInfo->setIsExternalizable(value->getIsExternalizable());
        amfTraitsInfo->setCount((unsigned int) (amfTraitsInfo->getIsDynamic() || amfTraitsInfo->getIsExternalizable() ? 0 : value->count()));
        amfTraitsInfo->setProperties(amfTraitsInfo->getIsDynamic() ? NULL : value->getPropertyNames());
        this->_encodeTraits(amfTraitsInfo);

        if (value->getIsExternalizable()) {
            CCObject *data;
            CCARRAY_FOREACH(value->getData(), data) {
                    this->encodeObject(data);
                }
        }

        CCObject *key;
        CCARRAY_FOREACH(value->getPropertyNames(), key) {
                if (amfTraitsInfo->getIsDynamic()) {
                    CCString *encodedKey;
                    if (dynamic_cast<CCInteger *>(key)) {
                        std::stringstream stream;
                        stream << dynamic_cast<CCInteger *>(key)->getValue();
                        encodedKey = CCString::create(stream.str());
                    } else {
                        encodedKey = (CCString *) key;
                    }
                    this->encodeString(encodedKey);
                }

                this->encodeObject(value->valueForKey(dynamic_cast<CCString *>(key)->getCString()));
                if (amfTraitsInfo->getIsDynamic())
                    _buffer->appendU29(EMPTY_STRING);
            }
        amfTraitsInfo->release();
    }
}

void AmfEncoder::_encodeTraits(AmfTraitsInfo *amfTraitsInfo) {
    if (this->_setTraitsReference(amfTraitsInfo)) {
        unsigned int infoBits = 3;
        if (amfTraitsInfo->getIsExternalizable()) infoBits |= 4;
        if (amfTraitsInfo->getIsDynamic()) infoBits |= 8;
        infoBits |= (amfTraitsInfo->getCount() << 4);
        _buffer->appendU29(infoBits);
        this->_encodeString(CCString::create(amfTraitsInfo->getClassName()), false);
        CCObject *property;
        CCARRAY_FOREACH(amfTraitsInfo->getProperties(), property) {
                this->_encodeString(dynamic_cast<CCString *>(property), false);
            }
    }
}

void AmfEncoder::_setSerializedValueForKey(CCObject *value, const char *key) {
    if (!value) value = CCNull::create();
    value->retain();
    _objectToSerialized->setValueForKey(value, key);
    this->_ensureIntegrityOfSerializedObject();
    value->release();
}

bool AmfEncoder::_addSerializedObject(CCObject *object) {
    bool result = _objectToSerialized != NULL;
    if (result) {
        _objectToSerialized->addObject(object);
        this->_ensureIntegrityOfSerializedObject();
    }
    return result;
}

void AmfEncoder::_ensureIntegrityOfSerializedObject() {
    CCAssert(!(_objectToSerialized && _objectToSerialized->getData() && _objectToSerialized->getProperties()),
    "You may not mix keyed archiving and non-keyed archiving on the same object!");
}

bool AmfEncoder::_setStringReference(CCString *string) {
    if (!_refCache) {
        _refCache = AmfRefCache::create();
        _refCache->retain();
    }
    int ref = _refCache->setStringReference(string);
    bool result = ref <= INVALID_INDEX;
    if (!result) {
        _buffer->appendU29(ref << 1);
    }
    return result;
}

bool AmfEncoder::_setObjectReference(CCObject *object) {
    if (!_refCache) {
        _refCache = AmfRefCache::create();
        _refCache->retain();
    }
    int ref = _refCache->setObjectReference(object);
    bool result = ref <= INVALID_INDEX;
    if (!result)
        _buffer->appendU29(ref << 1);
    return result;
}

bool AmfEncoder::_setTraitsReference(AmfTraitsInfo *amfTraitsInfo) {
    if (!_refCache) {
        _refCache = AmfRefCache::create();
        _refCache->retain();
    }
    int ref = _refCache->setTraitsReference(amfTraitsInfo);
    bool result = ref <= INVALID_INDEX;
    if (!result) {
        _buffer->appendU29((ref << 2) | 1);
    }
    return result;
}

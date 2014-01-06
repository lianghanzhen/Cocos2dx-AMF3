//
//  Created by lianghanzhen on 12/29/13.
//

#include "AmfObject.h"
#include "AmfUtil.h"

USING_NS_CC;

AmfObject::~AmfObject() {
    CC_SAFE_RELEASE_NULL(_properties);
    CC_SAFE_RELEASE_NULL(_propertyNames);
    CC_SAFE_RELEASE_NULL(_data);
}

bool AmfObject::init() {
    return true;
}

void AmfObject::reset() {
    _className = NULL;
    _isExternalizable = false;
    if (_properties) _properties->removeAllObjects();
    if (_propertyNames) _propertyNames->removeAllObjects();
    if (_data) _data->removeAllObjects();
}

void AmfObject::setValueForKey(CCObject *value, const char *key) {
    if (!_properties)
        setProperties(CCDictionary::create());
    if (!_propertyNames)
        setPropertyNames(CCArray::create());
    if (value) {
        _properties->setObject(value, key);
        _propertyNames->addObject(CCStringMake(key));
    }
}

CCObject * AmfObject::valueForKey(const char *key) {
    return _properties ? _properties->objectForKey(key) : NULL;
}

void AmfObject::addObject(CCObject *object) {
    if (!_data)
        setData(CCArray::create());
    _data->addObject(object);
}

int AmfObject::count() {
    return _isExternalizable ? _data->count() : _properties->count();
}

bool AmfObject::isEqual(CCObject *object) {
    if (!object) return false;
    AmfObject *amfObject = dynamic_cast<AmfObject *>(object);
    if (!amfObject) return false;
    return AmfUtil::isSame(CCStringMake(_className), CCStringMake(amfObject->getClassName()))
                && AmfUtil::isSame(CCBool::create(_isExternalizable), CCBool::create(amfObject->getIsExternalizable()))
                && AmfUtil::isSame(_properties, amfObject->getProperties())
                && AmfUtil::isSame(_data, amfObject->getData());
}

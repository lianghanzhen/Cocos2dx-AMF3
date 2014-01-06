//
//  Created by lianghanzhen on 12/30/13.
//

#include "AmfTraitsInfo.h"
#include "AmfUtil.h"

USING_NS_CC;

AmfTraitsInfo::AmfTraitsInfo() : _className(NULL), _isDynamic(false), _isExternalizable(false), _count(0) {
    _properties = CCArray::create();
    _properties->retain();
}

AmfTraitsInfo::~AmfTraitsInfo() {
    CC_SAFE_RELEASE_NULL(_properties);
}

bool AmfTraitsInfo::init() {
    return true;
}

void AmfTraitsInfo::addProperty(const char *property) {
    _properties->addObject(CCStringMake(property));
}

bool AmfTraitsInfo::isEqual(CCObject *object) {
    if (!object) return false;
    AmfTraitsInfo *traits = dynamic_cast<AmfTraitsInfo *>(object);
    if (!traits) return false;
    return AmfUtil::isSame(CCStringMake(_className), CCStringMake(traits->getClassName()))
            && AmfUtil::isSame(CCBool::create(_isDynamic), CCBool::create(traits->getIsDynamic()))
            && AmfUtil::isSame(CCBool::create(_isExternalizable), CCBool::create(traits->getIsExternalizable()))
            && AmfUtil::isSame(CCInteger::create(_count), CCInteger::create(traits->getCount()))
            && AmfUtil::isSame(_properties, traits->getProperties());
}

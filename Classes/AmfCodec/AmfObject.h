//
//  Created by lianghanzhen on 12/29/13.
//


#ifndef __AmfObject_H_
#define __AmfObject_H_

#include "cocos2d.h"

class AmfObject : public cocos2d::CCObject {

public:
    ~AmfObject();
    virtual bool init();
    CREATE_FUNC(AmfObject);
    void reset();

    void setValueForKey(cocos2d::CCObject *value, const char *key);
    cocos2d::CCObject * valueForKey(const char *key);
    void addObject(cocos2d::CCObject *object);
    int count();

    virtual bool isEqual(CCObject *object);

private:
    AmfObject() : _className(""), _isExternalizable(false), _properties(NULL), _propertyNames(NULL), _data(NULL) {}

    CC_SYNTHESIZE(const char *, _className, ClassName);
    CC_SYNTHESIZE(bool, _isExternalizable, IsExternalizable);
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, _propertyNames, PropertyNames);  // for AmfEncoder to keep properties encode order
    CC_SYNTHESIZE_RETAIN(cocos2d::CCDictionary *, _properties, Properties);
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, _data, Data);

};


#endif //__AmfObject_H_

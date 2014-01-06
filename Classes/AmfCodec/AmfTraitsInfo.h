//
//  Created by lianghanzhen on 12/30/13.
//


#ifndef __AmfTraitsInfo_H_
#define __AmfTraitsInfo_H_

#include "cocos2d.h"

class AmfTraitsInfo : public cocos2d::CCObject {

public:
    ~AmfTraitsInfo();
    virtual bool init();
    CREATE_FUNC(AmfTraitsInfo);

    void addProperty(const char *property);
    virtual bool isEqual(CCObject *object);

private:
    AmfTraitsInfo();

    CC_SYNTHESIZE(const char *, _className, ClassName);
    CC_SYNTHESIZE(bool, _isDynamic, IsDynamic);
    CC_SYNTHESIZE(bool, _isExternalizable, IsExternalizable);
    CC_SYNTHESIZE(unsigned int, _count, Count);
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, _properties, Properties);

};


#endif //__AmfTraitsInfo_H_

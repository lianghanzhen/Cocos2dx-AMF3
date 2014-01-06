//
//  Created by lianghanzhen on 12/29/13.
//

#ifndef __DewinPoker__AmfCoding__
#define __DewinPoker__AmfCoding__

#include "CCCoding.h"
#include "RTTI/CCDynamicObject.h"

class AmfCoding : public CCCoding, public CCDynamicObject {

    DECLARE_DYNAMIC(AmfCoding)

public:
    virtual const char * getClassName() { return ""; }
    virtual bool isEquals(cocos2d::CCObject *object);

};


#endif /* defined(__DewinPoker__AmfCoding__) */

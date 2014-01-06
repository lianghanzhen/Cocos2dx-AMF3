//
//  Created by lianghanzhen on 12/29/13.
//

#ifndef __DewinPoker__CCCoding__
#define __DewinPoker__CCCoding__

#include "cocos2d.h"
#include "CCEncoder.h"
#include "CCDecoder.h"

class CCCoding {

public:
    virtual cocos2d::CCObject * initWithDecoder(CCDecoder *decoder) { return NULL; }
    virtual void encodeWithEncoder(CCEncoder *encoder) {}
    
};

#endif /* defined(__DewinPoker__CCCoding__) */

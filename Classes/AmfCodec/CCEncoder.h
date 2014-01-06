//
//  Created by lianghanzhen on 12/29/13.
//

#ifndef __DewinPoker__CCCoder__
#define __DewinPoker__CCCoder__

#include "cocos2d.h"
#include "Date/CCDate.h"

class CCEncoder {
  
public:
    virtual void encodeBoolForKey(bool value, const char *key) {}
    virtual void encodeIntForKey(int value, const char *key) {}
    virtual void encodeFloatForKey(float value, const char *key) {}
    virtual void encodeDoubleForKey(double value, const char *key) {}
    virtual void encodeStringForKey(cocos2d::CCString *value, const char *key) {}
    virtual void encodeDateForKey(CCDate *value, const char *key) {}
    virtual void encodeArrayForKey(cocos2d::CCArray *value, const char *key) {}
    virtual void encodeObjectForKey(cocos2d::CCObject *value, const char *key) {}
    
};

#endif /* defined(__DewinPoker__CCCoder__) */

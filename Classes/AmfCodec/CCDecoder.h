//
//  Created by lianghanzhen on 12/29/13.
//


#ifndef __CCDecoder_H_
#define __CCDecoder_H_

#include "cocos2d.h"
#include "Date/CCDate.h"

class CCDecoder {

public:
    virtual bool decodeBoolForKey(const char *key) { return false; }
    virtual int decodeIntForKey(const char *key) { return 0; }
    virtual float decodeFloatForKey(const char *key) { return 0.0f; }
    virtual double decodeDoubleForKey(const char *key) { return 0.0; }
    virtual cocos2d::CCString * decodeStringForKey(const char *key) { return NULL; }
    virtual CCDate * decodeDateForKey(const char *key) { return NULL; }
    virtual cocos2d::CCArray * decodeArrayForKey(const char *key) { return NULL; }
    virtual cocos2d::CCObject * decodeObjectForKey(const char *key) { return NULL; }

};


#endif //__CCDecoder_H_

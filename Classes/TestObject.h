//
// Created by lianghanzhen on 1/1/14.
//


#ifndef __TestObject_H_
#define __TestObject_H_

#include "AmfCodec/AmfCoding.h"

class TestObject : public AmfCoding {

    DECLARE_DYNAMIC_CREATE(TestObject)

public:
    TestObject() : BoolValue(false), IntValue(0), DoubleValue(0.0), StringValue(NULL), ArrayValue(NULL) {}
    ~TestObject() {
        CC_SAFE_RELEASE_NULL(StringValue);
        CC_SAFE_RELEASE_NULL(ArrayValue);
    }

    virtual const char * getClassName();
    virtual cocos2d::CCObject * initWithDecoder(CCDecoder *decoder);
    virtual void encodeWithEncoder(CCEncoder *encoder);

    bool isEquals(cocos2d::CCObject *object);

    CC_SYNTHESIZE(bool, BoolValue, BoolValue);
    CC_SYNTHESIZE(int, IntValue, IntValue);
    CC_SYNTHESIZE(double, DoubleValue, DoubleValue);
    CC_SYNTHESIZE_RETAIN(cocos2d::CCString *, StringValue, StringValue);
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, ArrayValue, ArrayValue);

};


#endif //__TestObject_H_

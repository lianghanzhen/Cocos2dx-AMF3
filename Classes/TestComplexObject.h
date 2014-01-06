//
// Created by lianghanzhen on 1/1/14.
//


#ifndef __TestComplexObject_H_
#define __TestComplexObject_H_


#include "AmfCodec/AmfCoding.h"
#include "TestObject.h"

class TestComplexObject : public AmfCoding {

    DECLARE_DYNAMIC_CREATE(TestComplexObject)

public:
    TestComplexObject() : BoolValue(false), IntValue(0), DoubleValue(0.0), StringValue(NULL), ArrayValue(NULL), Inner(NULL) {}
    ~TestComplexObject() {
        CC_SAFE_RELEASE_NULL(StringValue);
        CC_SAFE_RELEASE_NULL(ArrayValue);
        CC_SAFE_RELEASE_NULL(Inner);
    }

    bool isEquals(cocos2d::CCObject *object);

    virtual const char * getClassName();
    virtual cocos2d::CCObject * initWithDecoder(CCDecoder *decoder);
    virtual void encodeWithEncoder(CCEncoder *encoder);

    CC_SYNTHESIZE(bool, BoolValue, BoolValue);
    CC_SYNTHESIZE(int, IntValue, IntValue);
    CC_SYNTHESIZE(double, DoubleValue, DoubleValue);
    CC_SYNTHESIZE_RETAIN(cocos2d::CCString *, StringValue, StringValue);
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, ArrayValue, ArrayValue);
    CC_SYNTHESIZE_RETAIN(TestObject *, Inner, Inner);

};


#endif //__TestComplexObject_H_

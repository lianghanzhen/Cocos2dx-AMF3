//
// Created by lianghanzhen on 1/1/14.
//

#include "TestComplexObject.h"
#include "AmfCodec/AmfUtil.h"

USING_NS_CC;

IMPLEMENT_DYNAMIC_CREATE(TestComplexObject, AmfCoding)

const char * TestComplexObject::getClassName() {
    return "dmz.TestComplexObject";
}

CCObject * TestComplexObject::initWithDecoder(CCDecoder *decoder) {
    Inner = dynamic_cast<TestObject *>(decoder->decodeObjectForKey("Inner"));
    BoolValue = decoder->decodeBoolForKey("BoolValue");
    IntValue = decoder->decodeIntForKey("IntValue");
    DoubleValue = decoder->decodeDoubleForKey("DoubleValue");
    StringValue = decoder->decodeStringForKey("StringValue");
    ArrayValue = decoder->decodeArrayForKey("ArrayValue");
    return this;
}

void TestComplexObject::encodeWithEncoder(CCEncoder *encoder) {
    encoder->encodeObjectForKey(Inner, "Inner");
    encoder->encodeBoolForKey(BoolValue, "BoolValue");
    encoder->encodeIntForKey(IntValue, "IntValue");
    encoder->encodeDoubleForKey(DoubleValue, "DoubleValue");
    encoder->encodeStringForKey(StringValue, "StringValue");
    encoder->encodeArrayForKey(ArrayValue, "ArrayValue");
}

bool TestComplexObject::isEquals(cocos2d::CCObject *object) {
    if (!object) return false;
    TestComplexObject *other = dynamic_cast<TestComplexObject *>(object);
    if (!other) return false;
    return BoolValue == other->getBoolValue() && IntValue == other->getIntValue()
            && DoubleValue == other->getDoubleValue() && AmfUtil::isSame(StringValue, other->getStringValue())
            && AmfUtil::isSame(ArrayValue, other->getArrayValue()) && AmfUtil::isSame(Inner, other->getInner());
}
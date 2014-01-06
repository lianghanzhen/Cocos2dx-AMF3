//
// Created by lianghanzhen on 1/1/14.
//

#include "TestObject.h"
#include "AmfCodec/AmfUtil.h"

USING_NS_CC;

IMPLEMENT_DYNAMIC_CREATE(TestObject, AmfCoding)

const char * TestObject::getClassName() {
    return "dmz.TestObject";
}

CCObject * TestObject::initWithDecoder(CCDecoder *decoder) {
    BoolValue = decoder->decodeBoolForKey("BoolValue");
    IntValue = decoder->decodeIntForKey("IntValue");
    DoubleValue = decoder->decodeDoubleForKey("DoubleValue");
    StringValue = decoder->decodeStringForKey("StringValue");
    ArrayValue = decoder->decodeArrayForKey("ArrayValue");
    return this;
}

void TestObject::encodeWithEncoder(CCEncoder *encoder) {
    encoder->encodeBoolForKey(BoolValue, "BoolValue");
    encoder->encodeIntForKey(IntValue, "IntValue");
    encoder->encodeDoubleForKey(DoubleValue, "DoubleValue");
    encoder->encodeStringForKey(StringValue, "StringValue");
    encoder->encodeArrayForKey(ArrayValue, "ArrayValue");
}

bool TestObject::isEquals(cocos2d::CCObject *object) {
    if (!object) return false;
    TestObject *other = dynamic_cast<TestObject *>(object);
    if (!other) return false;
    return BoolValue == other->getBoolValue() && IntValue == other->getIntValue()
        && DoubleValue == other->getDoubleValue() && AmfUtil::isSame(StringValue, other->getStringValue())
        && AmfUtil::isSame(ArrayValue, other->getArrayValue());
}

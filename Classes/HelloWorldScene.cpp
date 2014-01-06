#include "HelloWorldScene.h"
#include "AmfCodec/AmfEncoder.h"
#include "AmfDecoder.h"
#include "AmfCodec/AmfUtil.h"
#include "TestObject.h"
#include "TestComplexObject.h"
#include "CCNull.h"

USING_NS_CC;

typedef struct FileData {
    unsigned char *content;
    unsigned long length;
} FileData;

static std::string dump(unsigned char *binaries, unsigned long len) {
    char const hex_chars[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    std::string string;
    for(unsigned int i = 0; i < len; i++)
    {
        const char byte = binaries[i];
        string += hex_chars[(byte & 0xF0) >> 4];
        string += hex_chars[byte & 0x0F];
    }
    return string;
}

static FileData * getFileDataByName(const char *filename) {
    FileData *data = new FileData();
    data->content = CCFileUtils::sharedFileUtils()->getFileData(filename, "rb", &data->length);
    return data;
}

static AmfDecoder * getDecoder(const char *title, const char *filename) {
    FileData *data = getFileDataByName(filename);
    CCLOG("Decode %s : %s, Length: %d", title, dump(data->content, data->length).c_str(), data->length);
    return AmfDecoder::createWithData(data->content, data->length);
}

static bool compareBinary(unsigned char *ch1, unsigned char *ch2, unsigned int length) {
    for (int i = 0; i < length; i++) {
        if (*ch1++ != *ch2++) {
            return false;
        }
    }
    return true;
}

static void testEncoder(const char *title, CCObject *value, const char *filename) {
    AmfEncoder *encoder = AmfEncoder::create();
    encoder->encodeObject(value);
    CCLOG("Encode %s : %s, Length: %d", title, dump((unsigned char *) encoder->getData(), encoder->getDataLength()).c_str(), encoder->getDataLength());
    FileData *data = getFileDataByName(filename);
    CCAssert(encoder->getDataLength() == data->length, "Data length is not equal");
    CCAssert(compareBinary((unsigned char *) encoder->getData(), data->content, data->length), "Data content is not equal");
}

static void testDecodeUndefined() {
    CCAssert(getDecoder("Undefined", "amf3-undefined.bin")->decodeObject() == NULL, "decode amf3-undefined.bin failed.");
}

static void testDecodeNull() {
    CCAssert(getDecoder("Null", "amf3-null.bin")->decodeObject() == NULL, "decode amf3-null.bin failed.");
}

static void testDecodeFalse() {
    CCAssert(!getDecoder("False", "amf3-false.bin")->decodeBool(), "decode amf3-false.bin failed.");
}

static void testDecodeTrue() {
    CCAssert(getDecoder("True", "amf3-true.bin")->decodeBool(), "decode amf3-true.bin failed.");
}

static void testDecodeInteger0() {
    CCAssert(getDecoder("Integer-0", "amf3-0.bin")->decodeInt() == 0, "decode amf3-0.bin failed.");
}

static void testDecodeIntegerMin() {
    CCAssert(getDecoder("Integer-min", "amf3-min.bin")->decodeInt() == -0x10000000, "decode amf3-min.bin failed.");
}

static void testDecodeIntegerMax() {
    CCAssert(getDecoder("Integer-max", "amf3-max.bin")->decodeInt() == 0xFFFFFFF, "decode amf3-max.bin failed.");
}

static void testDecodeDouble() {
    CCAssert(getDecoder("Double", "amf3-float.bin")->decodeDouble() == 3.5, "decode amf3-float.bin failed.");
}

static void testDecodeBigDouble() {
    CCAssert(getDecoder("Double-bignum", "amf3-bignum.bin")->decodeDouble() == pow(2, 1000), "decode amf3-bignum.bin failed.");
}

static void testDecodeLargeMinDouble() {
    CCAssert(getDecoder("Double-large-min", "amf3-large-min.bin")->decodeDouble() == -0x10000000 - 1.0, "decode amf3-large-min.bin failed.");
}

static void testDecodeLargeMaxDouble() {
    CCAssert(getDecoder("Double-large-max", "amf3-large-max.bin")->decodeDouble() == 0xFFFFFFF + 1.0, "decode amf3-large-max.bin failed.");
}

static void testDecodeString() {
    CCAssert(AmfUtil::isSame(getDecoder("String", "amf3-string.bin")->decodeString(), CCStringMake("String . String")), "decode amf3-string.bin failed.");\
}

static void testDecodeStringRef() {
    CCArray *expected = CCArray::createWithCapacity(6);
    CCString *foo = CCStringMake("foo");
    CCString *str = CCStringMake("str");
    expected->addObject(foo);
    expected->addObject(str);
    expected->addObject(foo);
    expected->addObject(str);
    expected->addObject(foo);
    expected->addObject(str);
    CCAssert(AmfUtil::isSame(getDecoder("String-ref", "amf3-string-ref.bin")->decodeObject(), expected), "decode amf3-string-ref.bin failed.");
}

static void testDecodeEncodedStringRef() {
    CCArray *expected = CCArray::createWithCapacity(2);
    CCString *string = CCStringMake("AMF3协议编码");
    expected->addObject(string);
    expected->addObject(string);
    CCAssert(AmfUtil::isSame(getDecoder("String-encoded", "amf3-encoded-string-ref.bin")->decodeObject(), expected), "decode amf3-encoded-string-ref.bin failed.");
}

static void testDecodeComplexEncodedStringArray() {
    CCArray *expected = CCArray::createWithCapacity(4);
    expected->addObject(CCInteger::create(5));
    expected->addObject(CCStringMake("AMF3协议编码"));
    expected->addObject(CCStringMake("AMF3协议编码"));
    expected->addObject(CCInteger::create(5));
    CCAssert(AmfUtil::isSame(getDecoder("String-encoded-complex", "amf3-complex-encoded-string-array.bin")->decodeObject(), expected), "amf3-complex-encoded-string-array.bin failed.");
}

static void testDecodeEmptyStringRef() {
    CCArray *expected = CCArray::createWithCapacity(2);
    CCString *string = CCStringMake("");
    expected->addObject(string);
    expected->addObject(string);
    CCAssert(AmfUtil::isSame(getDecoder("String-empty-ref", "amf3-empty-string-ref.bin")->decodeObject(), expected), "decode amf3-empty-string-ref.bin failed.");
}

static void testDecodePrimitiveArray() {
    CCArray *expected = CCArray::createWithCapacity(5);
    expected->addObject(CCInteger::create(1));
    expected->addObject(CCInteger::create(2));
    expected->addObject(CCInteger::create(3));
    expected->addObject(CCInteger::create(4));
    expected->addObject(CCInteger::create(5));
    CCAssert(AmfUtil::isSame(getDecoder("Array-primitive", "amf3-primitive-array.bin")->decodeObject(), expected), "decode amf3-primitive-array.bin failed.");
}

static void testDecodeEmptyArray() {
    CCAssert(AmfUtil::isSame(getDecoder("Array-empty", "amf3-empty-array.bin")->decodeObject(), CCArray::createWithCapacity(0)), "decode amf3-empty-array.bin failed.");
}

static void testDecodeEmptyArrayRef() {
    CCArray *expected = CCArray::createWithCapacity(4);
    CCArray *emptyArray = CCArray::createWithCapacity(0);
    expected->addObject(emptyArray);
    expected->addObject(emptyArray);
    expected->addObject(emptyArray);
    expected->addObject(emptyArray);
    CCAssert(AmfUtil::isSame(getDecoder("Array-empty-ref", "amf3-empty-array-ref.bin")->decodeObject(), expected), "decode amf3-empty-array-ref.bin failed.");
}

static void testDecodeArrayRef() {
    CCArray *expected = CCArray::createWithCapacity(4);
    CCArray *primitiveArray = CCArray::createWithCapacity(3);
    primitiveArray->addObject(CCInteger::create(1));
    primitiveArray->addObject(CCInteger::create(2));
    primitiveArray->addObject(CCInteger::create(3));
    CCArray *stringArray = CCArray::createWithCapacity(3);
    stringArray->addObject(CCStringMake("a"));
    stringArray->addObject(CCStringMake("b"));
    stringArray->addObject(CCStringMake("c"));
    expected->addObject(primitiveArray);
    expected->addObject(stringArray);
    expected->addObject(primitiveArray);
    expected->addObject(stringArray);
    CCAssert(AmfUtil::isSame(getDecoder("Array-ref", "amf3-array-ref.bin")->decodeObject(), expected), "decode amf3-array-ref.bin failed.");
}

static TestObject * createTestObject() {
    TestObject *testObject = new TestObject();
    testObject->setBoolValue(true);
    testObject->setIntValue(1);
    testObject->setDoubleValue(1.1);
    testObject->setStringValue(CCStringMake("AMF3协议编码"));
    CCArray *stringArray = CCArray::createWithCapacity(3);
    stringArray->addObject(CCStringMake("a"));
    stringArray->addObject(CCStringMake("b"));
    stringArray->addObject(CCStringMake("c"));
    testObject->setArrayValue(stringArray);
    return testObject;
}

static TestObject * createOtherTestObject() {
    TestObject *testObject = new TestObject();
    testObject->setBoolValue(false);
    testObject->setIntValue(2);
    testObject->setDoubleValue(2.2);
    testObject->setStringValue(CCStringMake("AMF3协议编码AMF3协议编码"));
    CCArray *stringArray = CCArray::createWithCapacity(3);
    stringArray->addObject(CCStringMake("a"));
    stringArray->addObject(CCStringMake("b"));
    stringArray->addObject(CCStringMake("c"));
    testObject->setArrayValue(stringArray);
    return testObject;
}

static TestComplexObject * createTestComplexObject() {
    TestComplexObject *testComplexObject = new TestComplexObject();
    testComplexObject->setBoolValue(false);
    testComplexObject->setIntValue(2);
    testComplexObject->setDoubleValue(2.2);
    testComplexObject->setStringValue(CCStringMake("AMF3协议编码AMF3协议编码"));
    CCArray *stringArray = CCArray::createWithCapacity(3);
    stringArray->addObject(CCStringMake("a"));
    stringArray->addObject(CCStringMake("b"));
    stringArray->addObject(CCStringMake("c"));
    testComplexObject->setArrayValue(stringArray);
    testComplexObject->setInner(createTestObject());
    return testComplexObject;
}

static void testDecodeMixArray() {
    CCArray *array = CCArray::createWithCapacity(7);
    array->addObject(CCInteger::create(1));
    array->addObject(CCDouble::create(1.1));
    array->addObject(CCStringMake("AMF3协议编码"));
    CCArray *primitiveArray = CCArray::createWithCapacity(3);
    primitiveArray->addObject(CCInteger::create(1));
    primitiveArray->addObject(CCInteger::create(2));
    primitiveArray->addObject(CCInteger::create(3));
    array->addObject(primitiveArray);
    CCArray *stringArray = CCArray::createWithCapacity(3);
    stringArray->addObject(CCStringMake("a"));
    stringArray->addObject(CCStringMake("b"));
    stringArray->addObject(CCStringMake("c"));
    array->addObject(stringArray);
    array->addObject(createTestObject());
    array->addObject(createTestComplexObject());
    CCAssert(AmfUtil::isSame(array, getDecoder("Array-mixed", "amf3-mixed-array.bin")->decodeObject()), "decode amf3-mixed-array.bin failed.");
}

static void testDecodeTypeObject() {
    CCAssert(AmfUtil::isSame(createTestObject(), getDecoder("Object-typed", "amf3-typed-object.bin")->decodeObject()), "decode amf3-typed-object.bin failed.");
}

static void testDecodeComplexTypeObject() {
    CCAssert(AmfUtil::isSame(createTestComplexObject(), getDecoder("Object-typed-complex", "amf3-typed-complex-object.bin")->decodeObject()), "decode amf3-typed-complex-object.bin failed.");
}

static void testDecodeTraitsRef() {
    CCArray *array = CCArray::createWithCapacity(2);
    array->addObject(createTestObject());
    array->addObject(createOtherTestObject());
    CCAssert(AmfUtil::isSame(array, getDecoder("Object-traits-ref", "amf3-trait-ref.bin")->decodeObject()), "decode amf3-trait-ref.bin failed.");
}

static void testDecodeObjectRef() {
    CCArray *array = CCArray::createWithCapacity(4);
    array->addObject(createTestObject());
    array->addObject(createTestComplexObject());
    array->addObject(createTestObject());
    array->addObject(createTestComplexObject());
    CCAssert(AmfUtil::isSame(array, getDecoder("Object-ref", "amf3-object-ref.bin")->decodeObject()), "decode amf3-object-ref.bin failed.");
}

static CCDate * createDate() {
    CCDate *date = CCDate::create();
    date->setTime(1388625284.497251);
    return date;
}

static void testDecodeDate() {
    CCAssert(AmfUtil::isSame(createDate(), getDecoder("Date", "amf3-date.bin")->decodeObject()), "decode amf3-date.bin failed.");
}

static void testDecodeDateRef() {
    CCArray *array = CCArray::createWithCapacity(2);
    array->addObject(createDate());
    array->addObject(createDate());
    CCAssert(AmfUtil::isSame(array, getDecoder("Date-ref", "amf3-date-ref.bin")->decodeObject()), "decode amf3-date-ref.bin failed");
}

static void testEncodeStringRef() {
    CCArray *data = CCArray::createWithCapacity(6);
    CCString *foo = CCStringMake("foo");
    CCString *str = CCStringMake("str");
    data->addObject(foo);
    data->addObject(str);
    data->addObject(foo);
    data->addObject(str);
    data->addObject(foo);
    data->addObject(str);
    testEncoder("String-ref", data, "amf3-string-ref.bin");
}

static void testEncodeEncodedStringRef() {
    CCArray *data = CCArray::createWithCapacity(2);
    CCString *string = CCStringMake("AMF3协议编码");
    data->addObject(string);
    data->addObject(string);
    testEncoder("String-encoded", data, "amf3-encoded-string-ref.bin");
}

static void testEncodeComplexEncodedStringArray() {
    CCArray *data = CCArray::createWithCapacity(4);
    data->addObject(CCInteger::create(5));
    data->addObject(CCStringMake("AMF3协议编码"));
    data->addObject(CCStringMake("AMF3协议编码"));
    data->addObject(CCInteger::create(5));
    testEncoder("String-encoded-complex", data, "amf3-complex-encoded-string-array.bin");
}

static void testEncodeEmptyStringRef() {
    CCArray *data = CCArray::createWithCapacity(2);
    CCString *string = CCStringMake("");
    data->addObject(string);
    data->addObject(string);
    testEncoder("String-empty-ref", data, "amf3-empty-string-ref.bin");
}

static void testEncodePrimitiveArray() {
    CCArray *data = CCArray::createWithCapacity(5);
    data->addObject(CCInteger::create(1));
    data->addObject(CCInteger::create(2));
    data->addObject(CCInteger::create(3));
    data->addObject(CCInteger::create(4));
    data->addObject(CCInteger::create(5));
    testEncoder("Array-primitive", data, "amf3-primitive-array.bin");
}

static void testEncodeEmptyArrayRef() {
    CCArray *data = CCArray::createWithCapacity(4);
    CCArray *emptyArray = CCArray::createWithCapacity(0);
    data->addObject(emptyArray);
    data->addObject(emptyArray);
    data->addObject(emptyArray);
    data->addObject(emptyArray);
    testEncoder("Array-empty-ref", data, "amf3-empty-array-ref.bin");
}

static void testEncodeArrayRef() {
    CCArray *data = CCArray::createWithCapacity(4);
    CCArray *primitiveArray = CCArray::createWithCapacity(3);
    primitiveArray->addObject(CCInteger::create(1));
    primitiveArray->addObject(CCInteger::create(2));
    primitiveArray->addObject(CCInteger::create(3));
    CCArray *stringArray = CCArray::createWithCapacity(3);
    stringArray->addObject(CCStringMake("a"));
    stringArray->addObject(CCStringMake("b"));
    stringArray->addObject(CCStringMake("c"));
    data->addObject(primitiveArray);
    data->addObject(stringArray);
    data->addObject(primitiveArray);
    data->addObject(stringArray);
    testEncoder("Array-ref", data, "amf3-array-ref.bin");
}

static void testEncodeMixArray() {
    CCArray *data = CCArray::createWithCapacity(7);
    data->addObject(CCInteger::create(1));
    data->addObject(CCDouble::create(1.1));
    data->addObject(CCStringMake("AMF3协议编码"));
    CCArray *primitiveArray = CCArray::createWithCapacity(3);
    primitiveArray->addObject(CCInteger::create(1));
    primitiveArray->addObject(CCInteger::create(2));
    primitiveArray->addObject(CCInteger::create(3));
    data->addObject(primitiveArray);
    CCArray *stringArray = CCArray::createWithCapacity(3);
    stringArray->addObject(CCStringMake("a"));
    stringArray->addObject(CCStringMake("b"));
    stringArray->addObject(CCStringMake("c"));
    data->addObject(stringArray);
    data->addObject(createTestObject());
    data->addObject(createTestComplexObject());
    testEncoder("Array-mixed", data, "amf3-mixed-array.bin");
}

static void testEncodeTraitsRef() {
    CCArray *data = CCArray::createWithCapacity(2);
    data->addObject(createTestObject());
    data->addObject(createOtherTestObject());
    testEncoder("Object-traits-ref", data, "amf3-trait-ref.bin");
}

static void testEncodeObjectRef() {
    CCArray *data = CCArray::createWithCapacity(4);
    data->addObject(createTestObject());
    data->addObject(createTestComplexObject());
    data->addObject(createTestObject());
    data->addObject(createTestComplexObject());
    testEncoder("Object-ref", data, "amf3-object-ref.bin");
}

static void testEncodeDateRef() {
    CCArray *data = CCArray::createWithCapacity(2);
    data->addObject(createDate());
    data->addObject(createDate());
    testEncoder("Date-ref", data, "amf3-date-ref.bin");
}

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);

    // test decode
    testDecodeUndefined();
    testDecodeNull();
    testDecodeFalse();
    testDecodeTrue();
    testDecodeInteger0();
    testDecodeIntegerMin();
    testDecodeIntegerMax();
    testDecodeDouble();
    testDecodeBigDouble();
    testDecodeLargeMinDouble();
    testDecodeLargeMaxDouble();
    testDecodeString();
    testDecodeStringRef();
    testDecodeEncodedStringRef();
    testDecodeComplexEncodedStringArray();
    testDecodeEmptyStringRef();
    testDecodePrimitiveArray();
    testDecodeEmptyArray();
    testDecodeEmptyArrayRef();
    testDecodeArrayRef();
    testDecodeMixArray();
    testDecodeTypeObject();
    testDecodeComplexTypeObject();
    testDecodeTraitsRef();
    testDecodeObjectRef();
    testDecodeDate();
    testDecodeDateRef();

    CCLOG("");
    CCLOG("########################################");
    CCLOG("########################################");
    CCLOG("########################################");
    CCLOG("");

    // test encode
    testEncoder("Null", CCNull::create(), "amf3-null.bin");
    testEncoder("False", CCBool::create(false), "amf3-false.bin");
    testEncoder("True", CCBool::create(true), "amf3-true.bin");
    testEncoder("Integer-0", CCInteger::create(0), "amf3-0.bin");
    testEncoder("Integer-min", CCInteger::create(-0x10000000), "amf3-min.bin");
    testEncoder("Integer-max", CCInteger::create(0xFFFFFFF), "amf3-max.bin");
    testEncoder("Double", CCDouble::create(3.5), "amf3-float.bin");
    testEncoder("Double-bignum", CCDouble::create(pow(2, 1000)), "amf3-bignum.bin");
    testEncoder("Double-large-min", CCDouble::create(-0x10000000 - 1.0), "amf3-large-min.bin");
    testEncoder("Double-large-max", CCDouble::create(0xFFFFFFF + 1.0), "amf3-large-max.bin");
    testEncoder("String", CCStringMake("String . String"), "amf3-string.bin");
    testEncodeStringRef();
    testEncodeEncodedStringRef();
    testEncodeComplexEncodedStringArray();
    testEncodeEmptyStringRef();
    testEncodePrimitiveArray();
    testEncoder("Array-empty", CCArray::create(), "amf3-empty-array.bin");
    testEncodeEmptyArrayRef();
    testEncodeArrayRef();
    testEncodeMixArray();
    testEncoder("Object-typed", createTestObject(), "amf3-typed-object.bin");
    testEncoder("Object-typed-complex", createTestComplexObject(), "amf3-typed-complex-object.bin");
    testEncodeTraitsRef();
    testEncodeObjectRef();
    testEncoder("Date", createDate(), "amf3-date.bin");
    testEncodeDateRef();

    return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

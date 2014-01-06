Cocos2dx-AMF3
=============

Encode and decode Adobe AMF3 protocol data for Cocos2dx


Support AMF3 data type
======================

1. Undefined
2. Null
3. False
4. True
5. Integer
6. Double
7. String
8. Date (precented by CCDate)
9. Array (not supported mixed array)
10. Object (custom class must extern AmfCoding, not supported dynamic object and externalize)


What should you do to work with AMF3 in custom class?
=====================================

1. Create a custom class that externs AmfCoding
2. implements the following methods: getClassName(), isEquals(cocos2d::CCObject *), initWithDecoder(CCDecoder *), encodeWithEncoder(CCEncoder *)

If you don't work out, please check TestObject and TestComplexObject in Classes directory.

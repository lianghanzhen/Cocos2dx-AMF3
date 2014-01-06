//
//  Created by lianghanzhen on 12/29/13.
//


#ifndef __AmfEncoder_H_
#define __AmfEncoder_H_

#include "CCEncoder.h"
#include "AmfBuffer.h"
#include "AmfObject.h"
#include "AmfTraitsInfo.h"
#include "AmfCoding.h"
#include "AmfRefCache.h"

class AmfEncoder : public CCEncoder {

public:
    ~AmfEncoder();
    static AmfEncoder * create();

    void reset();
    char * getData();
    unsigned int getDataLength();

    virtual void encodeBoolForKey(bool value, const char *key);
    virtual void encodeIntForKey(int value, const char *key);
    virtual void encodeFloatForKey(float value, const char *key);
    virtual void encodeDoubleForKey(double value, const char *key);
    virtual void encodeStringForKey(cocos2d::CCString *value, const char *key);
    virtual void encodeDateForKey(CCDate *value, const char *key);
    virtual void encodeArrayForKey(cocos2d::CCArray *value, const char *key);
    virtual void encodeObjectForKey(cocos2d::CCObject *value, const char *key);

    virtual void encodeBool(bool value);
    virtual void encodeInt(int value);
    virtual void encodeFloat(float value);
    virtual void encodeDouble(double value);
    virtual void encodeDate(CCDate *value);
    virtual void encodeString(cocos2d::CCString *value);
    virtual void encodeArray(cocos2d::CCArray *value);
    virtual void encodeObject(cocos2d::CCObject *value);

private:
    AmfEncoder();

    void _encodeString(cocos2d::CCString *value, bool encodeType);
    void _encodeAmfObject(AmfObject *value);
    void _encodeTraits(AmfTraitsInfo *amfTraitsInfo);
    void _encodeCustomObject(AmfCoding *customObject);
    void _setSerializedValueForKey(cocos2d::CCObject *value, const char *key);
    bool _addSerializedObject(cocos2d::CCObject *object);
    void _ensureIntegrityOfSerializedObject();
    bool _setStringReference(cocos2d::CCString *string);
    bool _setObjectReference(cocos2d::CCObject *object);
    bool _setTraitsReference(AmfTraitsInfo *amfTraitsInfo);

    AmfBuffer *_buffer;
    AmfObject *_objectToSerialized;
    cocos2d::CCArray *_serializedObjectStack;
    AmfRefCache *_refCache;

};


#endif //__AmfEncoder_H_

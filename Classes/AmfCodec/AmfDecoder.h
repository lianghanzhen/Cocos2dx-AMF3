//
//  Created by lianghanzhen on 12/30/13.
//


#ifndef __AmfDecoder_H_
#define __AmfDecoder_H_

#include "CCDecoder.h"
#include "Amf.h"
#include "AmfCursor.h"
#include "AmfRefCache.h"
#include "AmfObject.h"

class AmfDecoder : public CCDecoder {

public:
    ~AmfDecoder();
    static AmfDecoder *createWithData(unsigned char *data, size_t length);

    virtual bool decodeBoolForKey(const char *key);
    virtual int decodeIntForKey(const char *key);
    virtual float decodeFloatForKey(const char *key);
    virtual double decodeDoubleForKey(const char *key);
    virtual cocos2d::CCString * decodeStringForKey(const char *key);
    virtual CCDate * decodeDateForKey(const char *key);
    virtual cocos2d::CCArray * decodeArrayForKey(const char *key);
    virtual cocos2d::CCObject * decodeObjectForKey(const char *key);

    bool decodeBool();
    int decodeInt();
    float decodeFloat();
    double decodeDouble();
    cocos2d::CCString * decodeString();
    CCDate * decodeDate();
    cocos2d::CCArray * decodeArray();
    cocos2d::CCObject * decodeObject();

private:
    AmfDecoder(unsigned char *buffer, size_t length);

    int _decodeInt();
    float _decodeFloat();
    double _decodeDouble();
    cocos2d::CCString * _decodeString();
    CCDate * _decodeDate();
    cocos2d::CCArray * _decodeArray();
    cocos2d::CCObject * _decodeAmfObject();
    AmfTraitsInfo * _decodeTraits(int ref);
    cocos2d::CCObject * _decodeObject(AmfObject *object);

    bool _isReference(unsigned int ref);
    AMF3Type _getType();
    void _checkType(AMF3Type type, const char *message);
    void _setStringReference(cocos2d::CCString *string);
    int _setObjectReference(cocos2d::CCObject *object);
    void _replaceObjectReference(cocos2d::CCObject *oldObject, cocos2d::CCObject *newObject);
    void _setTraitsReference(AmfTraitsInfo *traits);

    AmfCursor *_cursor;
    AmfObject *_objectToDeserialized;
    AmfRefCache *_refCache;

};


#endif //__AmfDecoder_H_

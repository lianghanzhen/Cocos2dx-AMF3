//
//  Created by lianghanzhen on 12/29/13.
//

#include "AmfUtil.h"
#include "AmfTraitsInfo.h"
#include "AmfObject.h"
#include "Date/CCDate.h"
#include "AmfCoding.h"

USING_NS_CC;

int AmfUtil::getItemIndex(cocos2d::CCArray *array, cocos2d::CCObject *object) {
    int itemIndex = INVALID_INDEX;
    int count = array->count();
    for (int i = 0; i < count; i++) {
        if (isSame(object, array->objectAtIndex(i))) {
            itemIndex = i;
            break;
        }
    }
    return itemIndex;
}

bool AmfUtil::isSame(cocos2d::CCObject *thisObj, cocos2d::CCObject *thatObj) {
    if (thisObj && !thatObj) return false;
    if (!thisObj && thatObj) return false;
    if (!thisObj && !thatObj) return true;

    if (dynamic_cast<CCBool *>(thisObj) && dynamic_cast<CCBool *>(thatObj)) {
        return dynamic_cast<CCBool *>(thisObj)->getValue() == dynamic_cast<CCBool *>(thatObj)->getValue();
    } else if (dynamic_cast<CCInteger *>(thisObj) && dynamic_cast<CCInteger *>(thatObj)) {
        return dynamic_cast<CCInteger *>(thisObj)->getValue() == dynamic_cast<CCInteger *>(thatObj)->getValue();
    } else if (dynamic_cast<CCFloat *>(thisObj) && dynamic_cast<CCFloat *>(thatObj)) {
        return dynamic_cast<CCFloat *>(thisObj)->getValue() == dynamic_cast<CCFloat *>(thatObj)->getValue();
    } else if (dynamic_cast<CCDouble *>(thisObj) && dynamic_cast<CCDouble *>(thatObj)) {
        return dynamic_cast<CCDouble *>(thisObj)->getValue() == dynamic_cast<CCDouble *>(thatObj)->getValue();
    } else if (dynamic_cast<CCString *>(thisObj) && dynamic_cast<CCString *>(thatObj)) {
        return strcmp(dynamic_cast<CCString *>(thisObj)->getCString(), dynamic_cast<CCString *>(thatObj)->getCString()) == 0;
    } else if (dynamic_cast<CCDate *>(thisObj) && dynamic_cast<CCDate *>(thatObj)) {
        return dynamic_cast<CCDate *>(thisObj)->isEqual(dynamic_cast<CCDate *>(thatObj));
    } else if (dynamic_cast<AmfTraitsInfo *>(thisObj) && dynamic_cast<AmfTraitsInfo *>(thatObj)) {
        return dynamic_cast<AmfTraitsInfo *>(thisObj)->isEqual(dynamic_cast<AmfTraitsInfo *>(thatObj));
    } else if (dynamic_cast<AmfObject *>(thisObj) && dynamic_cast<AmfObject *>(thatObj)) {
        return dynamic_cast<AmfObject *>(thisObj)->isEqual(dynamic_cast<AmfObject *>(thatObj));
    } else if (dynamic_cast<AmfCoding *>(thisObj) && dynamic_cast<AmfCoding *>(thatObj)) {
        return dynamic_cast<AmfCoding *>(thisObj)->isEquals(thatObj);
    } else if (dynamic_cast<CCArray *>(thisObj) && dynamic_cast<CCArray *>(thatObj)) {
        CCArray *thisArray = dynamic_cast<CCArray *>(thisObj);
        CCArray *thatArray = dynamic_cast<CCArray *>(thatObj);
        int count = thisArray->count();
        int capacity = thisArray->capacity();
        if (count != thatArray->count() || (count == 0 && capacity != thatArray->capacity()))
            return false;
        for (int i = 0; i < count; i++) {
            if (!isSame(thisArray->objectAtIndex(i), thatArray->objectAtIndex(i)))
                return false;
        }
        return true;
    } else if (dynamic_cast<CCDictionary *>(thisObj) && dynamic_cast<CCDictionary *>(thatObj)) {
        CCDictionary *thisDict = dynamic_cast<CCDictionary *>(thisObj);
        CCDictionary *thatDict = dynamic_cast<CCDictionary *>(thatObj);
        int count = thisDict->count();
        if (count != thatDict->count())
            return false;
        CCDictElement *thisElement;
        CCDICT_FOREACH(thisDict, thisElement) {
                char const *thisKey = thisElement->getStrKey();
                CCObject *thatValue = thatDict->objectForKey(thisKey);
                if (!thatValue) return false;
                if (!isSame(thisElement->getObject(), thatValue)) return false;
            }
        return true;
    } else {
        return thisObj->isEqual(thatObj);
    }
}

//
//  Created by lianghanzhen on 12/29/13.
//


#ifndef __AmfUtil_H_
#define __AmfUtil_H_

#include "cocos2d.h"

#define INVALID_INDEX (-1)


class AmfUtil {

public:
    static int getItemIndex(cocos2d::CCArray *array, cocos2d::CCObject *object);
    static bool isSame(cocos2d::CCObject *thisObj, cocos2d::CCObject *thatObj);

};


#endif //__AmfUtil_H_

//
// Created by lianghanzhen on 14-1-2.
//


#ifndef __CCNull_H_
#define __CCNull_H_

#include "cocos2d.h"

/**
* for AmfEncoder, because null cannot be putted into CCArray or CCDictionary
*/
class CCNull : public cocos2d::CCObject {

public:
    ~CCNull() {}

    static CCNull *create();

private:
    CCNull() {}

};


#endif //__CCNull_H_

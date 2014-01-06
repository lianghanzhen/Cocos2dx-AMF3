//
//  Created by lianghanzhen on 12/30/13.
//


#ifndef __CCDate_H_
#define __CCDate_H_


#include "cocos2d.h"

typedef int DATE_Year;
typedef int DATE_Month;
typedef int DATE_Week;
typedef int DATE_Day;
typedef int DATE_Hour;
typedef int DATE_Minute;
typedef int DATE_Second;
typedef long DATE_Microsecond;
typedef double DATE_Time;
typedef int DATE_Timezone;

class CCDate : public cocos2d::CCObject {

public:
    virtual bool init();
    CREATE_FUNC(CCDate);

    static CCDate* createWithCurrentTime();
    static CCDate* createWithCC_TimevalWithLocaltime(cocos2d::cc_timeval timeval);
    static CCDate* createWithTimevalWithLocaltime(timeval timeval);
    virtual void initWithCurrentTime();

    void setCC_TimevalWithLocaltime(cocos2d::cc_timeval timeval);
    void setTimevalWithLocaltime(timeval timeval);

    DATE_Time getTime();
    DATE_Time setTime(DATE_Time time);

    DATE_Timezone getTimezoneOffset();
    DATE_Time setTimezoneOffset(DATE_Timezone timezone);

    DATE_Microsecond getMicroseconds();
    DATE_Time setMicroseconds(DATE_Microsecond microsecond);

    DATE_Second getSeconds();
    DATE_Time setSeconds(DATE_Second second);

    DATE_Minute getMinutes();
    DATE_Time setMinutes(DATE_Minute minute);

    DATE_Hour getHours();
    DATE_Time setHours(DATE_Hour hour);

    DATE_Day getDay();
    DATE_Time setDay(DATE_Day day);

    DATE_Week getWeek();
    DATE_Week getUTCWeek();

    DATE_Month getMonth();
    DATE_Time setMonth(DATE_Month month);

    DATE_Year getYear();
    DATE_Time setYear(DATE_Year year);

    DATE_Microsecond getUTCMicroseconds();
    DATE_Time setUTCMicroseconds(DATE_Microsecond microsecond);

    DATE_Second getUTCSeconds();
    DATE_Time setUTCSeconds(DATE_Second second);

    DATE_Minute getUTCMinutes();
    DATE_Time setUTCMinutes(DATE_Minute minute);

    DATE_Hour getUTCHours();
    DATE_Time setUTCHours(DATE_Hour hour);

    DATE_Day getUTCDay();
    DATE_Time setUTCDay(DATE_Day day);

    DATE_Month getUTCMonth();
    DATE_Time setUTCMonth(DATE_Month month);

    DATE_Year getUTCYear();
    DATE_Time setUTCYear(DATE_Year year);

    bool isEqual(cocos2d::CCObject *object);
    std::string description();

private:
    static int _sharedTimezoneOffset();

    void _syncFromLocalTime();
    void _syncFromLocalDate();
    void _syncFromUTCTime();
    void _syncFromUTCDate();

    timeval _localTime;
    timeval _UTCTime;
    tm _localDate;
    tm _UTCDate;

};


#endif //__CCDate_H_

//
//  Created by lianghanzhen on 12/30/13.
//

#include "CCDate.h"
#include <cmath>

USING_NS_CC;

int timezoneOffset = 2147483647;

int CCDate::_sharedTimezoneOffset() {
    if (timezoneOffset == 2147483647) {
        timeval tempTimeval;
        gettimeofday(&tempTimeval, 0);
        tm tempTM;
        tempTM = *localtime(&tempTimeval.tv_sec );
        timezoneOffset = tempTM.tm_gmtoff;
    }
    return timezoneOffset;
}

CCDate* CCDate::createWithCurrentTime() {
    CCDate *currentDate = CCDate::create();
    currentDate->initWithCurrentTime();
    return currentDate;
}

CCDate* CCDate::createWithCC_TimevalWithLocaltime(cc_timeval timeval) {
    CCDate *date = CCDate::create();
    date->setCC_TimevalWithLocaltime(timeval);
    return date;
}

CCDate* CCDate::createWithTimevalWithLocaltime(timeval timeval) {
    CCDate *date = CCDate::create();
    date->setTimevalWithLocaltime(timeval);
    return date;
}

bool CCDate::init() {
    return true;
}

void CCDate::initWithCurrentTime() {
    gettimeofday(&_localTime, 0);
    this->_syncFromLocalTime();
}

void CCDate::setCC_TimevalWithLocaltime(cc_timeval timeval) {
    _localTime.tv_sec = timeval.tv_sec;
    _localTime.tv_usec = timeval.tv_usec;
    this->_syncFromLocalTime();
}

void CCDate::setTimevalWithLocaltime(timeval timeval) {
    _localTime.tv_sec = timeval.tv_sec;
    _localTime.tv_usec = timeval.tv_usec;
    this->_syncFromLocalTime();
}

void CCDate::_syncFromLocalTime() {
    _localDate = *localtime(&_localTime.tv_sec );
    _UTCTime.tv_sec = _localTime.tv_sec - _sharedTimezoneOffset();
    _UTCDate = *localtime(&_UTCTime.tv_sec );
}

void CCDate::_syncFromLocalDate() {
    _localTime.tv_sec = mktime(&_localDate);
    _UTCTime.tv_sec = _localTime.tv_sec - _sharedTimezoneOffset();
    _UTCDate = *localtime(&_UTCTime.tv_sec );
}

void CCDate::_syncFromUTCTime() {
    _UTCDate = *localtime(&_UTCTime.tv_sec );
    _localTime.tv_sec = _UTCTime.tv_sec + _sharedTimezoneOffset();
    _localDate = *localtime(&_localTime.tv_sec );
}

void CCDate::_syncFromUTCDate() {
    _UTCTime.tv_sec = mktime(&_UTCDate);
    _localTime.tv_sec = _UTCTime.tv_sec + _sharedTimezoneOffset();
    _localDate = *localtime(&_localTime.tv_sec );
}

DATE_Time CCDate::getTime() {
    return _localTime.tv_sec * 1.0 + _localTime.tv_usec * 1.0 / 1000000;
}

DATE_Timezone CCDate::getTimezoneOffset() {
    return _sharedTimezoneOffset();
}

DATE_Time CCDate::setTime(DATE_Time time) {
    _localTime.tv_sec = (long) time;
    _localTime.tv_usec = (long) ((time - _localTime.tv_sec) * 1000000);
    this->_syncFromLocalTime();
    return _localTime.tv_sec;
}

DATE_Time CCDate::setTimezoneOffset(DATE_Timezone timezone) {
    timezoneOffset = timezone;
    this->_syncFromUTCTime();
    return _localTime.tv_sec;
}

DATE_Microsecond CCDate::getMicroseconds() {
    return _localTime.tv_usec;
}

DATE_Second CCDate::getSeconds() {
    return _localDate.tm_sec;
}

DATE_Minute CCDate::getMinutes() {
    return _localDate.tm_min;
}

DATE_Hour CCDate::getHours() {
    return _localDate.tm_hour;
}

DATE_Day CCDate::getDay() {
    return _localDate.tm_mday;
}

DATE_Week CCDate::getWeek() {
    return _localDate.tm_wday;
}

DATE_Month CCDate::getMonth() {
    return _localDate.tm_mon;
}

DATE_Year CCDate::getYear() {
    return _localDate.tm_year + 1900;
}

DATE_Microsecond CCDate::getUTCMicroseconds() {
    return _UTCTime.tv_usec;
}

DATE_Second CCDate::getUTCSeconds() {
    return _UTCDate.tm_sec;
}

DATE_Minute CCDate::getUTCMinutes() {
    return _UTCDate.tm_min;
}

DATE_Hour CCDate::getUTCHours() {
    return _UTCDate.tm_hour;
}

DATE_Day CCDate::getUTCDay() {
    return _UTCDate.tm_mday;
}

DATE_Week CCDate::getUTCWeek() {
    return _UTCDate.tm_wday;
}

DATE_Month CCDate::getUTCMonth() {
    return _UTCDate.tm_mon;
}

DATE_Year CCDate::getUTCYear() {
    return _UTCDate.tm_year + 1900;
}

DATE_Time CCDate::setMicroseconds(DATE_Microsecond microsecond) {
    _localTime.tv_usec = microsecond;
    return this->getTime();
}

DATE_Time CCDate::setSeconds(DATE_Second second) {
    _localDate.tm_sec = second;
    this->_syncFromLocalDate();
    return this->getTime();
}

DATE_Time CCDate::setMinutes(DATE_Minute minute) {
    _localDate.tm_min = minute;
    this->_syncFromLocalDate();
    return this->getTime();
}

DATE_Time CCDate::setHours(DATE_Hour hour) {
    _localDate.tm_hour = hour;
    this->_syncFromLocalDate();
    return this->getTime();
}

DATE_Time CCDate::setDay(DATE_Day day) {
    _localDate.tm_mday = day;
    this->_syncFromLocalDate();
    return this->getTime();
}

DATE_Time CCDate::setMonth(DATE_Month month) {
    _localDate.tm_mon = month;
    this->_syncFromLocalDate();
    return this->getTime();
}

DATE_Time CCDate::setYear(DATE_Year year) {
    _localDate.tm_year = year - 1900;
    this->_syncFromLocalDate();
    return this->getTime();
}

DATE_Time CCDate::setUTCMicroseconds(DATE_Microsecond microsecond) {
    _UTCTime.tv_usec = microsecond;
    return this->getTime();
}

DATE_Time CCDate::setUTCSeconds(DATE_Second second) {
    _UTCDate.tm_sec = second;
    this->_syncFromUTCDate();
    return this->getTime();
}

DATE_Time CCDate::setUTCMinutes(DATE_Minute minute) {
    _UTCDate.tm_min = minute;
    this->_syncFromUTCDate();
    return this->getTime();
}

DATE_Time CCDate::setUTCHours(DATE_Hour hour) {
    _UTCDate.tm_hour = hour;
    this->_syncFromUTCDate();
    return this->getTime();
}

DATE_Time CCDate::setUTCDay(DATE_Day day) {
    _UTCDate.tm_mday = day;
    this->_syncFromUTCDate();
    return this->getTime();
}

DATE_Time CCDate::setUTCMonth(DATE_Month month) {
    _UTCDate.tm_mon = month;
    this->_syncFromUTCDate();
    return this->getTime();
}

DATE_Time CCDate::setUTCYear(DATE_Year year) {
    _UTCDate.tm_year = year - 1900;
    this->_syncFromUTCDate();
    return this->getTime();
}

bool CCDate::isEqual(cocos2d::CCObject *object) {
    if (!object) return false;
    CCDate *date = dynamic_cast<CCDate *>(object);
    if (!date) return false;
    return getTime() == date->getTime();
}

std::string CCDate::description() {
    CCString *tempDescription = CCString::createWithFormat("%d %d %d %d %d %d %ld",
            this->getYear(),
            this->getMonth(),
            this->getDay(),
            this->getHours(),
            this->getMinutes(),
            this->getSeconds(),
            this->getMicroseconds());
    return tempDescription->m_sString;
}

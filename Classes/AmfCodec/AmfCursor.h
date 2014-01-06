//
//  Created by lianghanzhen on 12/30/13.
//


#ifndef __AmfCursor_H_
#define __AmfCursor_H_

#include "cocos2d.h"

class AmfCursor {

public:
    ~AmfCursor();
    static AmfCursor * init(unsigned char *buffer, size_t length);
    void reset();

    int readU29();
    char * readString(unsigned int length);

    const char getByte(unsigned int position);
    unsigned char * getBuffer();
    void consumeBytes(size_t length);
    void needBytes(size_t length);

private:
    AmfCursor();

    unsigned char *_buffer;
    size_t _left;

};


#endif //__AmfCursor_H_

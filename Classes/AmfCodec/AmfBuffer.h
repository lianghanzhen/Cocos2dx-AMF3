//
//  Created by lianghanzhen on 12/29/13.
//

#ifndef __DewinPoker__AmfBuffer__
#define __DewinPoker__AmfBuffer__

#include "cocos2d.h"

class AmfBuffer {
    
public:
    ~AmfBuffer();
    static AmfBuffer * init();
    void reset();
    char * getData();
    unsigned int getLength();
    
    void append(const char *value, size_t len);
    void appendChar(char value);
    void appendDouble(double value);
    void appendU29(int value);    
    
private:
    AmfBuffer() : _buffer(NULL), _length(0), _free(0) {};
    void _append(char *value, size_t len);
    
    char *_buffer;
    size_t _length;
    size_t _free;
    
};

#endif /* defined(__DewinPoker__AmfBuffer__) */

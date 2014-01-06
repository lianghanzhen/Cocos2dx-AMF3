//
//  Created by lianghanzhen on 12/30/13.
//

#include "AmfCursor.h"


AmfCursor::AmfCursor() {
    _buffer = NULL;
    _left = 0;
}

AmfCursor::~AmfCursor() {
    free(_buffer);
}

AmfCursor * AmfCursor::init(unsigned char *buffer, size_t length) {
    AmfCursor *cursor = new AmfCursor();
    cursor->_buffer = buffer;
    cursor->_left = length;
    return cursor;
}

void AmfCursor::reset() {
    free(_buffer);
    _buffer = NULL;
    _left = 0;
}

int AmfCursor::readU29() {
    const size_t length = 1;
    int offset = 0;
    int result = 0;
    int tmp;
    do {
        this->needBytes(length);
        tmp = _buffer[0];
        if (offset == 3) {
            result <<= 8;
            result |= tmp & 0xff;
        } else {
            result <<= 7;
            result |= tmp & 0x7f;
        }
        this->consumeBytes(length);
    } while ((++offset < 4) && (tmp & 0x80));
    return result;
}

char * AmfCursor::readString(unsigned int length) {
    this->needBytes(length);
    char *result = (char *) malloc(length + 1);
    result[length] = '\0';
    memcpy(result, _buffer, length);
    this->consumeBytes(length);
    return result;
}

const char AmfCursor::getByte(unsigned int position) {
    return _buffer[position];
}

unsigned char * AmfCursor::getBuffer() {
    return _buffer;
}

void AmfCursor::consumeBytes(size_t length) {
    _buffer += length;
    _left -= length;
}

void AmfCursor::needBytes(size_t length) {
    CCAssert(_left >= length, "AMF decode EOF error!!!");
}

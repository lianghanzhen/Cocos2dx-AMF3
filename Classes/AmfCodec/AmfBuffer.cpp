//
//  Created by lianghanzhen on 12/29/13.
//

#include "AmfBuffer.h"
#include "AmfEndiness.h"

AmfBuffer::~AmfBuffer() {
    free(_buffer);
}

AmfBuffer * AmfBuffer::init() {
    return new AmfBuffer();
}

void AmfBuffer::reset() {
    free(_buffer);
    _buffer = NULL;
    _length = 0;
    _free = 0;
}

char * AmfBuffer::getData() {
    return _buffer;
}

unsigned int AmfBuffer::getLength() {
    return _length;
}

void AmfBuffer::append(const char *value, size_t len) {
    if (_free < len) {
        size_t newLen = _length + len;
        _buffer = (char *) realloc(_buffer, newLen * 2);
        _free = newLen;
    }
    memcpy(_buffer + _length, value, len);
    _length += len;
    _free -= len;
}

void AmfBuffer::appendChar(char value) {
    append(&value, 1);
}

void AmfBuffer::appendDouble(double value) {
    _append((char *) &value, 8);
}

void AmfBuffer::appendU29(int value) {
    char bytes[4];
    size_t size;
    value &= 0x1fffffff;
    if (value <= 0x7f) {
        bytes[0] = (char) value;
        size = 1;
    } else if (value <= 0x3fff) {
        bytes[1] = (char) (value & 0x7f);
        value >>= 7;
        bytes[0] = (char) (value | 0x80);
        size = 2;
    } else if (value <= 0x1fffff) {
        bytes[2] = (char) (value & 0x7f);
        value >>= 7;
        bytes[1] = (char) (value | 0x80);
        value >>= 7;
        bytes[0] = (char) (value | 0x80);
        size = 3;
    } else {
        bytes[3] = (char) (value);
        value >>= 8;
        bytes[2] = (char) (value | 0x80);
        value >>= 7;
        bytes[1] = (char) (value | 0x80);
        value >>= 7;
        bytes[0] = (char) (value | 0x80);
        size = 4;
    }
    append(bytes, size);
}

void AmfBuffer::_append(char *value, size_t len) {
    REVERSE_IF_LITTLE_ENDIAN(value, len);
    append(value, len);
}

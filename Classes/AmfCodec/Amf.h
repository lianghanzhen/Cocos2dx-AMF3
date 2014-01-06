//
//  Created by lianghanzhen on 12/29/13.
//


#ifndef __Amf_H_
#define __Amf_H_


typedef enum{
    kAMF3UndefinedType = 0x00,
    kAMF3NullType = 0x01,
    kAMF3FalseType = 0x02,
    kAMF3TrueType = 0x03,
    kAMF3IntegerType = 0x04,
    kAMF3DoubleType = 0x05,
    kAMF3StringType = 0x06,
    kAMF3XMLDocType = 0x07, // unsupported
    kAMF3DateType = 0x08,
    kAMF3ArrayType = 0x09,
    kAMF3ObjectType = 0x0A,
    kAMF3XMLType = 0x0B, // unsupported
    kAMF3ByteArrayType = 0x0C, // unsupported
    kAMF3VectorIntType = 0x0D, // unsupported
    kAMF3VectorUintType = 0x0E, // unsupported
    kAMF3VectorDoubleType = 0x0F, // unsupported
    kAMF3VectorObjectType = 0x10, // unsupported
    kAMF3DictionaryType = 0x11  // unsupported
} AMF3Type;

#define AMF3_MAX_INT     268435455 //  (2^28)-1
#define AMF3_MIN_INT    -268435456 // -(2^28)

#define AMF3_MAX_STR_LEN    268435455


#endif //__Amf_H_

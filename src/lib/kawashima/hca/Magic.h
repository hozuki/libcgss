#ifndef KAWASHIMA_MAGIC_H
#define KAWASHIMA_MAGIC_H

#include "../kstypedef.h"

struct Magic {

public:
    static ubool Match(uint32 toCheck, uint32 standard);

    static const uint32 HCA;
    static const uint32 FORMAT;
    static const uint32 COMPRESS;
    static const uint32 DECODE;
    static const uint32 VBR;
    static const uint32 ATH;
    static const uint32 LOOP;
    static const uint32 CIPHER;
    static const uint32 RVA;
    static const uint32 COMMENT;

};


#endif //KAWASHIMA_MAGIC_H

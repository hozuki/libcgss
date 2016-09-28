/*
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

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

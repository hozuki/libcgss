/*
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#include "Magic.h"

ubool Magic::Match(uint32 toCheck, uint32 standard) {
    return (ubool)((toCheck & 0x7f7f7f7f) == standard);
}

const uint32 Magic::HCA = 0x00414348;
const uint32 Magic::FORMAT = 0x00746D66;
const uint32 Magic::COMPRESS = 0x706D6F63;
const uint32 Magic::DECODE = 0x00636564;
const uint32 Magic::VBR = 0x00726276;
const uint32 Magic::ATH = 0x00687461;
const uint32 Magic::LOOP = 0x706F6F6C;
const uint32 Magic::CIPHER = 0x68706963;
const uint32 Magic::RVA = 0x00617672;
const uint32 Magic::COMMENT = 0x6D6D6F63;

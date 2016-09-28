/*
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#ifndef KAWASHIMA_HCADATA_H
#define KAWASHIMA_HCADATA_H

#include "../../kstypedef.h"

class CHcaData {

public:
    CHcaData(uint8 *data, uint32 size);

    int32 CheckBit(int32 bitSize);

    int32 GetBit(int32 bitSize);

    void AddBit(int32 bitSize);

private:
    uint8 *_data;
    int32 _size;
    int32 _bit;

};

#endif //KAWASHIMA_HCADATA_H

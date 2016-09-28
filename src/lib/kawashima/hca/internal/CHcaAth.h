/*
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */

#ifndef KAWASHIMA_HCAATH_H
#define KAWASHIMA_HCAATH_H

#include "../../kstypedef.h"
#include "../../hca_info.h"

class CHcaAth {

public:
    CHcaAth();

    ubool Init(uint16 type, uint32 key);

    const uint8 *GetTable();

private:
    uint8 _table[0x80];

    void Init0();

    void Init1(uint32 key);

};

#endif //KAWASHIMA_HCAATH_H

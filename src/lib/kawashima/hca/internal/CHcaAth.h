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

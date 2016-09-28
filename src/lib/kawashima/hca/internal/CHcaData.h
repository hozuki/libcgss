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

#include "RegisterMaskConstructors.h"

//setMask for standard register pattern
//Param:
//				tgtSectConfig = bits to be shifted
//				sectIndex			= 0-based index number for targeted sector
//				sectLen				= Length of Sector
//
//Example:
//				0000 0000 0110 0000
uint32_t setMask(uint32_t tgtSectConfig, uint32_t sectIndex, uint32_t sectLen)
{
	return (tgtSectConfig<<(sectIndex*sectLen));
}

//clearMask for standard register pattern
//Param:
//				tgtSectConfig = bits to be shifted
//				sectIndex			= 0-based index number for targeted sector
//				sectLen				= Length of Sector
//
//Example:
//				1111 1111 0110 1111
uint32_t clearMask(uint32_t tgtSectConfig, uint32_t sectIndex, uint32_t sectLen)
{
	return ((tgtSectConfig ^ ((1<<sectLen) - 1))<<(sectLen*sectIndex));
}

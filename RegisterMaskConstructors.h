#ifndef RegisterMaskConstructors
#define RegisterMaskConstructors

#include "stm32l432xx.h"

uint32_t setMask(uint32_t tgtSectConfig, uint32_t sectIndex, uint32_t sectLen);
uint32_t clearMask(uint32_t tgtSectConfig, uint32_t sectIndex, uint32_t sectLen);

#endif //RegisterMaskConstructors

#ifndef REGISTER_MASK_CONSTRUCTORS_H
#define REGISTER_MASK_CONSTRUCTORS_H

#include "stm32l432xx.h"

uint32_t setMask(uint32_t tgtSectConfig, uint32_t sectIndex, uint32_t sectLen);
uint32_t clearMask(uint32_t tgtSectConfig, uint32_t sectIndex, uint32_t sectLen);

#endif //REGISTER_MASK_CONSTRUCTORS_H

#ifndef KLAW_H
#define KLAW_H

#include "frdm_bsp.h"

#define PTA5_8_MASK_IN_ROWS	0x000001E0
#define PTA9_12_MASK_OUT_COLS	0x00001E00
#define PTA5_12_MASK	0x00001FE0
#define SET_PORTA 0x000001FE0


void Klaw_Init(void);


#endif  /* KLAW_H */

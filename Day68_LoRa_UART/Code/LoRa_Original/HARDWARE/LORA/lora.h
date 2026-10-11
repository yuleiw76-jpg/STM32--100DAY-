#ifndef __LORA_H
#define __LORA_H	 
#include "sys.h"

#define LORA_MD0 PAout(6) 

u8* LORA_CMD(u8 *cmd, u8 *ack);
void Lora_Init(void);
#endif

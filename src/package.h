#ifndef __PACKAGE_H__
#define __PACKAGE_H__

#define LINUX_HOST

#ifndef LINUX_HOST
#include "stm32f0xx.h"
struct package{
	uint8_t head[3];
	uint8_t channel;
	uint32_t value1;
	uint32_t value2;
	uint8_t checkout;
	uint8_t tail[4];
};

#else
#include "type.h"
#pragma pack(1)
struct package{
	uint8_t channel;
	uint32_t value1;
	uint32_t value2;
	uint8_t checkout;
};
#pragma pack()

#endif //LINUX_HOST


#endif //__PACKAGE_H__


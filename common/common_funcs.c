#include "common_funcs.h"

void itoa(uint32_t val, uint32_t base,  char *bufstr)
{
	uint8_t buf[32] = {0};
	int i = 31, j = 0;
	do{
		buf[i] = "0123456789abcdef"[val % base];
		val = val/base;
		i--;
	}while(val != 0);

	buf[i] = 0;
	i++;
	while (i < 32)
	{
		bufstr[j]=buf[i];
		i++;
		j++;
	}
	bufstr[j]=0;
}

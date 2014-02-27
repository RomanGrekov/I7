#ifndef GLOBS_H
#define GLOBS_H
#include <stdint.h>

#define F_CPU 12000000

#pragma pack(push,1)
struct SavedDomain{
	uint8_t first_run;
	uint8_t state;
	uint8_t alarm_in_memory;
	uint32_t c;
	uint8_t privat_tel_num_1[20];
	uint8_t privat_tel_num_2[20];
	uint8_t usr_pwd[5];
};
#pragma pack(pop)
extern struct SavedDomain SysConf;

enum {DISARM, HOME, AWAY};
#endif  /* GLOBS_H */

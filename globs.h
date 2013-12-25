#ifndef GLOBS_H
#define GLOBS_H

#define F_CPU 12000000

#pragma pack(push,1)
struct SavedDomain{
	uint8_t first_run;
	uint8_t b;
	uint32_t c;
	uint8_t privat_tel_num_1[20];
	uint8_t privat_tel_num_2[20];
};
#pragma pack(pop)
extern struct SavedDomain SysConf;

#endif  /* GLOBS_H */

#ifndef GLOBS_H
#define GLOBS_H

#define F_CPU 12000000

#pragma pack(push,1)
struct SavedDomain{
	uint8_t first_run;
	uint8_t b;
	uint32_t c;
	uint8_t priv_num1[20];
};
#pragma pack(pop)

#endif  /* GLOBS_H */

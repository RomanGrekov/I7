#ifndef GLOBS_H
#define GLOBS_H

typedef struct {
	uint8_t to_run;
	uint8_t in_progress;
	uint8_t done;
	uint32_t timer;
} task;

#define F_CPU 12000000

#endif  /* GLOBS_H */

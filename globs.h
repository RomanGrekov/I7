#ifndef GLOBS_H
#define GLOBS_H

typedef struct {
	uint8_t to_run;
	uint8_t in_progress;
	uint8_t done;
	uint32_t timer;
} task;

typedef	struct {
		uint8_t resp_data[32];
		uint8_t resp_res;
		uint8_t resp_expect;
} usart_resp;

#endif  /* GLOBS_H */

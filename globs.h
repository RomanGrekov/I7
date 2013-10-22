#ifndef GLOBS_H
#define GLOBS_H

#define SIZE_BUF 64 //Rx buffer size
#define USART_RESP 200 // Response from at command buffer size

typedef struct {
	uint8_t to_run;
	uint8_t in_progress;
	uint8_t done;
	uint32_t timer;
} task;

typedef	struct {
		uint8_t resp_data[USART_RESP];
		uint8_t resp_res;
		uint8_t resp_expect;
		uint32_t timeout;
		uint8_t length;
} usart_resp;

#endif  /* GLOBS_H */

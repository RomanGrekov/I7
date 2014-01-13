#include "sim_send_cmd.h"

uint8_t cmd_amount = 3;
struct command commands[3]={
		{"at\r\n", 500},
		{"at+cfun?\r\n\0", 1000},
		{"at+cusd=1,\"*101#\"\r\n", 7000}
};

void cmd_timed_out(void);
void next_cmd(void);
void prev_cmd(void);
uint8_t cmd_sent=0;
uint8_t cur_cmd_id=0;
uint8_t timed_out_flag=0;

void send_test_cmds(void){
	button *btn_obj;
	uint8_t btn;
	uint8_t old_cmd_id=1;

	do{
		btn_obj = get_btn();
		btn = btn_obj->button;
		switch (btn){
		case '2':
			changeMenu(MENU_THIS);
			break;
		case '6':
			next_cmd();
			break;
		case '4':
			prev_cmd();
			break;
		case '*':
			USARTSendCmd(commands[cur_cmd_id].cmd);
			wait_for_resp();
			break;
		}

		if (cur_cmd_id != old_cmd_id){
			lcd_clrscr();
			LCDPrintS(commands[cur_cmd_id].cmd);
			LCDLine(1);
			LCDPrintS("press * to send");
		}
		old_cmd_id = cur_cmd_id;



	}while (btn != '2');
}

void cmd_timed_out(void){
	lcd_clrscr();
	LCDPrintS("Result");
	LCDLine(1);
	LCDPrintS("timed out");
	timed_out_flag=1;
}

void next_cmd(void){
	if(cur_cmd_id < (cmd_amount-1))cur_cmd_id++;
	else cur_cmd_id=0;
}
void prev_cmd(void){
	if(cur_cmd_id > 0)cur_cmd_id--;
	else cur_cmd_id=(cmd_amount-1);
}

void wait_for_resp(void){
	struct usart_response *res;
	uint8_t data[10];
	uint8_t timer_id;
	timed_out_flag=0;

	lcd_clrscr();
	LCDPrintS("Sent ...");
	timer_id = Slow_Timer_Add(tm_Once, commands[cur_cmd_id].timeout, cmd_timed_out);

	while(!timed_out_flag){
		if(USARTHasResp()){
			Slow_Timer_Delete(timer_id);
			lcd_clrscr();
			LCDPrintS("Result:");
				if (cur_cmd_id != 2)// If sent command isn't money check
				{
					res = USARTGetResp();
					LCDLine(1);
					LCDPrintS(res->resp_data);
				}
				else{
					if (USARTFindCmdWithData("+CUSD: 0,\"", "UAH", data)){
						LCDLine(1);
						LCDPrintS(data);
						cmd_sent = 0;
					}
					else{
						cmd_timed_out();
					}

				}
			timed_out_flag=1;
		}
	}

}

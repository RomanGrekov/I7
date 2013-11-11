#include "sim_send_cmd.h"

uint8_t cmd_amount = 3;
struct command commands[3]={
		{"at\r\n", 500},
		{"at+cfun?\r\n", 1000},
		{"at+cusd=1,\"*101#\"\r\n", 7000}
};

void send_test_cmds(void){
	button *btn_obj;
	uint8_t btn;
	uint8_t cur_cmd_id=0, old_cmd_id=1, cmd_sent=0;
	struct usart_response *res;
	uint8_t data[10]={1,1,1,1,1,1,1,1,1,1};
	uint32_t timeout;

	do{
		if (cur_cmd_id != old_cmd_id){
			lcd_clrscr();
			LCDPrintS(commands[cur_cmd_id].cmd);
			LCDLine(1);
			LCDPrintS("press * to send");
		}
		old_cmd_id = cur_cmd_id;

		if (cmd_sent){
			lcd_clrscr();
			LCDPrintS("Sent ...");
			delay_timer_ms(timeout);
			if(USARTHasResp()){
				lcd_clrscr();
				LCDPrintS("Result:");
				if (cur_cmd_id != 2)// If sent command isn't money check
				{
					res = USARTGetResp();
					LCDLine(1);
					LCDPrintS(res->resp_data);
					cmd_sent = 0;
				}
				else{
					if (USARTFindCmdWithData("+CUSD: 0,\"", "UAH", data)){
						LCDLine(1);
						LCDPrintS(data);
						cmd_sent = 0;
					}
					else{
						LCDLine(1);
						LCDPrintS("timed out");
						cmd_sent = 0;

					}

				}
			}
			else{
				lcd_clrscr();
				LCDPrintS("Result");
				LCDLine(1);
				LCDPrintS("timed out");
				cmd_sent = 0;
			}
		}

		btn_obj = get_btn();
		btn = btn_obj->button;
		switch (btn){
		case '2':
			changeMenu(MENU_THIS);
			break;
		case '6':
			if(cur_cmd_id < (cmd_amount-1))cur_cmd_id++;
			else cur_cmd_id=0;
			break;
		case '4':
			if(cur_cmd_id > 0)cur_cmd_id--;
			else cur_cmd_id=(cmd_amount-1);
			break;
		case '*':
			USARTSendCmd(commands[cur_cmd_id].cmd);
			cmd_sent = 1;
			timeout = commands[cur_cmd_id].timeout;
			break;
		}

	}while (btn != '2');

}

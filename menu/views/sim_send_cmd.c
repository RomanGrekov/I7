#include "sim_send_cmd.h"

void send_test_cmds(void){
	usart_resp cmd_resp;
	button *btn_obj;
	uint8_t btn, cmd_flag=0;
	uint8_t cur_cmd_id=0, old_cmd_id=1, cmd_sent=0;
	uint8_t cmd_amount = 3;
	uint8_t *commands[cmd_amount];
	commands[0] = "at\r\n";
	commands[1] = "at+cfun?\r\n";
	commands[2] = "at+cusd=1,\"*101#\"\r\n";


	do{
		if (cur_cmd_id != old_cmd_id){
			lcd_clrscr();
			LCDPrintS(commands[cur_cmd_id]);
			LCDLine(1);
			LCDPrintS("1 - send command");
		}
		old_cmd_id = cur_cmd_id;
		if (cmd_sent){
			if (cmd_flag){
				lcd_clrscr();
				LCDPrintS("Sent ...");
				cmd_flag = 0;
			}
			if (USARTFindResponse())
			{
				lcd_clrscr();
				LCDPrintS("Result");
				LCDLine(1);
				LCDPrintS(cmd_resp.resp_data);
				cmd_sent = 0;
			}
			if (cmd_resp.timeout == 0){
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
		case '1':
			USARTSendCmd(commands[cur_cmd_id], &cmd_resp, 100000);
			cmd_sent = 1;
			cmd_flag = 1;
			break;
		}

	}while (btn != '2');

}

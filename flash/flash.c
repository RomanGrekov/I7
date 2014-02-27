#include "flash.h"

#define FLASH_KEY1 ((uint32_t)0x45670123)
#define FLASH_KEY2 ((uint32_t)0xCDEF89AB)

void WriteDefConf(void){
	uint8_t privat_num1[]={'1','2','3','4','5','6',0};
	char usr_pwd[]={"1111"};

	SysConf.first_run=13;
	strncpy(SysConf.privat_tel_num_1, privat_num1, 20);
	SysConf.state=DISARM;
	strncpy(SysConf.usr_pwd, usr_pwd, 5);
	SysConf.alarm_in_memory=0;
	flash_write_struct(params_addr, &SysConf, sizeof(SysConf));
}

void flash_unlock(void) {
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
}

void flash_lock() {
    FLASH->CR |= FLASH_CR_LOCK;
}

//Функция возврщает true когда можно стирать или писать память.
uint8_t flash_ready(void) {
    return !(FLASH->SR & FLASH_SR_BSY);
}
 
//Функция стирает ВСЕ страницы. При её вызове прошивка самоуничтожается 
void flash_erase_all_pages(void) {
    FLASH->CR |= FLASH_CR_MER; //Устанавливаем бит стирания ВСЕХ страниц
    FLASH->CR |= FLASH_CR_STRT; //Начать стирание
    while(!flash_ready()) // Ожидание готовности.. Хотя оно уже наверное ни к чему здесь...
        ;
    FLASH->CR &= FLASH_CR_MER;
}
 
//Функция стирает одну страницу. В качестве адреса можно использовать любой
//принадлежащий диапазону адресов той странице которую нужно очистить.
void flash_erase_page(uint32_t address) {
    flash_unlock();
    FLASH->CR|= FLASH_CR_PER; //Устанавливаем бит стирания одной страницы
    FLASH->AR = address; // Задаем её адрес
    FLASH->CR|= FLASH_CR_STRT; // Запускаем стирание 
    while(!flash_ready())
        ;  //Ждем пока страница сотрется. 
    FLASH->CR&= ~FLASH_CR_PER; //Сбрасываем бит обратно
    flash_lock();
}

uint8_t flash_read(uint32_t address){
    return *(__IO uint16_t*)address;
}

void flash_write(uint32_t address,uint32_t data) {
    flash_unlock();
    FLASH->CR |= FLASH_CR_PG; //Разрешаем программирование флеша
    while(!flash_ready()) //Ожидаем готовности флеша к записи
        ;
    *(__IO uint16_t*)address = (uint16_t)data; //Пишем младшие 2 бата
    while(!flash_ready())
        ;
    address+=2;
    data>>=16;
    *(__IO uint16_t*)address = (uint16_t)data; //Пишем старшие 2 байта
    while(!flash_ready())
        ;
    FLASH->CR &= ~(FLASH_CR_PG); //Запрещаем программирование флеша
    FLASH->CR&= ~FLASH_CR_PER; //Сбрасываем бит обратно
    flash_lock();
}

void flash_write_struct(uint32_t address, uint8_t *struct_p, uint32_t size){
	uint16_t data=0;

	flash_erase_page(params_addr);

	for(uint32_t i=0; i<size; i+=2){
		if(i == (size-1) && (size % 2) != 0){
			data = 0xff;
			data = data << 8;
			data |= struct_p[i];
		}
		else{
			data = struct_p[i+1];
			data = data << 8;
			data |= struct_p[i];
		}

		flash_unlock();
		FLASH->CR |= FLASH_CR_PG; //Разрешаем программирование флеша
		while(!flash_ready()) //Ожидаем готовности флеша к записи
			;
		*(__IO uint16_t*)address = data; //Пишем младшие 2 бата
		while(!flash_ready())
			;
		FLASH->CR &= ~(FLASH_CR_PG); //Запрещаем программирование флеша
		FLASH->CR &= ~FLASH_CR_PER; //Сбрасываем бит обратно
		flash_lock();

		address+=2;
	}
}

void flash_read_struct(uint8_t *struct_p, uint32_t size){
	for(uint32_t i=0; i<size; i++){
		struct_p[i]=*(__IO uint32_t*)(params_addr+i);
	}
}


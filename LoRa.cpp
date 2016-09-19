#include "LoRa.h"

//USART_TypeDef *uartClass232 = USART0;
USART_TypeDef *uartClassLora = UART0;

void LoRa::uartLoraInitialize(void) {
	
//	USART_InitAsync_TypeDef uartInitTypeDef232 = USART_INITASYNC_DEFAULT;
	USART_InitAsync_TypeDef uartInitTypeDefLora = USART_INITASYNC_DEFAULT;
	
	USART_Enable_TypeDef uartEnableTypeDef = usartEnable;
	
	uartInitTypeDefLora.baudrate = 57600;
//	uartInitTypeDef232.baudrate = 57600;
	
//	USART_InitAsync(uartClass232, &uartInitTypeDef232);
	USART_InitAsync(uartClassLora, &uartInitTypeDefLora);
	
	/* uart232 */
//	GPIO_PinModeSet(gpioPortE, 13, gpioModePushPull, 1); 
//  GPIO_PinModeSet(gpioPortE, 12, gpioModeInput, 0);
	
	/* uartLoRa */
	GPIO_PinModeSet(gpioPortA, 3, gpioModePushPull, 1); 
  GPIO_PinModeSet(gpioPortA, 4, gpioModeInput, 0);
	
	/* uart232 */
//	uartClass232->ROUTE |= 1 << 0 | 1 << 1 | 1 << 8 | 1 << 9;
//	USART_Enable(uartClass232, uartEnableTypeDef);
	
	/* uartLoRa */
	uartClassLora->ROUTE |= 1 << 0 | 1 << 1 | 0 << 8 | 1 << 9;
	USART_Enable(uartClassLora, uartEnableTypeDef);
}

LoRa::LoRa() {
	GPIO_PinModeSet(gpioPortA, 15, gpioModePushPull, 0);
	GPIO_PinOutClear(gpioPortA, 15);
	osDelay(100);
	GPIO_PinOutSet(gpioPortA, 15);
	osDelay(800);
}

char* LoRa::stringConcatenate(char first[], char second[]) {
	static char temp[64];
	memset(temp, 0, sizeof(temp));
	strcpy(temp, first);
	strcat(temp, " ");
	strcat(temp,second);
	strcat(temp, "\r\n");
	return temp;
}

char* LoRa::stringConcatenate(char first[], char second[], char third[]) {
	static char temp[64];
	memset(temp, 0, sizeof(temp));
	strcpy(temp, first);
	strcat(temp, " ");
	strcat(temp, second);
	strcat(temp, " ");
	strcat(temp, third);
	strcat(temp, "\r\n");
	return temp;
}

char* LoRa::stringConcatenate(char first[], char second[], char third[], char fourth[]) {
	static char temp[64];
	memset(temp, 0, sizeof(temp));
	strcpy(temp, first);
	strcat(temp, " ");
	strcat(temp, second);
	strcat(temp, " ");
	strcat(temp, third);
	strcat(temp, " ");
	strcat(temp, fourth);
	strcat(temp, "\r\n");
	return temp;
}

char* LoRa::stringConcatenate(char first[], char second[], char third[], char fourth[], char fifth[]) {
	static char temp[64];
	memset(temp, 0, sizeof(temp));
	strcpy(temp, first);
	strcat(temp, " ");
	strcat(temp, second);
	strcat(temp, " ");
	strcat(temp, third);
	strcat(temp, " ");
	strcat(temp, fourth);
	strcat(temp, " ");
	strcat(temp, fifth);
	strcat(temp, "\r\n");
	return temp;
}

char* LoRa::stringConcatenate(char first[], char second[], char third[], char fourth[], char fifth[], char sixth[]) {
	static char temp[64];
	memset(temp, 0, sizeof(temp));
	strcpy(temp, first);
	strcat(temp, " ");
	strcat(temp, second);
	strcat(temp, " ");
	strcat(temp, third);
	strcat(temp, " ");
	strcat(temp, fourth);
	strcat(temp, " ");
	strcat(temp, fifth);
	strcat(temp, " ");
	strcat(temp, sixth);
	strcat(temp, "\r\n");
	return temp;
}

void LoRa::uartLoraSend(USART_TypeDef *tempType, char data[]) {
	int i = 0;
	while(data[i]) {
		USART_Tx(tempType, data[i]);
		i++;
	}
}

LORA_COMMAND_RESPONSE LoRa::sys::pingdig(char pinname[], char pinstate[]) {
	uartLoraSend(uartClassLora, stringConcatenate((char *)"sys pingdig", pinname, pinstate));
	osDelay(100);
	return LORA_RESPONSE_OK;
}

LORA_COMMAND_RESPONSE LoRa::sys::sleep(char value[]) {
	uartLoraSend(uartClassLora, stringConcatenate((char *)"sys get", value));
	osDelay(100);
	return LORA_RESPONSE_OK;
}
LORA_COMMAND_RESPONSE LoRa::sys::reset(void) {
	uartLoraSend(uartClassLora, (char *)"sys reset\r\n" );
	osDelay(100);
	return LORA_RESPONSE_OK;
}
LORA_COMMAND_RESPONSE LoRa::sys::eraseFW(void) {
	uartLoraSend(uartClassLora, (char *)"sys eraseFW\r\n");
	osDelay(100);
	return LORA_RESPONSE_OK;
}
LORA_COMMAND_RESPONSE LoRa::sys::factoryRESET(void) {
	uartLoraSend(uartClassLora, (char *)"sys factoryRESET\r\n");
	osDelay(100);
	return LORA_RESPONSE_OK;
}

LORA_COMMAND_RESPONSE LoRa::sys::get(char parameter[]) {
	uartLoraSend(uartClassLora, stringConcatenate((char *)"sys get", parameter));
	osDelay(100);
	return LORA_RESPONSE_OK;
}

LORA_COMMAND_RESPONSE LoRa::sys::get(char parameter[], char parameter2[]) {
	uartLoraSend(uartClassLora, stringConcatenate((char *)"sys get", parameter, parameter2));
	osDelay(100);
	return LORA_RESPONSE_OK;
}

LORA_COMMAND_RESPONSE LoRa::sys::set(char parameter[], char value[]) {
	uartLoraSend(uartClassLora, stringConcatenate((char *)"sys set", parameter, value));
	osDelay(100);
	return LORA_RESPONSE_OK;
}

LORA_COMMAND_RESPONSE LoRa::sys::set(char parameter[], char value[], char value2[]) {
	uartLoraSend(uartClassLora, stringConcatenate((char *)"sys set", parameter, value, value2));
	osDelay(100);
	return LORA_RESPONSE_OK;
}

LORA_COMMAND_RESPONSE LoRa::mac::reset(char parameter[]) {
	uartLoraSend(uartClassLora, stringConcatenate((char *)"mac reset", parameter));
	osDelay(100);
	return LORA_RESPONSE_OK;
}

LORA_COMMAND_RESPONSE LoRa::mac::tx(char parameter[], char value[]) {
	uartLoraSend(uartClassLora, stringConcatenate((char *)"mac tx", parameter, value));
	osDelay(100);
	return LORA_RESPONSE_OK;
}

LORA_COMMAND_RESPONSE LoRa::mac::tx(char parameter[], char value[], char value2[]) {
	uartLoraSend(uartClassLora, stringConcatenate((char *)"mac tx", parameter, value, value2));
	osDelay(100);
	return LORA_RESPONSE_OK;
}

LORA_COMMAND_RESPONSE LoRa::mac::rx(char parameter[], char value[]) {
	uartLoraSend(uartClassLora, stringConcatenate((char *)"mac tx", parameter, value));
	osDelay(100);
	return LORA_RESPONSE_OK;
}

LORA_COMMAND_RESPONSE LoRa::mac::join(char parameter[]) {
	uartLoraSend(uartClassLora, stringConcatenate((char *)"mac join", parameter));
	osDelay(100);
	return LORA_RESPONSE_OK;
}

LORA_COMMAND_RESPONSE LoRa::mac::save(void) {
	uartLoraSend(uartClassLora, (char *)"mac save\r\n");
	osDelay(100);
	return LORA_RESPONSE_OK;
}

LORA_COMMAND_RESPONSE LoRa::mac::forceENABLE(void) {
	uartLoraSend(uartClassLora, (char *)"mac forceENABLE");
	osDelay(100);
	return LORA_RESPONSE_OK;
}

LORA_COMMAND_RESPONSE LoRa::mac::pause(void) {
	uartLoraSend(uartClassLora, (char *)"mac pause\r\n");
	osDelay(100);
	return LORA_RESPONSE_OK;
}

LORA_COMMAND_RESPONSE LoRa::mac::resume(void) {
	uartLoraSend(uartClassLora, (char *)"mac resume\r\n");
	osDelay(100);
	return LORA_RESPONSE_OK;
}

LORA_COMMAND_RESPONSE LoRa::mac::get(char parameter[]) {
	uartLoraSend(uartClassLora, stringConcatenate((char *)"mac get", parameter));
	osDelay(100);
	return LORA_RESPONSE_OK;
}

LORA_COMMAND_RESPONSE LoRa::mac::get(char parameter[], char parameter2[]) {
	uartLoraSend(uartClassLora, stringConcatenate((char *)"mac get", parameter, parameter2));
	osDelay(100);
	return LORA_RESPONSE_OK;
}

LORA_COMMAND_RESPONSE LoRa::mac::get(char parameter[], char parameter2[], char parameter3[]) {
	uartLoraSend(uartClassLora, stringConcatenate((char *)"mac get", parameter, parameter2, parameter3));
	osDelay(100);
	return LORA_RESPONSE_OK;
}

LORA_COMMAND_RESPONSE LoRa::mac::set(char parameter[], char value[]) {
	uartLoraSend(uartClassLora, stringConcatenate((char *)"mac set", parameter, value));
	osDelay(100);
	return LORA_RESPONSE_OK;
}

LORA_COMMAND_RESPONSE LoRa::mac::set(char parameter[], char value[], char value2[]) {
	uartLoraSend(uartClassLora, stringConcatenate((char *)"mac set", parameter, value, value2));
	osDelay(100);
	return LORA_RESPONSE_OK;
}

LORA_COMMAND_RESPONSE LoRa::mac::set(char parameter[], char value[], char value2[], char value3[]) {
	uartLoraSend(uartClassLora, stringConcatenate((char *)"sys set", parameter, value, value2, value3));
	osDelay(100);
	return LORA_RESPONSE_OK;
}

LORA_COMMAND_RESPONSE LoRa::mac::set(char parameter[], char value[], char value2[], char value3[], char value4[]) {
	uartLoraSend(uartClassLora, stringConcatenate((char *)"sys set", parameter, value, value2, value3, value4));
	osDelay(100);
	return LORA_RESPONSE_OK;
}

LORA_COMMAND_RESPONSE LoRa::radio::rx(char value[]) {
	uartLoraSend(uartClassLora, stringConcatenate((char *)"radio rx", value));
	osDelay(100);
	return LORA_RESPONSE_OK;
}

LORA_COMMAND_RESPONSE LoRa::radio::tx(char value[]) {
	uartLoraSend(uartClassLora, stringConcatenate((char *)"radio tx", value));
	osDelay(100);
	return LORA_RESPONSE_OK;
}

LORA_COMMAND_RESPONSE LoRa::radio::cw(char value[]) {
	uartLoraSend(uartClassLora, stringConcatenate((char *)"radio get", value));
	osDelay(100);
	return LORA_RESPONSE_OK;
}

LORA_COMMAND_RESPONSE LoRa::radio::get(char parameter[]) {
	uartLoraSend(uartClassLora, stringConcatenate((char *)"radio get", parameter));
	osDelay(100);
	return LORA_RESPONSE_OK;
}

LORA_COMMAND_RESPONSE LoRa::radio::set(char parameter[], char value[]) {
	uartLoraSend(uartClassLora, stringConcatenate((char *)"radio set", parameter, value));
	osDelay(100);
	return LORA_RESPONSE_OK;
}

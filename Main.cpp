/** 
		Hardware
*/

#include "cmsis_os.h"
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "RingBuffer.h"
#include "LoRa.h"
#include <stdlib.h>
#include <stdio.h>

USART_TypeDef *uart232 = USART0;
USART_TypeDef *uartWifi = UART0;

float adcResult = 404.0;
char iotBuffer[16];
uint32_t adcResultwhat = 0;

void Initialize(void) {
  CMU_OscillatorEnable (cmuOsc_HFXO, true, true);
  CMU_ClockSelectSet   (cmuClock_HF,    cmuSelect_HFXO);
  CMU_ClockDivSet      (cmuClock_HFPER, cmuClkDiv_1);
 
  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_GPIO, true);
  CMU_ClockEnable(cmuClock_USART0, true);
	CMU_ClockEnable(cmuClock_UART0, true);
//	CMU_ClockEnable(cmuClock_ADC0, true);		
}

void adcFunc(void) {	
	
	ADC0->CTRL = (24 << 16) | (4 << 8);
	ADC0->IEN = 0;
	ADC0->CMD = 1 << 0;
	ADC0->SINGLECTRL = (2 << 16) | (4 << 8) | (0 << 4); // VDD reference, adcn_ch4, 12bit resolution
	
	while (! (ADC0->STATUS & ( 1 << 16 )));
		adcResult = ADC0->SINGLEDATA;
		ADC0->CMD = 1 << 0;
	  adcResult = (((adcResult/4095)*(5*3.3)/3)*1000);
	  adcResultwhat = adcResult;
	  sprintf(iotBuffer,"%x",adcResultwhat);
}

void blinky(void const* arg) {
	
	GPIO_PinModeSet(gpioPortA, 7, gpioModePushPull, 0);
	
	while(1) {
		GPIO_PinOutSet(gpioPortA, 7);
		osDelay(1000);
		GPIO_PinOutClear(gpioPortA, 7);
		osDelay(50);
	}
}
osThreadDef(blinky, osPriorityNormal, 1, 0);

void uart0Initialize(void) {
	
	USART_InitAsync_TypeDef uartInitTypeDef232 = USART_INITASYNC_DEFAULT;
	USART_InitAsync_TypeDef uartInitTypeDefWifi = USART_INITASYNC_DEFAULT;
	
	USART_Enable_TypeDef uartEnableTypeDef = usartEnable;
	
	uartInitTypeDefWifi.baudrate = 115200;
	uartInitTypeDef232.baudrate = 115200;
	
	USART_InitAsync(uart232, &uartInitTypeDef232);
	USART_InitAsync(uartWifi, &uartInitTypeDefWifi);
	
	/* uart232 */
	GPIO_PinModeSet(gpioPortE, 13, gpioModePushPull, 1); 
  GPIO_PinModeSet(gpioPortE, 12, gpioModeInput, 0);
	
	/* uartWifi */
	GPIO_PinModeSet(gpioPortA, 3, gpioModePushPull, 1); 
  GPIO_PinModeSet(gpioPortA, 4, gpioModeInput, 0);
	
	/* uart232 */
	uart232->ROUTE |= 1 << 0 | 1 << 1 | 1 << 8 | 1 << 9;
	USART_Enable(uart232, uartEnableTypeDef);
	
	/* uartWifi */
	uartWifi->ROUTE |= 1 << 0 | 1 << 1 | 0 << 8 | 1 << 9;
	USART_Enable(uartWifi, uartEnableTypeDef);
}

void uartSend(USART_TypeDef *tempType, char data[]) {
	int i = 0;
	while(data[i]) {
		USART_Tx(tempType, data[i]);
//		USART_Tx(uart232, data[i]); //for debug
		i++;
	}
}

//void ReceiveThread(void const *arg) {
//	uint8_t temp;
//	while (1) {
//		temp = USART_Rx(uart232);
//		uartSend(uart232, (char*)(&temp));
//	}
//}
//osThreadDef(ReceiveThread, osPriorityNormal, 1, 0);

void ReceiveThread2(void const *arg) {
	uint8_t temp;
	while (1) {
		temp = USART_Rx(uartWifi);
		uartSend(uart232, (char *)(&temp));
	}
}
osThreadDef(ReceiveThread2, osPriorityNormal, 1, 0);

void WifiTransmit(void const *arg) {
	
	uartSend(uartWifi,(char *) "AT\r\n");
	osDelay(1000);
	uartSend(uartWifi,(char *) "AT+CIPMUX=1\r\n");
	osDelay(1000);
	uartSend(uartWifi,(char *) "AT+CIPSERVER=1,60200\r\n");
	osDelay(1000);
	uartSend(uartWifi,(char *) "AT+CIPSTO=0\r\n");
	osDelay(1000);
	
//	while (1) {
//		
//	}
}
osThreadDef(WifiTransmit, osPriorityNormal, 1, 0);

void LoraReceive(void const *arg) {
	
	while (1) {
		osDelay(1000);
		uartSend(uart232, (char *)"snr: ");
	}
}
osThreadDef(LoraReceive, osPriorityNormal, 1, 0);

void RunThread(void const *argument) {	
//	LoraInitialize();
//	LoraConfiguration();
//	while (1) {
//	}
}
osThreadDef(RunThread, osPriorityNormal, 1, 0);

int main (void) {
	
  SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/1000); // 1 milisecond SysTick
	
	osKernelInitialize();
	Initialize();
	uart0Initialize();
	
	osDelay(1000);
	
	uartSend(uart232, (char *) "\r\nSystem Initialize\r\n");
	osThreadCreate(osThread(blinky), NULL);
//	osThreadCreate(osThread(ReceiveThread), NULL);
	osThreadCreate(osThread(ReceiveThread2), NULL);
//	osThreadCreate(osThread(RunThread), NULL);
	
	osThreadCreate(osThread(WifiTransmit), NULL);
//	osThreadCreate(osThread(LoraReceive), NULL);
	
	osKernelStart();
	
	return 0;
}

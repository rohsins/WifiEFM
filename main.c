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
USART_TypeDef *uartLora = UART0;

float adcResult = 404.0;
char iotBuffer[16];
uint32_t adcResultwhat = 0;

void Initialize(void) {
	 
//  CHIP_Init();
 
  CMU_OscillatorEnable (cmuOsc_HFXO, true, true);
  CMU_ClockSelectSet   (cmuClock_HF,    cmuSelect_HFXO);
  CMU_ClockDivSet      (cmuClock_HFPER, cmuClkDiv_1);
 
  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_GPIO, true);
  CMU_ClockEnable(cmuClock_USART0, true);
	CMU_ClockEnable(cmuClock_UART0, true);
	CMU_ClockEnable(cmuClock_ADC0, true);
 
//  GPIO_PinModeSet(gpioPortE, 4, gpioModePushPull, 0);
//  GPIO_PinModeSet(gpioPortE, 1, gpioModePushPull, 0); 
//  GPIO_PinModeSet(gpioPortE, 2, gpioModePushPull, 0); 
//  GPIO_PinModeSet(gpioPortE, 3, gpioModePushPull, 0);
//	
//	GPIO_PinModeSet(gpioPortD, 2, gpioModeInput, 0);
//	
//	GPIO_PinModeSet(gpioPortE, 2, gpioModePushPull, 0);
//	GPIO_PinOutSet(gpioPortE,2);
//	GPIO_PinModeSet(gpioPortE, 3, gpioModePushPull, 0);
//	GPIO_PinOutSet(gpioPortE, 3);
//	GPIO_PinModeSet(gpioPortA, 15, gpioModePushPull, 0);
//	GPIO_PinOutSet(gpioPortE, 15);		
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
	USART_InitAsync_TypeDef uartInitTypeDefLora = USART_INITASYNC_DEFAULT;
	
	USART_Enable_TypeDef uartEnableTypeDef = usartEnable;
	
	uartInitTypeDefLora.baudrate = 57600;
	uartInitTypeDef232.baudrate = 115200;
	
	USART_InitAsync(uart232, &uartInitTypeDef232);
	USART_InitAsync(uartLora, &uartInitTypeDefLora);
	
	/* uart232 */
	GPIO_PinModeSet(gpioPortE, 13, gpioModePushPull, 1); 
  GPIO_PinModeSet(gpioPortE, 12, gpioModeInput, 0);
	
	/* uartLoRa */
	GPIO_PinModeSet(gpioPortA, 3, gpioModePushPull, 1); 
  GPIO_PinModeSet(gpioPortA, 4, gpioModeInput, 0);
	
	/* uart232 */
	uart232->ROUTE |= 1 << 0 | 1 << 1 | 1 << 8 | 1 << 9;
	USART_Enable(uart232, uartEnableTypeDef);
	
	/* uartLoRa */
	uartLora->ROUTE |= 1 << 0 | 1 << 1 | 0 << 8 | 1 << 9;
	USART_Enable(uartLora, uartEnableTypeDef);
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
		temp = USART_Rx(uartLora);
		uartSend(uart232, (char *)(&temp));
	}
}
osThreadDef(ReceiveThread2, osPriorityNormal, 1, 0);

void LoraInitialize(void) {
		//lora reset
	GPIO_PinModeSet(gpioPortA, 15, gpioModePushPull, 0);
	GPIO_PinOutClear(gpioPortA, 15);
	osDelay(100);
	GPIO_PinOutSet(gpioPortA, 15);
	
	osDelay(2000);
	
//	uartSend(uartLora, "mac set devaddr D43EBB86\r\n");
//	osDelay(1000);
//	uartSend(uartLora, "mac set deveui A4D894B00B1E5096\r\n");
//	osDelay(1000);
//	uartSend(uartLora, "mac set appeui B12498D9428361AE\r\n");
//	osDelay(1000);
//	uartSend(uartLora, "mac set appkey 921846A329B4C13D3041281942183627\r\n"); //921846A329B4C13D3041281942183627   D21846A329B4C13D
//	osDelay(1000);
//	uartSend(uartLora, "mac set nwkskey 2B76D5DDA85A891ECCAFB3ABCAD2F254\r\n"); //921846A329B4C13D3041281942183627   D21846A329B4C13D
//	osDelay(1000);
//	uartSend(uartLora, "mac set appskey E536195F254E138C03D05FFE9CBF7A39\r\n"); //921846A329B4C13D3041281942183627   D21846A329B4C13D
//	osDelay(1000);
//	uartSend(uartLora, "mac save\r\n");
//	osDelay(1000);

	uartSend(uartLora,(char *) "mac join otaa\r\n");
	osDelay(1000);
	uartSend(uart232,(char *) "Device Address: ");
	uartSend(uartLora, (char *) "mac get devaddr\r\n");
	osDelay(100);
	uartSend(uart232, (char *) "Device EUI Address: ");
	uartSend(uartLora, (char *) "mac get deveui\r\n");
	osDelay(100);
	uartSend(uart232, (char *) "Device APP EUI Address: ");
	uartSend(uartLora, (char *) "mac get appeui\r\n");
	osDelay(100);
	uartSend(uart232, (char *) "Device APP KEY: ");
	uartSend(uart232, (char *) "921846A329B4C13D3041281942183627\r\n");
	osDelay(100);
	uartSend(uart232, (char *) "Device NWKSKEY: ");
	uartSend(uart232, (char *) "2B76D5DDA85A891ECCAFB3ABCAD2F254\r\n"); //921846A329B4C13D3041281942183627   D21846A329B4C13D
	osDelay(100);
	uartSend(uart232, (char *) "Device APPSKEY: ");
	uartSend(uart232, (char *) "E536195F254E138C03D05FFE9CBF7A39\r\n"); //921846A329B4C13D3041281942183627   D21846A329B4C13D
	osDelay(100);
	uartSend(uartLora, (char *) "mac tx cnf 214 DE\r\n"); //921846A329B4C13D3041281942183627   D21846A329B4C13D
	osDelay(100);
	uartSend(uart232, (char *) "SNR: ");
	uartSend(uartLora, (char *) "radio get snr\r\n");
	osDelay(100);
	uartSend(uart232, (char *) "Current Data Rate: ");
	uartSend(uartLora, (char *) "mac get dr\r\n");
	osDelay(100);
	uartSend(uart232, (char *) "Number of retransmissions: ");
	uartSend(uartLora, (char *) "mac get retx\r\n");
	osDelay(100);
	uartSend(uart232, (char *) "Bandwidth: ");
	uartSend(uartLora, (char *) "mac get rx2\r\n");
	osDelay(100);
	uartSend(uart232, (char *) "Automatic Reply: ");
	uartSend(uartLora, (char *) "mac get ar\r\n");
	osDelay(100);
	uartSend(uartLora, (char *) "mac pause\r\n");
	osDelay(1000);
	uartSend(uartLora, (char *) "radio set wdt 0\r\n");
	osDelay(100);
	uartSend(uartLora, (char *) "radio rx 0\r\n");
	osDelay(100);
}

void LoraConfiguration(void) {
	
	static LoRa wlora; 
	
	wlora.sys.reset();
	wlora.sys.get(vdd);
	
//	wlora.mac.set(devaddr, DEVADDR);
//	wlora.mac.set(deveui, DEVEUI);
//	wlora.mac.set(appeui, APPEUI);
//	wlora.mac.set(nwkskey, NWKSKEY);
//	wlora.mac.set(appskey, APPSKEY);
//	wlora.mac.set(appkey, APPKEY);
	wlora.mac.set(pwridx, (char *)"1"); // set the tx output power to 14dBm
	wlora.mac.set(dr, (char *)"5"); //datarate 125kHz
	wlora.mac.set(adr, on); //enable the adaptive data rate(ADR)
	wlora.mac.set(bat, (char *)"127"); // battery is set to 50 %
	wlora.mac.set(retx, (char *)"5"); // the number of retransmission made for a an uplink confirmed packet is set to 5
	wlora.mac.set(linkchk , (char *)"100"); // the module will attempt a link check process at 100 second intervals
	wlora.mac.set(rxdelay1, (char *) "1000"); //set the delay between the transmission and the first Receive window to 1000 ms.
	wlora.mac.set(ar, on); //enables the automatic reply process inside the module
	wlora.mac.set(rx2,(char *) "3", (char *) "869525000"); //receive window 2 is configured with sf9/125kHz data rate with a center frequency of 865MHz.
//	wlora.mac.set(ch, freq, (char *) "8", (char *) "864000000"); // define frequency for channel 8 to be 864.
//	wlora.mac.set(ch, dcycle, (char *) "8", (char *) "9"); // defines duty cycle for channel 8 to be 10%. Since(100/10) -1 = 9, the parameter that gets configured is 9.
//	wlora.mac.set(ch, drrange, (char *) "8", (char *) "0", (char *) "2"); // on channel 8 the data rate can range from 0 (sf12/125kHz) to 2 (sf10/125) as required.
//	wlora.mac.save();

	wlora.mac.tx(uncnf, (char *) "1");

	uartSend(uart232, (char *) "Device APP KEY: ");
	uartSend(uart232, (char *) "921846A329B4C13D3041281942183627\r\n");
	osDelay(100);
	uartSend(uart232, (char *) "Device NWKSKEY: ");
	uartSend(uart232, (char *) "2B76D5DDA85A891ECCAFB3ABCAD2F254\r\n"); //921846A329B4C13D3041281942183627   D21846A329B4C13D
	osDelay(100);
	uartSend(uart232, (char *) "Device APPSKEY: ");
	uartSend(uart232, (char *) "E536195F254E138C03D05FFE9CBF7A39\r\n"); //921846A329B4C13D3041281942183627   D21846A329B4C13D
	osDelay(100);
	
	wlora.mac.join(abp);
	
	uartSend(uart232, (char *)"bt: none\r\n");
	wlora.radio.set(bt, none);
	uartSend(uart232, (char *)"mod: lora\r\n");
	wlora.radio.set(mod, lora);
	uartSend(uart232, (char *)"freq: 868000000\r\n");
	wlora.radio.set(freq, (char *)"868000000");
	uartSend(uart232, (char *)"pwr: 14\r\n");
	wlora.radio.set(pwr, (char *) "14");
	uartSend(uart232, (char *)"sf: sf10\r\n");
	wlora.radio.set(sf, (char *) "sf10");
	uartSend(uart232, (char *)"afcbw: 125\r\n");
	wlora.radio.set(afcbw, (char *) "125");
	uartSend(uart232, (char *)"rxbw: 125\r\n");
	wlora.radio.set(rxbw, (char *) "125");
	uartSend(uart232, (char *)"bitrate: 5000\r\n");
	wlora.radio.set(bitrate, (char *) "5000");
	uartSend(uart232, (char *)"fdev: 5000\r\n");
	wlora.radio.set(fdev, (char *) "5000");
	uartSend(uart232, (char *)"prlen: 8\r\n");
	wlora.radio.set(prlen, (char *) "8");
	uartSend(uart232, (char *)"crc: on\r\n");
	wlora.radio.set(crc, on);
	uartSend(uart232, (char *)"iqi: on\r\n");
	wlora.radio.set(iqi, on);
	uartSend(uart232, (char *)"cr: 4/7\r\n");
	wlora.radio.set(cr, (char *) "4/7");
	uartSend(uart232, (char *)"wdt: 0\r\n");
	wlora.radio.set(wdt, (char *) "0");
	uartSend(uart232, (char *)"sync: 12\r\n");
	wlora.radio.set(sync, (char *) "12");
	uartSend(uart232, (char *)"bw: 125\r\n");
	wlora.radio.set(bw, (char *) "125");
	
//	uartSend(uart232, (char *)"\r\n\r\n");
//	
//	uartSend(uart232, (char *)"devaddr: ");
//	wlora.mac.get(devaddr);
//	uartSend(uart232, (char *)"deveui: ");
//	wlora.mac.get(deveui);
//	uartSend(uart232, (char *)"appeui: ");
//	wlora.mac.get(appeui);
//	uartSend(uart232, (char *)"dr: ");
//	wlora.mac.get(dr);
//	uartSend(uart232, (char *)"band: ");
//	wlora.mac.get(band);
//	uartSend(uart232, (char *)"pwridx: ");
//	wlora.mac.get(pwridx);
//	uartSend(uart232, (char *)"adr: ");
//	wlora.mac.get(adr);
//	uartSend(uart232, (char *)"retx: ");
//	wlora.mac.get(retx);
//	uartSend(uart232, (char *)"rxdelay1: ");
//	wlora.mac.get(rxdelay1);
//	uartSend(uart232, (char *)"rxdelay2: ");
//	wlora.mac.get(rxdelay2);
//	uartSend(uart232, (char *)"ar: ");
//	wlora.mac.get(ar);
//	uartSend(uart232, (char *)"rx2: ");
//	wlora.mac.get(rx2);
//	uartSend(uart232, (char *)"dcycleps: ");
//	wlora.mac.get(dcycleps);
//	uartSend(uart232, (char *)"mrgn: ");
//	wlora.mac.get(mrgn);
//	uartSend(uart232, (char *)"gwnb: ");
//	wlora.mac.get(gwnb);
////	uartSend(uart232, (char *)"status: ");
////	wlora.mac.get(status);
//	uartSend(uart232, (char *)"ch: ");
//	wlora.mac.get(ch, freq, (char *) "8");
	
	uartSend(uart232, (char *)"\r\n\r\n");
	
	wlora.mac.pause();
	
//	wlora.radio.tx((char *)"48656C6C6F");
	
//	while (1) {
//		osDelay(5000);
//		wlora.radio.tx((char *)"48656C6C6F");
//	}
	
	wlora.radio.rx((char *) "0"); //puts the radio into continuous Receive mode.
	
	
//	wlora.sys.get(hweui);
//	wlora.mac.reset((char *)"868");
//	wlora.mac.tx(cnf, (char *)"214", (char*)"ABCDA");
	
//	wlora.sys
}

void LoraTransmit(void const *arg) {
	LoRa wlora;
	
	wlora.sys.reset();
	
//	wlora.mac.tx(uncnf, (char *)"1");
	osDelay(1000);
	wlora.mac.set(devaddr, DEVADDR);
	wlora.mac.set(deveui, DEVEUI);
	wlora.mac.set(appeui, APPEUI);
	wlora.mac.set(nwkskey, NWKSKEY);
	wlora.mac.set(appskey, APPSKEY);
	wlora.mac.set(appkey, APPKEY);
	
	wlora.mac.set(pwridx, (char *)"1"); // set the tx output power to 14dBm
	wlora.mac.set(dr, (char *)"5"); //datarate 125kHz
	wlora.mac.set(adr, on); //enable the adaptive data rate(ADR)
	wlora.mac.set(bat, (char *)"127"); // battery is set to 50 %
	wlora.mac.set(retx, (char *)"5"); // the number of retransmission made for a an uplink confirmed packet is set to 5
	wlora.mac.set(linkchk , (char *)"100"); // the module will attempt a link check process at 100 second intervals
	wlora.mac.set(rxdelay1, (char *) "1000"); //set the delay between the transmission and the first Receive window to 1000 ms.
	wlora.mac.set(ar, on); //enables the automatic reply process inside the module
	wlora.mac.set(rx2,(char *) "3", (char *) "868500000"); //receive window 2 is configured with sf9/125kHz data rate with a center frequency of 865MHz.
//	wlora.mac.set(ch, freq, (char *) "8", (char *) "864000000"); // define frequency for channel 8 to be 864.
//	wlora.mac.set(ch, dcycle, (char *) "8", (char *) "9"); // defines duty cycle for channel 8 to be 10%. Since(100/10) -1 = 9, the parameter that gets configured is 9.
//	wlora.mac.set(ch, drrange, (char *) "8", (char *) "0", (char *) "2"); // on channel 8 the data rate can range from 0 (sf12/125kHz) to 2 (sf10/125) as required.
	wlora.mac.save();
	
	osDelay(5000);
	
	wlora.mac.get(devaddr);
	osDelay(1000);
	wlora.mac.get(deveui);
	osDelay(1000);
	wlora.mac.get(appeui);
	osDelay(1000);
	
	wlora.mac.join(abp);

//	wlora.mac.tx(cnf, (char *)"123", (char *)"52");
//	
//	wlora.radio.set(mod, lora);
////	wlora.radio.set(freq, (char *)"865295000");
//	wlora.radio.set(freq, (char *)"867500000");
////	wlora.radio.set(freq, (char *)"868500000");
//	wlora.radio.set(pwr, (char *)"14");
//	wlora.radio.set(sf, (char *)"sf12");
//	wlora.radio.set(afcbw, (char *)"125");
//	wlora.radio.set(rxbw, (char *)"125");
//	wlora.radio.set(fdev, (char *)"5000");
//	wlora.radio.set(prlen, (char *)"8");
//	wlora.radio.set(crc, on);
//	wlora.radio.set(cr, (char *)"4/7");
//	wlora.radio.set(wdt, (char *)"0");
//	wlora.radio.set(sync, (char *)"12");
//	wlora.radio.set(bw, (char *)"125");
		
//	wlora.mac.pause();
	while (1) {
		wlora.mac.tx(cnf, (char *)"214", (char *)iotBuffer);
		osDelay(60000);
		adcFunc();
	}
}
osThreadDef(LoraTransmit, osPriorityNormal, 1, 0);

void LoraReceive(void const *arg) {
	LoRa wlora;
//	wlora.mac.tx(uncnf, (char *)"1");
	
	wlora.radio.set(mod, lora);
//	wlora.radio.set(freq, (char *)"865295000");
	wlora.radio.set(freq, (char *)"867500000");
//	wlora.radio.set(freq, (char *)"868500000");
	wlora.radio.set(pwr, (char *)"14");
	wlora.radio.set(sf, (char *)"sf12");
	wlora.radio.set(afcbw, (char *)"125");
	wlora.radio.set(rxbw, (char *)"125");
	wlora.radio.set(fdev, (char *)"5000");
	wlora.radio.set(prlen, (char *)"8");
	wlora.radio.set(crc, on);
	wlora.radio.set(cr, (char *)"4/7");
	wlora.radio.set(wdt, (char *)"0");
	wlora.radio.set(sync, (char *)"12");
	wlora.radio.set(bw, (char *)"250");
	
	wlora.mac.pause();
	
	while (1) {
		wlora.radio.rx((char *)"0");
		osDelay(1000);
		uartSend(uart232, (char *)"snr: ");
		wlora.radio.get(snr);
		osDelay(1000);
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
	
	osThreadCreate(osThread(LoraTransmit), NULL);
//	osThreadCreate(osThread(LoraReceive), NULL);
	
	osKernelStart();
	
	return 0;
}

#ifndef _LoRa_H_
#define _LoRa_H_

#include <stdint.h>
#include <string.h>
#include "em_usart.h"
#include "RingBuffer.h"
#include "cmsis_os.h"
#include "em_cmu.h"
#include "em_gpio.h"

#define band (char *) "band"
#define deveui (char *) "deveui"
#define appeui (char *) "appeui"
#define appkey (char *) "appkey"
#define nwkskey (char *) "nwkskey"
#define appskey (char *) "appskey"
#define devaddr (char *) "devaddr"
#define freq (char *) "freq"
#define dcycle (char *) "dcycle"
#define drrange (char *) "drrange"
#define status (char *) "status"
#define pwridx (char *) "pwridx"
#define dr (char *) "dr"
#define adr (char *) "adr"
#define bat (char *) "bat"
#define retx (char *) "retx"
#define linkchk (char *) "linkchk"
#define rxdelay1 (char *) "rxdelay1"
#define rxdelay2 (char *) "rxdelay2"
#define ar (char *) "ar"
#define rx1 (char *) "rx1"
#define rx2 (char *) "rx2"
#define ch (char *) "ch"
#define bt (char *) "bt"
#define mod (char *) "mod"
#define freq (char *) "freq"
#define pwr (char *) "pwr"
#define sf (char *) "sf"
#define afcbw (char *) "afcbw"
#define rxbw (char *) "rxbw"
#define bitrate (char *) "bitrate"
#define fdev (char *) "fdev"
#define prlen (char *) "prlen"
#define crc (char *) "crc"
#define iqi (char *) "iqi"
#define cr (char *) "cr"
#define wdt (char *) "wdt"
#define sync (char *) "sync"
#define bw (char *) "bw"
#define snr (char *) "snr"
#define dcycleps (char *) "dcycleps"
#define mrgn (char *) "mrgn"
#define gwnb (char *) "gwnb"
#define vdd (char *) "vdd"
#define hweui (char *) "hweui"
#define cnf (char *) "cnf"
#define on (char *) "on"
#define off (char *) "off"
#define otaa (char *) "otaa"
#define abp (char *) "abp"
#define none (char *) "none"
#define lora (char *) "lora"
#define fsk (char *) "fsk"
#define uncnf (char *) "uncnf"

#define DEVADDR (char *) "D43EBB86"
#define DEVADDR2 (char *) "B443B404"
#define DEVEUI (char *) "A4D894B00B1E5096"
#define DEVEUI2 (char *) "D4B894B0A11E5043"
#define APPEUI (char *) "B12498D9428361AE"
#define APPEUI2 (char *) "D13498D942838451"
#define APPKEY (char *) "921846A329B4C13D3041281942183627"
#define APPKEY2 (char *) "821D46A329B4C13D3041281942183479"
#define NWKSKEY (char *) "2B76D5DDA85A891ECCAFB3ABCAD2F254"
#define NWKSKEY2 (char *) "0B76D5DDA85A891ECCAFB3ABCAD47256"
#define APPSKEY (char *) "E536195F254E138C03D05FFE9CBF7A39"
#define APPSKEY2 (char *) "A616195F254E138C03D05FFE9CBF9725"

typedef enum _LORA_COMMAND_RESPONSE {
	LORA_RESPONSE_OK,
	LORA_RESPONSE_ERROR,
	LORA_RESPONSE_INVALID
} LORA_COMMAND_RESPONSE;

class LoRa {
		
	private:
		static char* stringConcatenate(char first[], char second[]);
		static char* stringConcatenate(char first[], char second[], char third[]);
		static char* stringConcatenate(char first[], char second[], char third[], char fourth[]);
		static char* stringConcatenate(char first[], char second[], char third[], char fourth[], char fifth[]);
		static char* stringConcatenate(char first[], char second[], char third[], char fourth[], char fifth[],char sixth[]);
	
	public:
		LoRa();
		void uartLoraInitialize();
	
		struct sys {
			LORA_COMMAND_RESPONSE sleep(char value[]);
			LORA_COMMAND_RESPONSE reset(void);
			LORA_COMMAND_RESPONSE eraseFW(void);
			LORA_COMMAND_RESPONSE factoryRESET(void);
			LORA_COMMAND_RESPONSE get(char parameter[]);
			LORA_COMMAND_RESPONSE get(char parameter[], char parameter2[]);
			LORA_COMMAND_RESPONSE set(char parameter[], char value[]);
			LORA_COMMAND_RESPONSE set(char parameter[], char value[], char value2[]);
			LORA_COMMAND_RESPONSE pingdig(char pinname[], char pinstate[]);
		} sys;
		
		struct mac {
			LORA_COMMAND_RESPONSE reset(char parameter[]);
			LORA_COMMAND_RESPONSE tx(char parameter[], char value[]);
			LORA_COMMAND_RESPONSE tx(char parameter[], char value[], char value2[]);
			LORA_COMMAND_RESPONSE rx(char parameter[], char value[]);
			LORA_COMMAND_RESPONSE join(char parameter[]);
			LORA_COMMAND_RESPONSE save(void);
			LORA_COMMAND_RESPONSE forceENABLE(void);
			LORA_COMMAND_RESPONSE pause(void);
			LORA_COMMAND_RESPONSE resume(void);
			LORA_COMMAND_RESPONSE get(char parameter[]);
			LORA_COMMAND_RESPONSE get(char parameter[] , char parameter2[]);
			LORA_COMMAND_RESPONSE get(char parameter[] , char parameter2[], char parameter3[]);
			LORA_COMMAND_RESPONSE set(char parameter[], char value[]);
			LORA_COMMAND_RESPONSE set(char parameter[], char value[], char value2[]);
			LORA_COMMAND_RESPONSE set(char parameter[], char value[], char value2[], char value3[]);
			LORA_COMMAND_RESPONSE set(char parameter[], char value[], char value2[], char value3[], char value4[]);
		} mac;
		
		struct radio {
			LORA_COMMAND_RESPONSE rx(char value[]);
			LORA_COMMAND_RESPONSE tx(char value[]);
			LORA_COMMAND_RESPONSE cw(char value[]);
			LORA_COMMAND_RESPONSE get(char parameter[]);
			LORA_COMMAND_RESPONSE get(char parameter[], char parameter2[]);
			LORA_COMMAND_RESPONSE set(char parameter[], char value[]);
			LORA_COMMAND_RESPONSE set(char parameter[], char value[], char value2[]);
		} radio;
		
		static void uartLoraSend(USART_TypeDef *tempType, char data[]);
};

#endif

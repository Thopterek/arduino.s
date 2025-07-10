#include "stdint.h"

/*
	Parts that being reused a lot
	called fully on the 3rd variable
*/
#define PD 0x25
#define PB 0x28
#define PC 0x28
#define __ATTR_PROGMEM__ __attribute__((__progmem__))
#define PROGMEM __ATTR_PROGMEM__
/*
	first variable that is being set
	uint8_t timer is equal to below
	the passed P should be beginning var
*/
#define digitalPinToTimer(P) ( pgm_read_byte( digital_pin_to_timer_PGM + (P) ) )
/*
	calling then in chain corresponding definitions below:
	hopefully those are the correct definitions (needs test)
	should go through them depending on the board type (not sure)
*/
#define NOT_ON_TIMER 0
#define TIMER2B 8
#define TIMER0B 2
#define TIMER0A 1
#define TIMER1A 3
#define TIMER1B 4
#define TIMER2  6
#define TIMER2A 7
/*
	definition of values used below
	hopefully thats all of them
	and in order of the usage
*/
const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER, /* 0 - port D */
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	// on the ATmega168, digital pin 3 has hardware pwm
#if defined(__AVR_ATmega8__)
	NOT_ON_TIMER,
#else
	TIMER2B,
#endif
	NOT_ON_TIMER,
	// on the ATmega168, digital pins 5 and 6 have hardware pwm
#if defined(__AVR_ATmega8__)
	NOT_ON_TIMER,
	NOT_ON_TIMER,
#else
	TIMER0B,
	TIMER0A,
#endif
	NOT_ON_TIMER,
	NOT_ON_TIMER, /* 8 - port B */
	TIMER1A,
	TIMER1B,
#if defined(__AVR_ATmega8__)
	TIMER2,
#else
	TIMER2A,
#endif
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER, /* 14 - port C */
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
};
/*
	used address short is 16 bit
	its the pin number used
*/
#define pgm_read_byte(address_short)    pgm_read_byte_near(address_short)
#define pgm_read_byte_near(address_short) __LPM((uint16_t)(address_short))
#define __LPM(addr)         __LPM_classic__(addr)
#define __LPM_classic__(addr)   \
(__extension__({                \
    uint16_t __addr16 = (uint16_t)(addr); \
    uint8_t __result;           \
    __asm__ __volatile__        \
    (                           \
        "lpm" "\n\t"            \
        "mov %0, r0" "\n\t"     \
        : "=r" (__result)       \
        : "z" (__addr16)        \
        : "r0"                  \
    );                          \
    __result;                   \
}))

/*
	second variable bit, repeat the above logic:
	uses the macro for bit shifting
*/
#define _BV(bit) (1 << (bit))
/*
	putting out the chain of definitions
	starting with pre definitions:
	digital_pit_to_bit_mask_PGM
*/
const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(0), /* 0, port D */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
	_BV(0), /* 8, port B */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(0), /* 14, port C */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
};
/*
	it follows chain of definitions from the above one
	it tries to use the definition of __LPM(addr)
	problem is that I only found the __LPM_classic__(addr)
	that I passed in definition mentions before
*/
#define digitalPinToBitMask(P) ( pgm_read_byte( digital_pin_to_bit_mask_PGM + (P) ) )

/*
	third variable port using as the ones before the pin
	starting with predefinition the pgm_read_byte stays the same
	now adding the digital_pin_port_PGM parts those added at top
*/
const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PD, /* 0 */
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,
	PB, /* 8 */
	PB,
	PB,
	PB,
	PB,
	PB,
	PC, /* 14 */
	PC,
	PC,
	PC,
	PC,
	PC,
};
/*
	set of defintions for the PD PB PC I still didn't find
	at the end we are going past setting up the variables
*/
#define digitalPinToPort(P) ( pgm_read_byte( digital_pin_to_port_PGM + (P) ) )

/*
	starting with if statements and switch statement
	that is being called in one of them uses cbi
	the _BV was already used before in another instance
*/
#define _SFR_MEM_ADDR(sfr) ((uint16_t) &(sfr))
#define _SFR_ADDR(sfr) _SFR_MEM_ADDR(sfr)
#define _MMIO_BYTE(mem_addr) (*(volatile uint8_t *)(mem_addr))
#define _SFR_BYTE(sfr) _MMIO_BYTE(_SFR_ADDR(sfr))
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
/*
	now going for a shorter banch
	the ones used in the if statements
	and horrible switch statement
*/
#define NOT_A_PIN 0
#define NOT_ON_TIMER 0
static void turnOffPWM(uint8_t timer)
{
	switch (timer)
	{
		#if defined(TCCR1A) && defined(COM1A1)
		case TIMER1A:   cbi(TCCR1A, COM1A1);    break;
		#endif
		#if defined(TCCR1A) && defined(COM1B1)
		case TIMER1B:   cbi(TCCR1A, COM1B1);    break;
		#endif
		#if defined(TCCR1A) && defined(COM1C1)
		case TIMER1C:   cbi(TCCR1A, COM1C1);    break;
		#endif
		
		#if defined(TCCR2) && defined(COM21)
		case  TIMER2:   cbi(TCCR2, COM21);      break;
		#endif
		
		#if defined(TCCR0A) && defined(COM0A1)
		case  TIMER0A:  cbi(TCCR0A, COM0A1);    break;
		#endif
		
		#if defined(TCCR0A) && defined(COM0B1)
		case  TIMER0B:  cbi(TCCR0A, COM0B1);    break;
		#endif
		#if defined(TCCR2A) && defined(COM2A1)
		case  TIMER2A:  cbi(TCCR2A, COM2A1);    break;
		#endif
		#if defined(TCCR2A) && defined(COM2B1)
		case  TIMER2B:  cbi(TCCR2A, COM2B1);    break;
		#endif
		
		#if defined(TCCR3A) && defined(COM3A1)
		case  TIMER3A:  cbi(TCCR3A, COM3A1);    break;
		#endif
		#if defined(TCCR3A) && defined(COM3B1)
		case  TIMER3B:  cbi(TCCR3A, COM3B1);    break;
		#endif
		#if defined(TCCR3A) && defined(COM3C1)
		case  TIMER3C:  cbi(TCCR3A, COM3C1);    break;
		#endif

		#if defined(TCCR4A) && defined(COM4A1)
		case  TIMER4A:  cbi(TCCR4A, COM4A1);    break;
		#endif					
		#if defined(TCCR4A) && defined(COM4B1)
		case  TIMER4B:  cbi(TCCR4A, COM4B1);    break;
		#endif
		#if defined(TCCR4A) && defined(COM4C1)
		case  TIMER4C:  cbi(TCCR4A, COM4C1);    break;
		#endif			
		#if defined(TCCR4C) && defined(COM4D1)
		case TIMER4D:	cbi(TCCR4C, COM4D1);	break;
		#endif			
			
		#if defined(TCCR5A)
		case  TIMER5A:  cbi(TCCR5A, COM5A1);    break;
		case  TIMER5B:  cbi(TCCR5A, COM5B1);    break;
		case  TIMER5C:  cbi(TCCR5A, COM5C1);    break;
		#endif
	}
}

/*
	Starting with putting the code for
	portOutputRegister(port) the
	port_to_output_PGM used by it
*/
extern const uint16_t PROGMEM port_to_output_PGM[];
/*
	LPM that is being used by it
	port_to_out_PGM would need to be recreated
	or its something hides from me the logic for it
*/
#define __LPM_word_classic__(addr)          \
(__extension__({                            \
    uint16_t __addr16 = (uint16_t)(addr);   \
    uint16_t __result;                      \
    __asm__ __volatile__                    \
    (                                       \
        "lpm"           "\n\t"              \
        "mov %A0, r0"   "\n\t"              \
        "adiw r30, 1"   "\n\t"              \
        "lpm"           "\n\t"              \
        "mov %B0, r0"   "\n\t"              \
        : "=r" (__result), "=z" (__addr16)  \
        : "1" (__addr16)                    \
        : "r0"                              \
    );                                      \
    __result;                               \
}))
/*
	and the definitions chain that gets to it
	and is being called up with the ft
*/
#define __LPM_word(addr)    __LPM_word_classic__(addr)
#define pgm_read_word_near(address_short) __LPM_word((uint16_t)(address_short))
#define pgm_read_word(address_short)    pgm_read_word_near(address_short)
#define portOutputRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_output_PGM + (P))) )

/*
	setting the value of oldSREG
	by the usage of SREG, _MMIO_BYTE was already used
	only adding the offset value  rest stay
*/
#ifndef __SFR_OFFSET
#  if __AVR_ARCH__ >= 100
#    define __SFR_OFFSET 0x00
#  else
#    define __SFR_OFFSET 0x20
#  endif
#endif
#define _SFR_IO8(io_addr) _MMIO_BYTE((io_addr) + __SFR_OFFSET)
#define _SFR_MEM8(mem_addr) _MMIO_BYTE(mem_addr)
/*
	I am missing the setting of __AVR_ARCH__
	value where it is happenig
*/
#ifndef SREG
#  if __AVR_ARCH__ >= 100
#    define SREG _SFR_MEM8(0x3F)
#  else
#    define SREG _SFR_IO8(0x3F)
#  endif
#endif

/*
	cli that is being used afterwards
	hopefully deep diving is gonna show why
	for now it just keeps the __asm__
*/
#define cli()  __asm__ __volatile__ ("cli" ::: "memory")

/*
	some simple LOW, just like HIGH
	that we trying to set
*/
#define LOW  0x0

/*
	for the Arduino uno the base pin values is 13
	value of val (high) is set as 0x1
	value of val (low) is set as 0x0
*/
void digitalWrite(uint8_t pin, uint8_t val)
{
	uint8_t timer = digitalPinToTimer(pin);
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	volatile uint8_t *out;

	if (port == NOT_A_PIN) return;

	// If the pin that support PWM output, we need to turn it off
	// before doing a digital write.
	if (timer != NOT_ON_TIMER) turnOffPWM(timer);

	out = portOutputRegister(port);

	uint8_t oldSREG = SREG;
	cli();

	if (val == LOW) {
		*out &= ~bit;
	} else {
		*out |= bit;
	}

	SREG = oldSREG;
}
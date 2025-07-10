#include "stdint.h"
#include "digitalWrite.c"

/*
	pre defintion being called by micros
	the helper function for delay
	_MMIO_BYTE and offset in digitalWrite
*/
volatile unsigned long timer0_overflow_count = 0;
#define _SFR_IO8(io_addr) _MMIO_BYTE((io_addr) + __SFR_OFFSET)
#define TCNT0 _SFR_IO8(0x26)
#define TIFR0 _SFR_IO8(0x15)
#define TOV0 0
/*
	the defintion for the clockCyclesPerMicrosends
	and all of the chain of definition related to it
*/
#define F_CPU 16000000L
#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
/*
	setting up the start variable
	main caller for the helpers
	some of things like SREG / cli()
	used in the digitalWrite
*/
unsigned long micros() {
	unsigned long m;
	uint8_t oldSREG = SREG, t;
	
	cli();
	m = timer0_overflow_count;
#if defined(TCNT0)
	t = TCNT0;
#elif defined(TCNT0L)
	t = TCNT0L;
#else
	#error TIMER 0 not defined
#endif

#ifdef TIFR0
	if ((TIFR0 & _BV(TOV0)) && (t < 255))
		m++;
#else
	if ((TIFR & _BV(TOV0)) && (t < 255))
		m++;
#endif

	SREG = oldSREG;
	
	return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
}

/*
	yield as a function for library writers
	to support the cooperative threads
	weak to be changed later on
*/
void yield(void) __attribute__ ((weak, alias("__empty")));

/*
	delay function being called for blinking effect
	the timer function thats bettwen digitalWrite calls
*/
void delay(unsigned long ms)
{
	uint32_t start = micros();

	while (ms > 0) {
		yield();
		while ( ms > 0 && (micros() - start) >= 1000) {
			ms--;
			start += 1000;
		}
	}
}

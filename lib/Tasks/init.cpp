#include <stdint.h>
#include <setjmp.h>
#include "task.h"

#if defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)

void task::create(void *stack) {
	setjmp(_context);
	unsigned sp = (unsigned)stack, e = (unsigned)entry;
	_context[0]._jb[_JBLEN-4] = (sp >> 8);
	_context[0]._jb[_JBLEN-5] = (sp & 0xff);
	_context[0]._jb[_JBLEN-1] = (e >> 8);
	_context[0]._jb[_JBLEN-2] = (e & 0xff);
}

#elif defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny45__) \
 ||   defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny44__)

void task::create(void *stack) {
	setjmp(_context);
	unsigned sp = (unsigned)stack, e = (unsigned)entry;
	_context[0]._jb[_JBLEN-4] = (sp >> 8);
	_context[0]._jb[_JBLEN-5] = (sp & 0xff);
	_context[0]._jb[_JBLEN-1] = (e >> 8);
	_context[0]._jb[_JBLEN-2] = (e & 0xff);
}

#elif defined(__MSP430FR5739__) || defined(__MSP430G2553__)

void task::create(void *stack) {
	setjmp(_context);
	
	// jmpbuf register names are inconsistent with setjmp/longjmp
	_context[0].__j_r11 = (uint32_t)entry;
	_context[0].__j_pc = (uint32_t)stack;
}

#else
#error "Unsupported architecture!"
#endif

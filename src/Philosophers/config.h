#if defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)

#define STACK	100
#define MAX	5
#define LED1	8
#define LED2	9
#define LED3	10
#define LED4	11
#define LED5	12

#elif defined(__MSP430FR5739__)

//#define SERIAL 9600
#define STACK	100
#define MAX	4

#else

#error "Unsupported architecture!"

#endif

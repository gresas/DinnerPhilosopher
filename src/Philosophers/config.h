#if defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)

#define STACK	100
#define MAX	5
#define LED1	6
#define LED2	7
#define LED3	8
#define LED4	9
#define LED5	10

#elif defined(__MSP430FR5739__)

//#define SERIAL 9600
#define STACK	100
#define MAX	4

#else

#error "Unsupported architecture!"

#endif

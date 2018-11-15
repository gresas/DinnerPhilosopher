#include "config.h"
#include <Tasks.h>
#include <Interrupted.h>
#include <LiquidCrystal.h>
 
//Define os pinos que serão utilizados para ligação ao display
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

Semaphore forks[MAX];
static byte ids = 1;
static byte leds[] = { LED1, LED2, LED3, LED4, LED5 };

void lcdPrint(){
  char buff[16];
  lcd.setCursor(3, 0);
  sprintf(buff, "|%u|%u|%u|%u|%u|", forks[0]._count, forks[1]._count, forks[2]._count, forks[3]._count, forks[4]._count);
  lcd.print(buff);
}

void eating(byte id, byte weight) {

	digitalWrite(leds[id], LOW);

	Tasks::delay(MAX * random(101, 501));
}

void thinking(byte id, byte weight) {

  digitalWrite(leds[id], HIGH);

  Tasks::delay(MAX * random(101, 501));
}

class Philosopher: public Task<50> {
public:
	void setup() {
		_id = ids++;
	}

	void loop() {
    lcdPrint();
    thinking(_id, _w++);
  
		forks[_id - 1].wait();
		forks[_id].wait();

		eating(_id, _w++);

		forks[_id - 1].signal();
		forks[_id].signal();
	}

private:
	byte _id, _w;
};

Philosopher philosophers[MAX-1];

void setup() {

  lcd.begin(16, 2);

	Tasks::init();
	Tasks::set_idle_handler(timer_sleep);
	for (int i = 0; i < MAX; i++) {
		forks[i].signal();
		pinMode(leds[i], OUTPUT);
	}
	for (int i = 0; i < MAX-1; i++)
		Tasks::start(philosophers[i]);

  //Serial.begin(9600);
}

void loop() {
  lcdPrint();

	static byte weight;
  thinking(0, weight++);
  
  forks[0].wait();
  forks[MAX-1].wait();

  eating(0, weight++);

  forks[0].signal();
  forks[MAX-1].signal();
}

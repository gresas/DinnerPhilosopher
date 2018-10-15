#include "config.h"
#include <Tasks.h>

#include <Interrupted.h>

Semaphore forks[MAX];
static byte ids = 1;
static byte leds[] = { LED1, LED2, LED3, LED4, LED5 };

void eating(byte id, byte weight) {

	digitalWrite(leds[id], LOW);

	Tasks::delay(MAX * random(101, 301));

	//digitalWrite(leds[id], LOW);
}

void thinking(byte id, byte weight) {

  digitalWrite(leds[id], HIGH);

  Tasks::delay(MAX * random(101, 301));

  // digitalWrite(leds[id], LOW);
}

class Philosopher: public Task<50> {
public:
	void setup() {
		_id = ids++;
	}

	void loop() {
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

	Tasks::init();
	Tasks::set_idle_handler(timer_sleep);
	for (int i = 0; i < MAX; i++) {
		forks[i].signal();
		pinMode(leds[i], OUTPUT);
	}
	for (int i = 0; i < MAX-1; i++)
		Tasks::start(philosophers[i]);
}

void loop() {
	static byte weight;
  thinking(0, weight++);
  
  forks[0].wait();
  forks[MAX-1].wait();

  eating(0, weight++);

  forks[0].signal();
  forks[MAX-1].signal();
}

#include "config.h"
#include <Tasks.h>

#include <Interrupted.h>

#include <LiquidCrystal.h>
 
//Define os pinos que serão utilizados para ligação ao display
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

Semaphore forks[MAX];
static byte ids = 1;
static byte leds[] = { LED1, LED2, LED3, LED4, LED5 };
static byte buttonPin = 13;
static bool state = false;
static int vect[] = {1, 1, 1, 1, 1};


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
    vect[_id]--;
    vect[_id - 1]--;
    lcd.setCursor(2, 0);
    char buff[12];
    sprintf(buff, "|%d|%d|%d|%d|%d|", vect[0], vect[1], vect[2], vect[3], vect[4]);
    lcd.print(buff);
    
    eating(_id, _w++);

    forks[_id - 1].signal();
    forks[_id].signal();  
    vect[_id]++;
    vect[_id - 1]++;
    lcd.setCursor(2, 0);
    sprintf(buff, "|%d|%d|%d|%d|%d|", vect[0], vect[1], vect[2], vect[3], vect[4]);
    lcd.print(buff);
	}

private:
	byte _id, _w;
};

Philosopher philosophers[MAX-1];

void setup() {
  
  //Define o número de colunas e linhas do LCD
  lcd.begin(16, 2);
  
	Tasks::init();
	Tasks::set_idle_handler(timer_sleep);
	for (int i = 0; i < MAX; i++) {
		forks[i].signal();
		pinMode(leds[i], OUTPUT);
	}
	for (int i = 0; i < MAX-1; i++)
		Tasks::start(philosophers[i]);

  lcd.setCursor(2, 0);
  char buff[12];
  sprintf(buff, "|%d|%d|%d|%d|%d|", vect[0], vect[1], vect[2], vect[3], vect[4]);
  lcd.print(buff);
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

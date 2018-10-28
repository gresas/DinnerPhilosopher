#include "config.h"
#include <Tasks.h>
#include <Interrupted.h>
#include <LiquidCrystal.h>

static byte buttonPin = 13;
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 300;    // the debounce time; increase if the output flickers
 
//Define os pinos que serão utilizados para ligação ao display
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

Semaphore forks[MAX];
static byte ids = 1;
static byte leds[] = { LED1, LED2, LED3, LED4, LED5 };

void lcdPrint(){
  lcd.setCursor(2, 0);
  char buff[12];
  sprintf(buff, "|%u|%u|%u|%u|%u|", forks[0]._count, forks[1]._count, forks[2]._count, forks[3]._count, forks[4]._count);
  lcd.print(buff);
}

void serialPrint(int _id){
  
  char buff[16];
  sprintf(buff, "|%u|%u|%u|%u|%u| - %d", forks[0]._count, forks[1]._count, forks[2]._count, forks[3]._count, forks[4]._count, _id);
  Serial.println(buff);
}

void eating(byte id, byte weight) {
  Serial.print(id);
  Serial.println(" is eating");
	digitalWrite(leds[id], LOW);
  lcdPrint();

  Tasks::delay(MAX * random(101, 501));
	//digitalWrite(leds[id], LOW);
}

void thinking(byte id, byte weight) {
  Serial.print(id);
  Serial.println(" is thinking");
  digitalWrite(leds[id], HIGH);
  Tasks::delay(MAX * random(101, 501));
  // digitalWrite(leds[id], LOW);
}

class Philosopher: public Task<50> {
public:
	void setup() {
		_id = ids++;
	}

	void loop() {    
    thinking(_id, _w++);

    // Serial.print(_id);
    // Serial.println(" <----------- Left Fork\n");
		forks[_id - 1].wait();
    
    // serialPrint(_id);
		// Serial.println(" <----------- Right Fork\n");
		forks[_id].wait();
    serialPrint(_id);
    
		eating(_id, _w++);

    // Serial.print(_id);
    // Serial.println(" -----------> Right Fork\n");
		forks[_id].signal();
		serialPrint(_id);
    // Serial.print(_id);
    // Serial.println(" -----------> Left Fork\n");
		forks[_id - 1].signal();
    serialPrint(_id);	
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
  Serial.begin(9600);
}

void loop() {

	static byte weight;
  thinking(0, weight++);
  
  forks[0].wait();
  serialPrint(0);
  forks[MAX-1].wait();
  serialPrint(0);
  
  eating(0, weight++);

  forks[0].signal();
  serialPrint(0);
  forks[MAX-1].signal();
  serialPrint(0);
}

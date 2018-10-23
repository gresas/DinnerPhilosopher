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
static bool state = false;
static int vect[] = {1, 1, 1, 1, 1};

void lcdPrint(){
  lcd.setCursor(2, 0);
  char buff[12];
  sprintf(buff, "|%u|%u|%u|%u|%u|", forks[0]._count, forks[1]._count, forks[2]._count, forks[3]._count, forks[4]._count);
  lcd.print(buff);
}

void eating(byte id, byte weight) {
  
	digitalWrite(leds[id], LOW);
	Tasks::delay(MAX * random(101, 501));

	//digitalWrite(leds[id], LOW);
}

void thinking(byte id, byte weight) {

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
  int reading = digitalRead(buttonPin);
  
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        thinking(_id, _w++);
        
        forks[_id - 1].wait();
        lcdPrint();
        forks[_id].wait();
        lcdPrint();
        
        eating(_id, _w++);
        
        forks[_id - 1].signal();
        lcdPrint();
        forks[_id].signal();  
        lcdPrint();
      }
    }
  }
  lastButtonState = reading;
  lcdPrint();
}

private:
	byte _id, _w;
};

Philosopher philosophers[MAX-1];

void setup() {
  
  //Define o número de colunas e linhas do LCD
  lcd.begin(16, 2);
  pinMode(buttonPin, INPUT);
  
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
  lcdPrint();
  forks[MAX-1].wait();
  lcdPrint();
  
  eating(0, weight++);

  forks[0].signal();
  lcdPrint();
  forks[MAX-1].signal();  
  lcdPrint();
}

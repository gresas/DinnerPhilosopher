#include <stdint.h>
#include <setjmp.h>
#include <Arduino.h>

#include "task.h"
#include "sem.h"
#include "atomic.h"

void Semaphore::wait(byte id) {
	Atomic block;
	if (_count == 0) {
		Serial.print(id);
		Serial.println(" - blocked");
		_waiting.add(Tasks::current());
		Tasks::reschedule();
	} else
		_count--;
	
	// return _count;
}

void Semaphore::signal(byte id) {
	Atomic block;
	if (_waiting.empty())
		_count++;
	else{
		Serial.print(id);
		Serial.println(" - unblocked");
		Tasks::ready(_waiting.remove());
	}
	// return _count;
}


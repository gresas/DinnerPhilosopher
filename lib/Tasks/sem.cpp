#include <stdint.h>
#include <setjmp.h>
#include <Arduino.h>

#include "task.h"
#include "sem.h"
#include "atomic.h"

void Semaphore::wait() {
	Atomic block;
	if (_count == 0) {
		_waiting.add(Tasks::current());
		Tasks::reschedule();
	} else
		_count--;
}

void Semaphore::signal() {
	Atomic block;
	if (_waiting.empty())
		_count++;
	else
		Tasks::ready(_waiting.remove());
}


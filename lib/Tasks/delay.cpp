#include <Arduino.h>
#include <stdint.h>
#include <setjmp.h>
#include <atomic.h>
#include "task.h"

static task_queue delaying;

void task_queue::insert(task *p, task *t) {
	if (p) {
		t->next = p->next;
		p->next = t;
		if (p == _tail)
			_tail = t;
	} else {
		t->next = _head;
		_head = t;
	}
}

#define MAX_DELAY       0xf000000

void Tasks::delay(unsigned long ms) {
	unsigned long now = millis();
	Atomic block;
	task *p, *q;
	for (p = 0, q = delaying.head(); q; p = q, q = q->next) {
		unsigned long qms = q->wake - now;
		if (qms >= MAX_DELAY) {
			Tasks::ready(delaying.remove());
			q = delaying.head();
			continue;
		}
		if (qms > ms)
			break;
	}
	task *t = Tasks::current();
	t->wake = now + ms;
	delaying.insert(p, t);
	Tasks::reschedule();
}

void timer_sleep() {
	task *t = delaying.remove();
	interrupts();
	unsigned long ms = t->wake - millis();
	if (ms < MAX_DELAY)
		delay(ms);
	noInterrupts();
	Tasks::ready(t);
}

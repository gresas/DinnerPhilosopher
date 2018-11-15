#include <stdint.h>
#include <setjmp.h>
#include "task.h"

task *task_queue::remove() {			// Remove a "cabeça" da fila de prontos Tasks::_ready
	if (empty())
		return 0;
	task *t = _head;			 
	_head = t->next;			  
	if (empty())				  
		_tail = 0;					
	return t;
}

void task_queue::add(task *t){				// Adiciona tarefa na fila de prontos Tasks::_ready
	if (_tail)
		_tail->next = t;			
	else
		_head = t;
	_tail = t;
	t->next = 0;
}

task *Tasks::_curr;
task_queue Tasks::_ready;
runnable Tasks::_idle_handler;

void Tasks::reschedule(void) {
	task * volatile run;
	while (!(run = _ready.remove()))
		_idle_handler();				// Execução do "timer_spleep();" e bloqueio de cada tarefa na fila de ready

	if (run != _curr)
		if (_curr->save() == 0) {
			_curr = run;
			_curr->restore();
		}
}

void Tasks::init(void) {			// Inicia Tasks::_curr com endereçamento de 'static task'
	static task main;				
	_curr = &main;
}

void task::entry() {
	task * volatile t = Tasks::current();
	t->setup();
	for (;;)
		t->loop();
}

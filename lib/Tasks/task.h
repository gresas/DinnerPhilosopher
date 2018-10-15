#ifndef __TASK_H__
#define __TASK_H__

typedef void (*runnable)(void);
void timer_sleep(void);

class task {
public:
	// FIXME: clean up access permissions
	struct task *next;
	unsigned long wake;

	inline int save() { return setjmp(_context); }
	inline void restore() { longjmp(_context, 1); }

	virtual void begin() {}
protected:
	virtual void setup() {}
	virtual void loop() {}

	void create(void *stack);
private:
	static void entry();
	jmp_buf _context;
};

template<unsigned WORDS>
class Task: public task {
public:
	void begin() {
		create(&_stack[WORDS-1]);
	}

private:
	unsigned _stack[WORDS];
};

class task_queue {
public:
	task_queue() { _head = _tail = 0; }

	task *remove();

	void add(task *t);

	void insert(task *p, task *t);

	inline bool empty() { return _head == 0; }

	inline task *head() { return _head; }
private:
	task *_head, *_tail;
};

class Tasks {
public:
	/*
	 * initialises the task library: call from setup().
	 */
	static void init(void);

	/*
	 * makes a task runnable.
	 */
	static inline void ready(task *t) { _ready.add(t); }

	/*
	 * initialises a task and makes it runnable.
	 */
	static inline void start(task &t) { 
		t.begin();
		ready(&t); 
	}

	static inline task *current(void) { return _curr; }

	static void reschedule(void);

	static void delay(unsigned long ms);

	/*
	 * sets a handler to be called when no task is runnable,
	 *
	 * by default (on AVR at least) the processor will jump
	 * to the reset vector (at address 0x0000).
	 * this can cause confusing behaviour!
	 *
	 * if the handler does any I/O, it should enable interrupts
	 * first.
	 */
	static inline void set_idle_handler(runnable idle_handler) {
		_idle_handler = idle_handler;
	}
private:
	static task *_curr;
	static task_queue _ready;
	static runnable _idle_handler;
};

#endif

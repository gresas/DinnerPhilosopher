#ifndef __CHAN_H__
#define __CHAN_H__

template<class T> class Channel {
public:
	void out(T &t) {
		noInterrupts();
		if (_t)
			*_arrive() = t;
		else
			_wait(t);
		interrupts();
	}

	void in(T &t) {
		noInterrupts();
		if (_t)
			t = *_arrive();
		else
			_wait(t);
		interrupts();
	}

private:
	void _wait(T &t) {
		_t = &t;
		_waiting.add(Tasks::current());
		Tasks::reschedule();
	}

	T *_arrive() {
		T *t = _t;
		_t = 0;
		Tasks::ready(_waiting.remove());
		return t;
	}

	task_queue _waiting;
	T *_t;

};

#endif

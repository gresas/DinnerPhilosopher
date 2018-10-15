#ifndef __PSEUDO_H__
#define __PSEUDO_H__

#include <Device.h>

class Pseudo: public Device {
public:
	bool begin() { return true; }

	void ready() {
		Device::ready();
		if (is_enabled())
			TaskDevices::soft_irq();
	}

protected:
	Pseudo(int id): Device(id) {}

	void _enable(bool e) {}
};

// FIXME: move into own header
class Signal: public Pseudo {
public:
	Signal(int id): Pseudo(id) {}

	void send() { ready(); }
};

#endif

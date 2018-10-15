#ifndef __SEM_H__
#define __SEM_H__

class Semaphore {
public:
	/*
	 * initialises a (counting) semaphore.
	 */
	Semaphore(unsigned count): _count(count) {}

	Semaphore(): _count(0) {}

	/*
	 * if a task is waiting on the semaphore, releases it, otherwise
	 * increments its count.
 	 */
	void signal();

	/*
	 * if the semaphore's count is positive, decrements it and continues,
	 * otherwise blocks.
	 */
	void wait();

private:
	task_queue _waiting;
	unsigned _count;
};

#endif

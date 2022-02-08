#ifndef _EventQueue_H
#define _EventQueue_H
#define WIN32_LEAN_AND_MEAN
#include <mutex>
#include <queue>
#include <condition_variable>
#include "mail.h"

class EventQueue 
{
private:
	std::queue<Mail> queue;              // The queue itself
	mutable std::mutex mutex;            // Mutex: For allowing threads to access queue
	std::condition_variable condition;   // Block calling thread until notified
public:
	EventQueue() : queue(), mutex(), condition() {};
	~EventQueue() {};

	void enqueue(Mail& m)
	{
		// lock the mutex
		std::lock_guard<std::mutex> lock(mutex);

		// push the element to the queue
		queue.push(m);

		// unlock the thread
		condition.notify_one();

	}
	const Mail dequeue()
	{
		// lock the mutex
		std::unique_lock<std::mutex> lock(mutex);

		if (!queue.empty())
		{
			// remove the front message
			Mail message = queue.front();
			queue.pop();
			return message;
		}
		else
			// return an empty message
			return Mail();
	}
	const bool isEmpty()
	{
		return queue.empty();
	}
};
#endif

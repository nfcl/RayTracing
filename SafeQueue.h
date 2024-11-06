#ifndef _SAFEQUEUE_H_
#define _SAFEQUEUE_H_

#include <queue>
#include <mutex>

template <typename T>
class SafeQueue {
private:
	std::queue<T> mQueue;
	std::mutex mMutex;
public:
	SafeQueue() {}
	SafeQueue(SafeQueue&& other) {}
	~SafeQueue() {}
	bool empty() {
		std::unique_lock<std::mutex> lock(mMutex);
		return mQueue.empty();
	}
	int size() {
		std::unique_lock<std::mutex> lock(mMutex);
		return mQueue.size();
	}
	void enqueue(T& t) {
		std::unique_lock<std::mutex> lock(mMutex);
		mQueue.emplace(t);
	}
	bool dequeue(T& t) {
		std::unique_lock<std::mutex> lock(mMutex);
		if (mQueue.empty()) {
			return false;
		}
		t = std::move(mQueue.front());
		mQueue.pop();
		return true;
	}
};

#endif // !_SAFEQUEUE_H_
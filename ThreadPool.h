#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include "SafeQueue.h"
#include <functional>
#include <future>

class ThreadPool {
private:

	class ThreadWorker {
	private:
		int mId;
		ThreadPool* mPool;
	public:
		ThreadWorker(ThreadPool* pool, const int id) : mPool(pool), mId(id) {}
		void operator()() {
			std::function<void()> func;
			bool dequeued;
			while (!mPool->mShutdown) {
				{
					std::unique_lock<std::mutex> lock(mPool->mConditionalMutex);
					if (mPool->mQueue.empty()) {
						mPool->mConditionalLock.wait(lock);
					}
					dequeued = mPool->mQueue.dequeue(func);
				}
				if (dequeued) {
					func();
				}
			}
		}
	};

	bool mShutdown;
	SafeQueue<std::function<void()>> mQueue;
	std::vector<std::thread> mThreads;
	std::mutex mConditionalMutex;
	std::condition_variable mConditionalLock;
public:
	ThreadPool(const int nThreads = 4) : mThreads(std::vector<std::thread>(nThreads)), mShutdown(false) {}
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool(ThreadPool&&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
	ThreadPool& operator=(ThreadPool&&) = delete;
	void init() {
		for (int i = 0; i < mThreads.size(); ++i) {
			mThreads.at(i) = std::thread(ThreadWorker(this, i));
		}
	}
	void shutdown() {
		mShutdown = true;
		mConditionalLock.notify_all();
		for (int i = 0; i < mThreads.size(); ++i) {
			if (mThreads.at(i).joinable()) {
				mThreads.at(i).join();
			}
		}
	}
	template<typename F, typename... Args>
	auto submit(F&& f, Args&&...args) -> std::future<decltype(f(args...))> {
		std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
		auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);
		std::function<void()> warpper_func = [task_ptr]()
			{
				(*task_ptr)();
			};
		mQueue.enqueue(warpper_func);
		mConditionalLock.notify_one();
		return task_ptr->get_future();
	}
};

#endif // !_THREADPOOL_H_

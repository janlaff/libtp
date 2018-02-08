//
// Created by jan on 2/8/18.
//

#ifndef LIBTP_THREAD_POOL_H
#define LIBTP_THREAD_POOL_H

#include <queue>
#include <thread>
#include <atomic>

#include "task.h"
#include "lambda_task.h"

namespace tp {
	class thread_pool {
	public:
		explicit thread_pool(size_t num_threads = std::thread::hardware_concurrency());
		~thread_pool();

		void pause();
		void resume();
		void cancel();
		void wait();

		template<typename Task, typename ... Args>
		std::shared_ptr<Task> post(Args &&... args) {
			static_assert(is_task<Task>::value);
			auto task = std::make_shared<Task>(std::forward<Args>(args)...);

			std::unique_lock lock(task_mutex);
			tasks.push(task);
			lock.unlock();

			task_cv.notify_one();

			return task;
		}

		template<typename ... Args>
		std::shared_ptr<lambda_task> post(Args &&... args) {
			return post<lambda_task, Args...>(std::forward<Args>(args)...);
		}

	private:
		void clear_tasks();
		void init_threads();
		void join_threads();
		void task_worker();

		std::queue<std::shared_ptr<task>> tasks;
		std::vector<std::thread> threads;
		std::mutex task_mutex;
		std::condition_variable task_cv;

		const size_t num_threads;
		std::atomic<bool> quit;
		std::atomic<bool> paused;
	};
}

#endif //LIBTP_THREAD_POOL_H

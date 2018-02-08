//
// Created by jan on 2/8/18.
//

#ifndef LIBTP_TASK_H
#define LIBTP_TASK_H

#include <list>
#include <mutex>
#include <functional>
#include <condition_variable>

namespace tp {
	enum class task_status {
		queued, running, canceled, finished
	};

	class task {
	public:
		task_status get_status();

		void cancel();
		void run();
		bool wait();

		virtual void execute() = 0;

	private:
		std::mutex m;
		task_status status;
		std::condition_variable cv;
	};

	template<typename T, typename = void>
	struct is_task : std::false_type {
	};

	template<typename T>
	struct is_task<T, std::void_t<
			decltype(std::declval<T>().get_status()),
			decltype(std::declval<T>().run()),
			decltype(std::declval<T>().cancel()),
			decltype(std::declval<T>().wait()),
			decltype(std::declval<T>().execute())
	>> : std::true_type {
	};
}

#endif //LIBTP_TASK_H

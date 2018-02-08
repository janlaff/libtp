//
// Created by jan on 2/8/18.
//

#ifndef LIBTP_FUTURE_TASK_H
#define LIBTP_FUTURE_TASK_H

#include <future>
#include <optional>

#include "task.h"

namespace tp {
	template<typename T>
	class future_task : public task {
	public:
		explicit future_task(std::function<T()> fn);

		void execute() override;

		T get();

	private:
		std::function<T()> fn;
		std::promise<T> promise;
		std::future<T> future;
	};
}

#include "future_task.impl.h"

#endif //LIBTP_FUTURE_TASK_H

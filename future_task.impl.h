//
// Created by jan on 2/8/18.
//

#include "future_task.h"

namespace tp {
	template<typename T>
	future_task<T>::future_task(std::function<T()> fn)
			: fn(fn), future(promise.get_future()) {}

	template<typename T>
	void future_task<T>::execute() {
		promise.set_value(std::forward<T &&>(fn()));
	}

	template<typename T>
	T future_task<T>::get() {
		return std::move(future.get());
	}
}
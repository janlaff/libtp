# libtp

## Examples

```
#include <thread_pool.h>

int main() {
    tp::thread_pool pool;

    for (int i = 0; i < 10; i++) {
        pool.post([]() {
	    doCalcStuff();
	});
    }

    pool.wait();
}
```

### Example: Wait for task to finish

```
auto myTask = pool.post([]() { doCalcStuff(); });

myTask->wait(); // Blocks until myTask has finished
```

### Example: Get task result

```
auto myTask = pool.post<pt::future_task<std::string>>([]() {
    return "Result: " + calcStringValue();
});

// Blocks until result is available
std::cout << myTask->get() << std::endl;
```

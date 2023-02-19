#include <iostream>
#include <thread>
#include "safequeue.h"
#include "threadpool.h"

using namespace std::chrono_literals;

void func1() {
	std::cout << __FUNCTION__ << " " << std::this_thread::get_id() << std::endl;
}

void func2() {
	std::cout << __FUNCTION__ << " " << std::this_thread::get_id() << std::endl;
}


int main()
{
	Thread_pool<std::function<void()>> tp;
	for (int i = 0; i < 10; ++i) {
		tp.submit(func1);
		tp.submit(func2);
		std::this_thread::sleep_for(2000ms);
	}
}


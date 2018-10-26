#include "Utils.h"
#include "Pod.h"
#include "Event.hpp"
#include "Simulator.hpp"

#include <sched.h>
#include <chrono>

TEST(UtilsTest, busyWaitTestShort) {
	double start = std::clock();
	int time = 1000;
	Utils::busyWait(time);
	EXPECT_GE(std::clock(), start + time);
}

TEST(UtilsTest, busyWaitTestMed) {
	double start = std::clock();
	int time = 100000;
	Utils::busyWait(time);
	EXPECT_GE(std::clock(), start + time);
}

TEST(UtilsTest, busyWaitTestLong) {
	double start = std::clock();
	int time = 10000000;
	Utils::busyWait(time);
	EXPECT_GE(std::clock(), start + time);
}

TEST(UtilsTest, busyWaitThreadTest) {
	double start = std::clock();
	int time = 100000;
	struct timespec currTime;

	std::thread t([&](){ Utils::busyWait(time); ;
	clockid_t threadClockId;
	pthread_getcpuclockid(pthread_self(), &threadClockId);
	clock_gettime(threadClockId, &currTime); });

	
	t.join();
	EXPECT_GE(currTime.tv_sec * 1000000 + currTime.tv_nsec / 1000, start + time);
}

TEST(UtilsTest, busyWaitManyThreadTest) {
	double start = std::clock();
	int time = 1000000;
	struct timespec thread1;
	struct timespec thread2;
	
//	cpu_set_t  mask;
//	CPU_ZERO(&mask);
//	CPU_SET(0, &mask);
//	CPU_SET(2, &mask);
//	result = sched_setaffinity(0, sizeof(mask), &mask);
	
	std::thread t1([&](){ 
		Utils::busyWait(time); ;
		clockid_t threadClockId;
		pthread_getcpuclockid(pthread_self(), &threadClockId);
		clock_gettime(threadClockId, &thread1); 
	});

	std::thread t2([&](){
		Utils::busyWait(time); ;
		clockid_t threadClockId;
		pthread_getcpuclockid(pthread_self(), &threadClockId);
		clock_gettime(threadClockId, &thread2); 
	});

	t1.join();
	t2.join();
	
	EXPECT_GE((thread1.tv_sec + thread2.tv_sec) * 1000000 + (thread1.tv_nsec + thread2.tv_nsec) / 1000, start + time * 2 );
}

#include "Assert.hxx"
#include "Err.hxx"

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using std::cout;
using std::endl;

namespace
{
	template <typename MutexType>
	void test_basic_locking()
	{
		MutexType mutex;
		std::unique_lock<MutexType> lock(mutex, std::defer_lock);

		CLAM_ASSERT(!lock.owns_lock(), "Deferred lock should start unlocked");
		lock.lock();
		CLAM_ASSERT(lock.owns_lock(), "Lock did not acquire the mutex");
		lock.unlock();
		CLAM_ASSERT(!lock.owns_lock(), "Unlock did not release the mutex");
		lock.lock();
		CLAM_ASSERT(lock.owns_lock(), "Relock did not acquire the mutex");
	}

	template <typename MutexType>
	void test_try_locking()
	{
		MutexType mutex;
		std::unique_lock<MutexType> owner(mutex);
		bool acquired_in_other_thread = true;
		std::thread blocked([&] {
			std::unique_lock<MutexType> failed(mutex, std::try_to_lock);
			acquired_in_other_thread = failed.owns_lock();
		});
		blocked.join();

		CLAM_ASSERT(!acquired_in_other_thread, "try_to_lock acquired a mutex owned by another thread");
		owner.unlock();

		std::unique_lock<MutexType> acquired(mutex, std::try_to_lock);
		CLAM_ASSERT(acquired.owns_lock(), "try_to_lock failed on an unlocked mutex");
	}

	template <typename MutexType>
	void test_timed_locking()
	{
		MutexType mutex;
		std::unique_lock<MutexType> owner(mutex);
		bool acquired_in_other_thread = true;
		std::thread blocked([&] {
			std::unique_lock<MutexType> timed(mutex, std::defer_lock);
			acquired_in_other_thread = timed.try_lock_for(std::chrono::milliseconds(10));
		});
		blocked.join();

		CLAM_ASSERT(!acquired_in_other_thread, "Timed lock acquired a mutex owned by another thread");
		owner.unlock();

		std::unique_lock<MutexType> timed(mutex, std::defer_lock);
		CLAM_ASSERT(timed.try_lock_for(std::chrono::milliseconds(100)), "Timed lock failed on an unlocked mutex");
	}

	void test_mutex()
	{
		test_basic_locking<std::mutex>();
		test_try_locking<std::mutex>();

		cout << "std::mutex test passed...." << endl;
	}

	void test_timed_mutex()
	{
		test_basic_locking<std::timed_mutex>();
		test_try_locking<std::timed_mutex>();
		test_timed_locking<std::timed_mutex>();

		cout << "std::timed_mutex test passed..." << endl;
	}

	void test_recursive_mutex()
	{
		test_basic_locking<std::recursive_mutex>();
		test_try_locking<std::recursive_mutex>();

		std::recursive_mutex mutex;
		std::lock_guard<std::recursive_mutex> lock1(mutex);
		std::lock_guard<std::recursive_mutex> lock2(mutex);

		cout << "std::recursive_mutex test passed..." << endl;
	}

	void test_recursive_timed_mutex()
	{
		test_basic_locking<std::recursive_timed_mutex>();
		test_try_locking<std::recursive_timed_mutex>();
		test_timed_locking<std::recursive_timed_mutex>();

		std::recursive_timed_mutex mutex;
		std::lock_guard<std::recursive_timed_mutex> lock1(mutex);
		std::lock_guard<std::recursive_timed_mutex> lock2(mutex);

		cout << "std::recursive_timed_mutex test passed..." << endl;
	}

	class TestCondition
	{
	public:
		void WaitUntilNotified()
		{
			std::unique_lock<std::mutex> lock(mutex);
			++waiting;
			ready_condition.notify_one();
			condition.wait(lock, [this] { return notified; });
			++awoken;
		}

		std::mutex mutex;
		std::condition_variable ready_condition;
		std::condition_variable condition;
		bool notified = false;
		int waiting = 0;
		int awoken = 0;
	};

	void test_condition_notify_one()
	{
		TestCondition test_cond;
		std::thread thread(&TestCondition::WaitUntilNotified, &test_cond);

		{
			std::unique_lock<std::mutex> lock(test_cond.mutex);
			test_cond.ready_condition.wait(lock, [&test_cond] { return test_cond.waiting == 1; });
			test_cond.notified = true;
		}
		test_cond.condition.notify_one();

		thread.join();
		CLAM_ASSERT(test_cond.awoken == 1, "std::condition_variable notify_one failed");

		cout << "std::condition_variable notify_one passed!" << endl;
	}

	void test_condition_notify_all()
	{
		constexpr int nthreads = 5;
		TestCondition test_cond;
		std::vector<std::thread> threads;

		for (int i = 0; i < nthreads; ++i)
			threads.emplace_back(&TestCondition::WaitUntilNotified, &test_cond);

		{
			std::unique_lock<std::mutex> lock(test_cond.mutex);
			test_cond.ready_condition.wait(lock, [&test_cond, n=nthreads] { return test_cond.waiting == n; });
			test_cond.notified = true;
		}
		test_cond.condition.notify_all();

		for (std::thread& thread : threads)
			thread.join();

		CLAM_ASSERT(test_cond.awoken == nthreads, "std::condition_variable notify_all failed");

		cout << "std::condition_variable notify_all passed" << endl;
	}

	void test_condition_waits()
	{
		std::mutex mutex;
		std::condition_variable condition;
		int value = 0;

		{
			std::unique_lock<std::mutex> lock(mutex);
			CLAM_ASSERT(
				condition.wait_for(lock, std::chrono::milliseconds(10), [&value] { return value == 1; }) == false,
				"std::condition_variable wait_for should time out"
			);
		}

		std::thread thread([&] {
			{
				std::lock_guard<std::mutex> lock(mutex);
				value = 1;
			}
			condition.notify_one();
		});

		{
			std::unique_lock<std::mutex> lock(mutex);
			condition.wait(lock, [&value] { return value == 1; });
			CLAM_ASSERT(value == 1, "std::condition_variable predicate wait failed");
		}

		thread.join();

		cout << "std::condition_variable waits passed" << endl;
	}

	void test_condition()
	{
		cout << "Testing std::condition_variable notify_one" << endl;
		test_condition_notify_one();
		cout << "Testing std::condition_variable notify_all" << endl;
		test_condition_notify_all();
		cout << "Testing std::condition_variable waits" << endl;
		test_condition_waits();
	}

	void test_thread()
	{
		bool executed = false;
		std::thread thread([&executed] {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			executed = true;
		});

		thread.join();
		CLAM_ASSERT(executed, "std::thread did not execute");

		cout << "std::thread test passed!" << endl;
	}
}

int main(int argc, char** argv)
{
	(void) argc;
	(void) argv;

	try
	{
		cout << "Testing std::mutex" << endl;
		test_mutex();
		cout << "Testing std::timed_mutex" << endl;
		test_timed_mutex();
		cout << "Testing std::recursive_mutex" << endl;
		test_recursive_mutex();
		cout << "Testing std::recursive_timed_mutex" << endl;
		test_recursive_timed_mutex();
		cout << "Testing std::thread" << endl;
		test_thread();
		cout << "Testing std::condition_variable" << endl;
		test_condition();

		cout << "All tests passed. Congratulations!" << endl;
	}
	catch (CLAM::Err& e)
	{
		e.Print();
		std::cerr << "Abnormal program termination" << std::endl;
		return 1;
	}

	return 0;
}

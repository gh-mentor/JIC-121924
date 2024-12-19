/*
This app simulates a workload using a lambda function and a packaged task.
TODO:
1) refactor 'job' as a global variable so it may be unit testable
2) test the refactored 'job' function
*/

/*
Include header for input/output stream, future, chrono, thread, functional, and random
*/

#include <iostream>     // this is for input/output stream
#include <future>       // this is for future, which is a mechanism to retrieve a value from a thread
#include <chrono>       // this is for time-related functions
#include <thread>       // this is for threading
#include <functional>   // this is for functional programming
#include <random>       // this is for random number generation


/*
Create a function named 'execute' that executes a given packaged task in a separate thread.
Arguments:
- t: a function that takes an integer as an argument and returns void
- load: an integer representing the workload. 
Returns: void
Details:
- Create a packaged_task with the given function
- Get the future from the task
- Start the task in a separate thread using move semantics
- Wait for the task to complete
- Join the worker thread
- Print "Task completed." after the task is done
Example:
execute(job, 10);
*/
void execute(std::function<void(int)> t, int load)
{
    std::packaged_task<void(int)> task(t);
    std::future<void> future = task.get_future();
    std::thread worker(std::move(task), load);
    worker.join();
    future.get();
    std::cout << "Task completed." << '\n';
}



int main()
{

    /*
    Create a lambda function named 'job' that simulates a workload.
    Arguments:
    - 'workload': an integer representing the number of steps in the workload
    Returns: 
    - void
    Details:
    - Use a 'for loop' to to simulate a workload by iterating from the given workload value down to 1. 
    - For each iteration, it performs the following steps:
        1) Generates a random sleep time between 0.5 and 2.5 seconds.
        2) Pauses the execution for the generated sleep time.
        3) Prints a message indicating the completion of the current step.
    Error conditions:
    - If the workload is less than or equal to 0, the function should throw an invalid_argument exception with the message "Invalid workload value".
    Example:
    - job(10); // Simulates a workload with 10 steps
    */

    auto job = [](int workload) {
        if (workload <= 0)
        {
            throw std::invalid_argument("Invalid workload value");
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.5, 2.5);

        for (int i = workload; i > 0; i--)
        {
            double sleep_time = dis(gen);
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sleep_time * 1000)));
            std::cout << "Step " << i << " completed." << '\n';
        }
    };


    const int steps = 10;
    execute(job, steps);

    return 0;
}

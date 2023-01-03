# std-thread
An introduction to std::thread and locking basics

Note that the use of `volatile` when sharing variables in these examples 
is the traditional form of what might be better done with the use of 
[std::atomic](https://en.cppreference.com/w/cpp/atomic/atomic).

- `thread1`: print in two threads and see interleaved output
- `thread2`: same using a command line arg to set the number of threads
- `thread2`: "big lock" the threads to clean up the output
- `thread3`: fine grain locking clean output and can see both threads running at the same time
- `thread4`: two threads using a counter to share their work w/race condition on the counter
- `thread5`: add locks to protect counter race condition

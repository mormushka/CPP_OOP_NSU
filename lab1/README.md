# Priority Queue

- `PriorityQueue` class based on `std::vector<int>`
- Two modes: **max-heap** (default) and **min-heap**
- Core operations: `push()`, `pop()`, `top()` in O(log n)
- Boundary case checks (exceptions for empty queue)
- Comparison and stream output operators
- Full support for copy/move semantics

## Build

```bash
mkdir build && cd build
cmake .. && cmake --build .
```
## Run Tests

### CMake
```bash
ctest
```

### Tests with Valgrind
```bash
$ g++ -g -O0 -Wall -Wextra tests.cpp -lgtest -lgtest_main -lpthread -o tests && valgrind --leak-check=full ./tests && rm ./tests
```

```bash
==1635== Memcheck, a memory error detector
==1635== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==1635== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==1635== Command: ./tests
==1635==
Running main() from ./googletest/src/gtest_main.cc
[==========] Running 16 tests from 8 test suites.
[----------] Global test environment set-up.
[----------] 4 tests from Basic
[ RUN      ] Basic.1
[       OK ] Basic.1 (7 ms)
[ RUN      ] Basic.2
[       OK ] Basic.2 (13 ms)
[ RUN      ] Basic.3
[       OK ] Basic.3 (2 ms)
[ RUN      ] Basic.4
[       OK ] Basic.4 (2 ms)
[----------] 4 tests from Basic (29 ms total)

[----------] 3 tests from PushPopTop
[ RUN      ] PushPopTop.5
[       OK ] PushPopTop.5 (7 ms)
[ RUN      ] PushPopTop.6
[       OK ] PushPopTop.6 (2 ms)
[ RUN      ] PushPopTop.7
[       OK ] PushPopTop.7 (3 ms)
[----------] 3 tests from PushPopTop (13 ms total)

[----------] 2 tests from Exception
[ RUN      ] Exception.8
[       OK ] Exception.8 (24 ms)
[ RUN      ] Exception.9
[       OK ] Exception.9 (2 ms)
[----------] 2 tests from Exception (26 ms total)

[----------] 1 test from Clear
[ RUN      ] Clear.10
[       OK ] Clear.10 (1 ms)
[----------] 1 test from Clear (1 ms total)

[----------] 1 test from Reserve
[ RUN      ] Reserve.11
[       OK ] Reserve.11 (1 ms)
[----------] 1 test from Reserve (2 ms total)

[----------] 2 tests from Operators
[ RUN      ] Operators.12
[       OK ] Operators.12 (3 ms)
[ RUN      ] Operators.13
[       OK ] Operators.13 (1 ms)
[----------] 2 tests from Operators (5 ms total)

[----------] 1 test from OutputOperator
[ RUN      ] OutputOperator.14
[       OK ] OutputOperator.14 (5 ms)
[----------] 1 test from OutputOperator (5 ms total)

[----------] 2 tests from VerifyHeap
[ RUN      ] VerifyHeap.15
[       OK ] VerifyHeap.15 (1 ms)
[ RUN      ] VerifyHeap.16
[       OK ] VerifyHeap.16 (1 ms)
[----------] 2 tests from VerifyHeap (3 ms total)

[----------] Global test environment tear-down
[==========] 16 tests from 8 test suites ran. (106 ms total)
[  PASSED  ] 16 tests.
==1635==
==1635== HEAP SUMMARY:
==1635==     in use at exit: 0 bytes in 0 blocks
==1635==   total heap usage: 339 allocs, 339 frees, 136,352 bytes allocated
==1635==
==1635== All heap blocks were freed -- no leaks are possible
==1635==
==1635== For lists of detected and suppressed errors, rerun with: -s
==1635== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
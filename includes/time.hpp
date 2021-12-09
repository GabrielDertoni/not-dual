#ifndef _TIME_H_
#define _TIME_H_

#include <chrono>
#include <ratio>

typedef std::chrono::time_point<std::chrono::high_resolution_clock> Timestamp;

template <typename Rep, class Period = std::ratio<1>>
using Duration = std::chrono::duration<Rep, Period>;

Timestamp getNow();

#endif

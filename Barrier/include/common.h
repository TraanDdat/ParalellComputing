#pragma once

#include<iostream>
#include<vector>
#include<atomic>
#include<thread>
#include<stdatomic.h>
#include<mutex>
#include <chrono>

using namespace std;

extern std::mutex mtx;
extern std::atomic<bool> testing_perf;
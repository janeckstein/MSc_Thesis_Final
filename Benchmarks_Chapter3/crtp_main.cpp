#include <iostream>
#include "normal_class.hpp"
#include "crtp_class.hpp"
#include "benchmark/benchmark.h"

static void Normal_class_test(benchmark::State& state) {
    uint64_t res;
    for ( auto _ : state) {
        volatile NormalDerived obj1;
        RunNormal(&obj1);
        res = obj1.GetValue();
    }
     //std::cout << res << "\n";
}

static void CRTP_class_test(benchmark::State& state) {
    uint64_t res;
    for ( auto _ : state) {
        volatile CRTPDerived obj2;
        RunCRTP<CRTPDerived>(&obj2);
        res = obj2.GetValue();
    }
     //std::cout << res  << "\n";
}

BENCHMARK(Normal_class_test);
BENCHMARK(CRTP_class_test);
// Run the benchmark
BENCHMARK_MAIN();
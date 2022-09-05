//
// Created by jan eckstein on 12.07.22.
//

#include <benchmark/benchmark.h>
#include "slowpath_h.h"
#include <stdexcept>
#include <iostream>

constexpr std::size_t max_len = 32;
constexpr std::size_t seed = 1;

void BM_bubble_sort(auto& vector) {
    unsigned int len = vector.size();
    int tmp;
    for (unsigned int i = 0; i < len; ++i) {
        for (unsigned int j = 0; j < i; ++j) {
            if (vector[i] < vector[j]) {
                std::swap(vector[i], vector[j]);
            }
        }
    }
}


static void BM_Bubble_sort(benchmark::State &state) {
    auto len = state.range(0);
    auto distrib = std::uniform_int_distribution(-100000, 100000);
    const auto v0 = get_random_vec(len, seed, distrib);
    auto v = v0;
    for (auto _: state) {
        v = v0;
        BM_bubble_sort( v );
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_Bubble_sort)->RangeMultiplier(2)->Range(2, max_len);

BENCHMARK_MAIN();
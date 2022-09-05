//
// Created by jan eckstein on 11.07.22.
//

#include <benchmark/benchmark.h>
#include "slowpath_h.h"
#include <stdexcept>
#include <iostream>

constexpr std::size_t max_len = 32;
constexpr std::size_t seed = 1;


void quick_sort(auto first, auto last) {
    if (first == last) {
        return;
    }
    auto pivot = *std::next(first, std::distance(first, last) / 2);
    auto middle1 = std::partition(first, last, [pivot](const auto &em) { return em < pivot; });
    auto middle2 = std::partition(middle1, last, [pivot](const auto &em) { return !(pivot < em); });
    quick_sort(first, middle1);
    quick_sort(middle2, last);
}

void __attribute__ ((noinline)) handleerror() {
    std::cout << "Error detected" << std::endl;
    std::cout << "Stopping operations" << std::endl;
}

void bubble_sort(auto first, auto last) {
    for (auto i = first; i != last; ++i) {
        for (auto j = first; j < i; ++j) {
            if (*i > 100000) {
                handleerror();
            }
            if (*i < *j) {
                std::iter_swap(i, j);
            }
        }
    }
}

void merge_sort(auto first, auto last) {
    if (last - first > 1) {
        auto middle = first + (last - first) / 2;
        merge_sort(first, middle);
        merge_sort(middle, last);
        std::inplace_merge(first, middle, last);
    }
}

void insertion_sort(auto first, auto last) {
    for (auto i = first; i != last; ++i) {
        std::rotate(std::upper_bound(first, i, *i), i, i + 1);
    }
}

static void BM_SortBase(benchmark::State &state, auto sort) {
    auto len = state.range(0);
    auto distrib = std::uniform_int_distribution(-100000, 100000);
    const auto v0 = get_random_vec(len, seed, distrib);
    auto v = v0;

    for (auto _: state) {
        v = v0;
        sort(v.begin(), v.end() );
        benchmark::ClobberMemory();
    }

    if (!std::is_sorted(v.begin(), v.end())) {
        throw std::runtime_error("bad sort");
    }

    auto bytes = len * sizeof(int);
    state.SetBytesProcessed(bytes * state.iterations());
    state.counters["bytes_used"] = benchmark::Counter(bytes,
                                                      benchmark::Counter::kDefaults,
                                                      benchmark::Counter::OneK::kIs1024);
    state.counters["n"] = state.range(0);
}

static void BM_BubbleSort(benchmark::State &state) {
    using It = std::vector<int>::iterator;
    BM_SortBase(state, bubble_sort < It, It >);
    state.SetLabel("bubble");
}

BENCHMARK(BM_BubbleSort)->RangeMultiplier(2)->Range(2, max_len);
BENCHMARK_MAIN();
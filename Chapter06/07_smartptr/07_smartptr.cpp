#include "07_smartptr.h"

#include "benchmark/benchmark.h"

void BM_rawptr(benchmark::State& state) {
    for (auto _ : state) {
        int* p(get_raw_ptr());
        d(p);
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_uniqueptr(benchmark::State& state) {
    for (auto _ : state) {
        auto p(get_unique_ptr());
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_sharedptr(benchmark::State& state) {
    for (auto _ : state) {
        auto p(get_shared_ptr());
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_make_sharedptr(benchmark::State& state) {
    for (auto _ : state) {
        auto p(make_shared_ptr());
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_smartptr(benchmark::State& state) {
    for (auto _ : state) {
        auto p(get_smartptr());
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_smartptr_te(benchmark::State& state) {
    for (auto _ : state) {
        auto p(get_smartptr_te());
    }
    state.SetItemsProcessed(state.iterations());
}



BENCHMARK(BM_rawptr);
BENCHMARK(BM_uniqueptr);
BENCHMARK(BM_sharedptr);
BENCHMARK(BM_make_sharedptr);
BENCHMARK(BM_smartptr);
BENCHMARK(BM_smartptr_te);


BENCHMARK_MAIN();
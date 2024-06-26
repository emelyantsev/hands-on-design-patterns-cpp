#include <exception>
#include <iostream>

enum Outcome { SUCCESS, FAIL_RETURN, FAIL_THROW };

// Demo disk storage, does nothing useful but may throw exception.
class Storage {
    public:
    Storage() = default;
    bool insert(int i, Outcome outcome) {
        if (outcome == FAIL_THROW) throw 0;
        if (outcome == FAIL_RETURN) return false;
        i1_ = i_; i_ = i;
        return true;
    }
    void undo() {
        i_ = i1_;
    }
    void finalize() { finalized_ = true; }
    bool finalized() const { return finalized_; }
    int get() const { return i_; }
    private:
    int i_ = 0;
    int i1_ = 0;
    bool finalized_ = false;
};

// Demo memory index, does nothing useful but may throw exception.
class Index {
    public:
    Index() = default;
    bool insert(int i, Outcome outcome) {
        if (outcome == FAIL_THROW) throw 0;
        if (outcome == FAIL_RETURN) return false;
        i1_ = i_; i_ = i;
        return true;
    }
    void undo() {
        i_ = i1_;
    }
    int get() const { return i_; }
    private:
    int i_ = 0;
    int i1_ = 0;
};

namespace __cxxabiv1 {
    struct __cxa_eh_globals;
    extern "C" __cxa_eh_globals* __cxa_get_globals() noexcept;
}
class UncaughtExceptionDetector {
    public:
    UncaughtExceptionDetector() : count_(uncaught_exceptions()) {}
    operator bool() const noexcept { return uncaught_exceptions() > count_; }
    private:
    const int count_;
    int uncaught_exceptions() const noexcept {
        return *(reinterpret_cast<int*>(
                    static_cast<char*>(
                        static_cast<void*>(
                            __cxxabiv1::__cxa_get_globals())) + sizeof(void*)));
    }
};

template <typename Func, bool on_success, bool on_failure>
class ScopeGuard {
    public:
    ScopeGuard(Func&& func) : func_(func) {}
    ScopeGuard(const Func& func) : func_(func) {}
    ~ScopeGuard() {
        if ((on_success && !detector_) ||
            (on_failure && detector_)) func_();
    }
    ScopeGuard(ScopeGuard&& other) = default;
    private:
    Func func_;
    UncaughtExceptionDetector detector_;
    ScopeGuard() = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
};

#define CONCAT2(x, y) x##y
#define CONCAT(x, y) CONCAT2(x, y)
#ifdef __COUNTER__
#define ANON_VAR(x) CONCAT(x, __COUNTER__)
#else
#define ANON_VAR(x) CONCAT(x, __LINE__)
#endif

struct ScopeGuardOnExit {};
template <typename Func>
auto operator+(ScopeGuardOnExit, Func&& func) {
    return ScopeGuard<Func, true, true>(std::forward<Func>(func));
}
#define ON_SCOPE_EXIT \
    auto ANON_VAR(SCOPE_EXIT_STATE) = ScopeGuardOnExit() + [&]()

struct ScopeGuardOnSuccess {};
template <typename Func>
auto operator+(ScopeGuardOnSuccess, Func&& func) {
    return ScopeGuard<Func, true, false>(std::forward<Func>(func));
}
#define ON_SCOPE_SUCCESS \
    auto ANON_VAR(SCOPE_EXIT_STATE) = ScopeGuardOnSuccess() + [&]()

struct ScopeGuardOnFailure {};
template <typename Func>
auto operator+(ScopeGuardOnFailure, Func&& func) {
    return ScopeGuard<Func, false, true>(std::forward<Func>(func));
}
#define ON_SCOPE_FAILURE \
    auto ANON_VAR(SCOPE_EXIT_STATE) = ScopeGuardOnFailure() + [&]()

int main() {
    Storage S;
    Index I;
    try {
        S.insert(42, SUCCESS);
        ON_SCOPE_EXIT { S.finalize(); };
        ON_SCOPE_FAILURE { S.undo(); };
        I.insert(42, FAIL_THROW);
    } catch (...) {
    }

    {
        ON_SCOPE_SUCCESS { std::cout << "Success" << std::endl; };
    }

    if (S.get() != I.get()) std::cout << "Inconsistent state: " << S.get() << " != " << I.get() << std::endl;
    else if (!S.finalized()) std::cout << "Not finalized" << std::endl;
    else std::cout << "Database OK" << std::endl;
}
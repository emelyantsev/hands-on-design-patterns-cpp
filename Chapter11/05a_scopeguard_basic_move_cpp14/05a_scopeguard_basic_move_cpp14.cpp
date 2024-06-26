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
    int get() const { return i_; }
    private:
    int i_ = 0;
    int i1_ = 0;
};
void undo(Storage& S) { S.undo(); }

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

class ScopeGuardImplBase {
    public:
    ScopeGuardImplBase() = default;
    void commit() const noexcept { commit_ = true; }

    protected:
    ScopeGuardImplBase(ScopeGuardImplBase&& other) : commit_(other.commit_) { other.commit(); }
    ~ScopeGuardImplBase() {}
    mutable bool commit_ = false;

    private:
    ScopeGuardImplBase& operator=(const ScopeGuardImplBase&) = delete;
};
using ScopeGuard = const ScopeGuardImplBase&;

template <typename Func, typename Arg>
class ScopeGuardImpl : public ScopeGuardImplBase {
    public:
    ScopeGuardImpl(const Func& func, Arg& arg) : func_(func), arg_(arg) {}
    ~ScopeGuardImpl() { if (!commit_) func_(arg_); }
    ScopeGuardImpl(ScopeGuardImpl&& other) = default;
    private:
    const Func& func_;
    Arg& arg_;
};

template <typename Func, typename Arg>
auto MakeGuard(const Func& func, Arg& arg) {
    return ScopeGuardImpl<Func, Arg>(func, arg);
}

int main() {
    Storage S;
    Index I;
    try {
        S.insert(42, SUCCESS);
        ScopeGuard SG = MakeGuard(undo, S);
        I.insert(42, FAIL_THROW);
        SG.commit();
    } catch (...) {
    }

    if (S.get() != I.get()) std::cout << "Inconsistent state: " << S.get() << " != " << I.get() << std::endl;
    else std::cout << "Database OK" << std::endl;
}
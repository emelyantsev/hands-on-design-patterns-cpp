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
        throw 0;
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

class ScopeGuardBase {
    public:
    ScopeGuardBase() : commit_(false) {}
    void commit() noexcept { commit_ = true; }

    protected:
    ScopeGuardBase(ScopeGuardBase&& other) : commit_(other.commit_) { other.commit(); }
    ~ScopeGuardBase() {}
    bool commit_ = false;

    private:
    ScopeGuardBase& operator=(const ScopeGuardBase&) = delete;
};

template <typename Func>
class ScopeGuard : public ScopeGuardBase {
    public:
    ScopeGuard(Func&& func) : func_(std::move(func)) {}
    ScopeGuard(const Func& func) : func_(func) {}
    ~ScopeGuard() {
        if (!commit_) try { func_(); } catch (...) {}
    }
    ScopeGuard(ScopeGuard&& other) = default;
    private:
    Func func_;
};

template <typename Func>
ScopeGuard<Func> MakeGuard(Func&& func) {
    return ScopeGuard<Func>(std::forward<Func>(func));
}

int main() {

    Storage S;
    Index I;
    
    try {
    
        S.insert(42, SUCCESS);
        auto SF = MakeGuard([&] { S.finalize(); });
        auto SG = MakeGuard([&] { S.undo(); });
        I.insert(42, FAIL_THROW);
        SG.commit();

    } catch (...) {
        
    }

    if (S.get() != I.get()) std::cout << "Inconsistent state: " << S.get() << " != " << I.get() << std::endl;
    else if (!S.finalized()) std::cout << "Not finalized" << std::endl;
    else std::cout << "Database OK" << std::endl;
}
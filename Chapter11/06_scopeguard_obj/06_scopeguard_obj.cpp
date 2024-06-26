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

template <typename MemFunc, typename Obj>
class ScopeGuardImpl : public ScopeGuardImplBase {
    public:
    ScopeGuardImpl(const MemFunc& memfunc, Obj& obj) : memfunc_(memfunc), obj_(obj) {}
    ~ScopeGuardImpl() { if (!commit_) (obj_.*memfunc_)(); }
    ScopeGuardImpl(ScopeGuardImpl&& other) = default;
    private:
    const MemFunc& memfunc_;
    Obj& obj_;
};

template <typename MemFunc, typename Obj>
ScopeGuardImpl<MemFunc, Obj> MakeGuard(const MemFunc& memfunc, Obj& obj) {
    return ScopeGuardImpl<MemFunc, Obj>(memfunc, obj);
}

int main() {
    Storage S;
    Index I;
    try {
        S.insert(42, SUCCESS);
        ScopeGuard SG = MakeGuard(&Storage::undo, S);
        I.insert(42, FAIL_THROW);
        SG.commit();
    } catch (...) {
    }

    if (S.get() != I.get()) std::cout << "Inconsistent state: " << S.get() << " != " << I.get() << std::endl;
    else std::cout << "Database OK" << std::endl;
}
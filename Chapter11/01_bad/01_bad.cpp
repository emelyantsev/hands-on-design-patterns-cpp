#include <iostream>

enum Outcome { SUCCESS, FAIL_RETURN, FAIL_THROW };

// Demo disk storage, does nothing useful but may throw exception.
class Storage {
    public:
    Storage() = default;
    bool insert(int i, Outcome outcome) {
        if (outcome == FAIL_THROW) throw 0;
        if (outcome == FAIL_RETURN) return false;
        i_ = i;
        return true;
    }
    int get() const { return i_; }
    private:
    int i_ = 0;
};

// Demo memory index, does nothing useful but may throw exception.
class Index {
    public:
    Index() = default;
    bool insert(int i, Outcome outcome) {
        if (outcome == FAIL_THROW) throw 0;
        if (outcome == FAIL_RETURN) return false;
        i_ = i;
        return true;
    }
    int get() const { return i_; }
    private:
    int i_ = 0;
};

int main() {

    Storage S;
    Index I;
    
    try {

        S.insert(42, SUCCESS);
        I.insert(42, FAIL_THROW);

    } 
    catch (...) {

    }

    if ( S.get() != I.get() ) std::cout << "Inconsistent state: " << S.get() << " != " << I.get() << std::endl;
}
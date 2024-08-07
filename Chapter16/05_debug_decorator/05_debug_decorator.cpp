// 03 with debug decorator
#include <iostream>
using std::cout;
using std::endl;

// Basic decorator
class Unit {
    public:
    Unit(double strength, double armor) : strength_(strength), armor_(armor) {}
    virtual bool hit(Unit& target) { return attack() > target.defense(); }
    virtual double attack() = 0;
    virtual double defense() = 0;
    protected:
    double strength_;
    double armor_;
};

class Knight : public Unit {
    public:
    using Unit::Unit;
    double attack() override { double res = strength_ + sword_bonus_ + charge_bonus_; charge_bonus_ = 0; return res; }
    double defense() override { return armor_ + plate_bonus_; }
    void charge() { charge_bonus_ = 1; }
    protected:
    double charge_bonus_ {};
    static constexpr double sword_bonus_ = 2;
    static constexpr double plate_bonus_ = 3;
};

class Ogre : public Unit {
    public:
    using Unit::Unit;
    double attack() override { return strength_ + club_penalty_; }
    double defense() override { return armor_ + leather_penalty_; }
    protected:
    static constexpr double club_penalty_ = -1;
    static constexpr double leather_penalty_ = -1;
};

class Troll : public Unit {
    public:
    using Unit::Unit;
    double attack() override { return strength_ + mace_bonus_; }
    double defense() override { return armor_ + hide_bonus_; }
    protected:
    static constexpr double mace_bonus_ = 3;
    static constexpr double hide_bonus_ = 8;
};

template <typename U> class VeteranUnit : public U {
    public:
    VeteranUnit(U&& unit, double strength_bonus, double armor_bonus) : U(unit), strength_bonus_(strength_bonus), armor_bonus_(armor_bonus) {}
    double attack() override { return U::attack() + strength_bonus_; }
    double defense() override { return U::defense() + armor_bonus_; }
    private:
    double strength_bonus_;
    double armor_bonus_;
};

template <typename U> class DebugDecorator : public U {
    public:
    using U::U;
    double attack() override { double res = U::attack(); cout << "Attack: " << res << endl; return res; }
    double defense() override { double res = U::defense(); cout << "Defense: " << res << endl; return res; }
};

int main() {

    DebugDecorator<Knight> k(10, 5);
    DebugDecorator<Ogre> o(12, 2);
    k.charge();
    cout << "Knight hits Ogre: " << k.hit(o) << endl;
    Troll t(14, 10);
    cout << "Knight hits Troll: " << k.hit(t) << endl;
    DebugDecorator<VeteranUnit<Knight>> vk(std::move(k), 7, 2);
    cout << "Veteran Knight hits Troll: " << vk.hit(t) << endl;
    DebugDecorator<VeteranUnit<Ogre>> vo(std::move(o), 1, 9);
    cout << "Veteran Knight hits Veteran Ogre: " << vk.hit(vo) << endl;
    DebugDecorator<VeteranUnit<VeteranUnit<Ogre>>> vvo(std::move(vo), 1, 9);
    cout << "Veteran Knight hits Veteran2 Ogre: " << vk.hit(vvo) << endl;
    vk.charge();
    
    cout << "Veteran Knight hits Veteran2 Ogre: " << vk.hit(vvo) << endl;
}
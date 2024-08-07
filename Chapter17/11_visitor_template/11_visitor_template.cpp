// 02 with template Visitable

#include <iostream>
#include <string>
#include <string_view>
#include <memory>

class Cat;
class Dog;

class PetVisitor {
    public:
    virtual void visit(Cat* c) = 0;
    virtual void visit(Dog* d) = 0;
};

class Pet {
    public:
    virtual ~Pet() {}
    Pet(std::string_view color) : color_(color) {}
    const std::string& color() const { return color_; }
    virtual void accept(PetVisitor& v) = 0;
    private:
    std::string color_;
};

template <typename Derived>
class Visitable : public Pet {
    public:
    using Pet::Pet;
    void accept(PetVisitor& v) override {
        v.visit(static_cast<Derived*>(this)); 
    }
};

class Cat : public Visitable<Cat> {
    using Visitable<Cat>::Visitable;
};

class Dog : public Visitable<Dog> {
    using Visitable<Dog>::Visitable;
};

class FeedingVisitor : public PetVisitor {
    public:
    void visit(Cat* c) override { std::cout << "Feed tuna to the " << c->color() << " cat" << std::endl; }
    void visit(Dog* d) override { std::cout << "Feed steak to the " << d->color() << " dog" << std::endl; }
};

class PlayingVisitor : public PetVisitor {
    public:
    void visit(Cat* c) override { std::cout << "Play with feather with the " << c->color() << " cat" << std::endl; }
    void visit(Dog* d) override { std::cout << "Play fetch with the " << d->color() << " dog" << std::endl; }
};

int main() {
    std::unique_ptr<Pet> c(new Cat("orange"));
    std::unique_ptr<Pet> d(new Dog("brown"));

    FeedingVisitor fv;
    c->accept(fv);
    d->accept(fv);

    PlayingVisitor pv;
    c->accept(pv);
    d->accept(pv);
}
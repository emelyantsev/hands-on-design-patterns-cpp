#pragma once 

#include <vector>
#include <iostream>
#include <sstream>
#include <memory>


class SerializerBase {

    static size_t type_count;
    using Factory = SerializerBase* (*)(std::istream& s);
    static std::vector<Factory> registry;

  protected:
  
    virtual void Serialize(std::ostream& s) const = 0;

  public:
  
    virtual ~SerializerBase() {}

    static size_t RegisterType( Factory factory ) ;
    static std::unique_ptr<SerializerBase> Deserialize( size_t type, std::istream& s ) ;
};


class DeserializerFactory {

    std::istream& s_;

  public:

    explicit DeserializerFactory(std::istream& s) ;

    template <typename It>
    void Deserialize(It iter) {

        while (true) {
        size_t type;
        s_ >> type;
        if (s_.eof()) return;
        iter = SerializerBase::Deserialize(type, s_);
        }    
    }
};


class Derived1 : public SerializerBase {
    
  public:
    Derived1(int i) ;
    void Serialize(std::ostream& s) const override ;
    static const size_t type_tag;

  private:
    int i_;
};


class Derived2 : public SerializerBase {
  
  public:
    Derived2(double x, double y) ;
    void Serialize(std::ostream& s) const override ;
    static const size_t type_tag;

  private:
    double x_, y_;
};


class Derived3 : public SerializerBase {

  public:

    Derived3(int i) ;
    Derived3(double x) ;
    void Serialize(std::ostream& s) const ;
    static const size_t type_tag;

  private:

    bool integer_;
    int i_ {};
    double x_ {};
};
#include "Serialize.h"


size_t SerializerBase::RegisterType(Factory factory) {

    registry.push_back( factory );
    return type_count++;
}

std::unique_ptr<SerializerBase> SerializerBase::Deserialize( size_t type, std::istream& s ) {

    Factory factory = registry[ type ];
    return std::unique_ptr<SerializerBase>( factory(s) );
}


std::vector<SerializerBase::Factory> SerializerBase::registry;
size_t SerializerBase::type_count = 0;

DeserializerFactory::DeserializerFactory( std::istream& s ) : s_( s ) {}

/* -------------------- */

Derived1::Derived1(int i) : i_(i) { std::cout << "Derived1(" << i << ")" << std::endl; }

void Derived1::Serialize(std::ostream& s) const { s << type_tag << " " << i_ << std::endl; }

const size_t Derived1::type_tag = RegisterType( [](std::istream& s)->SerializerBase* { int i; s >> i; return new Derived1( i ) ; } );


/* -------------------- */

Derived2::Derived2(double x, double y) : x_(x), y_(y) { std::cout << "Derived2(" << x << ", " << y << ")" << std::endl; }

void Derived2::Serialize(std::ostream& s) const { s << type_tag << " " << x_ << " " << y_ << std::endl; }

const size_t Derived2::type_tag = RegisterType([](std::istream& s)->SerializerBase* { double x, y; s >> x >> y; return new Derived2(x, y); });


/* ------------------ */

Derived3::Derived3(int i) : integer_(true), i_(i) { std::cout << "Derived3(" << i << ") - integer" << std::endl; }

Derived3::Derived3(double x) : integer_(false), x_(x) { std::cout << "Derived3(" << x << ") - double" << std::endl; }

void Derived3::Serialize(std::ostream& s) const { s << type_tag << " " << integer_ << " "; if (integer_) s << i_; else s << x_; s << std::endl; }


const size_t Derived3::type_tag = RegisterType( [](std::istream& s)->SerializerBase* {
  
  bool integer; s >> integer;
  
  if (integer) {
    int i; s >> i; return new Derived3( i );
  } else {
    double x; s >> x; return new Derived3( x );
  }
}) ;
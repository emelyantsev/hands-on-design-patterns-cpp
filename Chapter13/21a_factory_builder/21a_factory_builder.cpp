#include "Serialize.h"

#include <fstream> 

int main() {

  std::fstream S;
  S.open ("test.txt", std::fstream::in | std::fstream::out | std::fstream::trunc );

  // Serialization.
  Derived1( 42 ).Serialize( S );

  std::cout << "Out pos = " << S.tellp() << " In pos " << S.tellg() << std::endl;
  
  Derived2( 3.1415, 2.71828 ).Serialize( S );
  
  std::cout << "Out pos = " << S.tellp() << " In pos " << S.tellg() << std::endl;
  
  Derived3( 0.5772156649 ).Serialize( S );

  std::cout << "Out pos = " << S.tellp() << " In pos " << S.tellg() << std::endl;

  Derived3( 144 ).Serialize( S );

  std::cout << "Out pos = " << S.tellp() << " In pos " << S.tellg() << std::endl;

  S.seekg( 0, S.beg );

  std::cout <<  "-----------------------------------"  << std::endl ;

  // Deserialization.
  std::vector< std::unique_ptr<SerializerBase> > v;
  DeserializerFactory F( S );
  F.Deserialize( std::back_inserter( v ) );

  S.close();
}

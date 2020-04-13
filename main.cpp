#include <iostream>
#include "allocators/GrowingBlockAllocator.h"

struct Data
{
public:
	int32_t* Marks;
	int32_t id;

	Data( int32_t id_ ) :id( id_ )
	{
		Marks = new int32_t[ 5 ];
		std::cout << "Data_" << id << "is constructed" << std::endl;
	}

	~Data( )
	{
		delete[ ] Marks;
		std::cout << "Data_" << id << "is destroyed" << std::endl;
	}
};

struct Pack
{
public:
	Pack( int32_t v )
		:value( v )
	{
		std::cout << "Pack_" << value << "is constructed at " << (void*) this << std::endl;
		data = new Data( value );
	}

	~Pack( )
	{
		delete data;
		std::cout << "Pack_" << value << "is destroyed at " << (void*) this << std::endl;
	}

	Data* data;
	int32_t value;
};

int main( )
{
	GrowingBlockAllocator<Pack, 3> allocator;
	std::vector< ptr_ref<Pack>> packs;

	std::cout << '\n' << "**************************" << '\n' << std::endl;
	for (size_t i = 0; i < 7; i++)
	{
		packs.push_back( allocator.instantiate( static_cast<int32_t>( 1 + i ) ) );
	}

	std::cout << '\n' << "**************************" << '\n' << std::endl;
	allocator.release( packs[ 1 ] );
	allocator.release( packs[ 4 ] );

	std::cout << "Is packs[4] null? " << ( packs[ 4 ] == nullptr ) << std::endl;
	packs[ 4 ] = allocator.instantiate( 8 );
	std::cout << "Is packs[4] null? " << ( packs[ 4 ] == nullptr ) << std::endl;

	std::cout << '\n' << "**************************" << '\n' << std::endl;

	std::cout << "Elements in Memery Pool :" << std::endl;
	for (size_t i = 0; i < allocator.size( ); i++)
	{
		std::cout << "Data_" << allocator[ i ]->value << std::endl;
	}
	std::cout << '\n' << "**************************" << '\n' << std::endl;

	return 0;
}

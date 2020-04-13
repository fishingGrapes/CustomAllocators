# CustomAllocators
This repository contains custom allocators I wrote(and will) for my educational game engine project. 

## Growing List Allocator

![growinglistallocator](https://github.com/fishingGrapes/CustomAllocators/blob/master/images/growinglistallocator.png)

I needed to iterate through a collection of components(similar to Unity3D) every frame. There are game objects which are composed of components and these components contain behaviour like Transform, Camera, Light, Mesh Renderer and so on, which need to be updated every frame. I could not afford to iterate through components at different places in memory. So I wrote this allocactor which is nothing but a list of fixed-size memory blocks and actively keeps the objects tightly packed on allocation and deallocation, so as to ensure the best cache locality and fast iterations. 


## ptr_ref
I also needed my own shared pointer implementation as the underlying object pointed to by __std::shared_ptr__ can only be destroyed when the reference count reaches zero. But in my engine the user can remove a component on demand and ptr_ref combines the advantages of std::shared_ptr and my requirements. ptr_ref works exactly to std::shared_ptr on the outside,but with the exception that all instances pointing to the same memory can be invalidated(their underlying data set to nullptr). This effectively allows us to destroy the underlying object and later on the user can query whether the ptr_ref is pointing to a valid object or not by simply checking equlaity against _nullptr_.


## Usage
```c++
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
```
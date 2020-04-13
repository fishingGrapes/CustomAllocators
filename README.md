# CustomAllocators
This repository contains custom allocators for my educational game engine project where I need to iterate through a collection of components(similar to Unity3D) every frame. There are game objects and every game object is composed of these components. Components are classes like Transform, Camera, Light, Mesh Renderer and so on which need to be updated every frame.

I also needed my own shared_ptr implementation, ptr_ref, as these components need to be destroyed even before the reference count reaches zero and this was not possible with std::shared_ptr. ptr_ref works very similar to std::shared_ptr on the outside, with the exception that all instances pointing to the same memory can be invalidated(their underlying data set to nullptr).

#pragma once
#include <functional>

template <typename T>
class ptr_ref
{
	using Deleter = std::function<void( ptr_ref<T>& )>;

public:

	ptr_ref( )
		: refCount_( nullptr ), data_( nullptr ), deleter_( nullptr )
	{
	}

	ptr_ref( T* ptr, const Deleter& deleter = nullptr )
		: refCount_( new size_t( 1 ) ), data_( new T* ( ptr ) ), deleter_( deleter )
	{
	}

	ptr_ref( const ptr_ref& other )
		: refCount_( other.refCount_ ), data_( other.data_ ), deleter_( other.deleter_ )
	{
		++( *refCount_ );
	}

	ptr_ref( ptr_ref&& other ) noexcept
		: refCount_( other.refCount_ ), data_( other.data_ ), deleter_( other.deleter_ )
	{
		other.data_ = nullptr;
		other.refCount_ = nullptr;
		other.deleter_ = nullptr;
	}

	ptr_ref& operator=( const ptr_ref& other )
	{
		this->refCount_ = other.refCount_;
		this->data_ = other.data_;
		this->deleter_ = other.deleter_;

		++( *refCount_ );

		return *this;
	}

	ptr_ref& operator=( ptr_ref&& other ) noexcept
	{
		this->refCount_ = other.refCount_;
		this->data_ = other.data_;
		this->deleter_ = other.deleter_;

		other.data_ = nullptr;
		other.refCount_ = nullptr;
		other.deleter_ = nullptr;

		return *this;
	}

	~ptr_ref( )
	{
		if (data_)
		{
			--( *refCount_ );

			if (*refCount_ <= 0)
			{
				if (*data_)
				{
					if (deleter_)
						deleter_( *this );
					else
						delete ( *data_ );
				}

				delete refCount_;
				delete data_;

				data_ = nullptr;
				refCount_ = nullptr;
			}
		}
	}

	//decreases the refcount by 1
	void reset( )
	{
		if (data_)
		{
			--( *refCount_ );

			if (*refCount_ <= 0)
			{

				if (*data_)
				{
					if (deleter_)
						deleter_( *this );
					else
						delete ( *data_ );
				}

				delete refCount_;
				delete data_;
			}

			data_ = nullptr;
			refCount_ = nullptr;
		}
	}

	T* operator->( ) const
	{
		return ( *data_ );
	}

	T operator*( ) const
	{
		return *( *data_ );
	}

	bool operator==( const ptr_ref& other ) const
	{
		return ( *data_ ) == *( other.data_ );
	}

	bool operator!=( const ptr_ref& other ) const
	{
		return ( *data_ ) != *( other.data_ );
	}

	template <typename... params >
	static ptr_ref<T> make( params&& ...args )
	{
		ptr_ref<T> ref;

		ref.refCount_ = new size_t( 1 );
		ref.data_ = new T * ( new T( std::forward<params>( args )... ) );

		return ref;
	}

	inline T* get( ) const
	{
		return ( *data_ );
	}

	inline T** get_address( ) const
	{
		return data_;
	}

	inline size_t ref_count( ) const
	{
		return ( *refCount_ );
	}

	void set( T* ptr )
	{
		*data_ = ptr;
	}

protected:
	size_t* refCount_;
	T** data_;
	Deleter deleter_;
};


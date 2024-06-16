

#include <stdlib.h>
#include <string.h>
#include <new>
#include <utility>
#include <iostream>
#include <thread>


template<size_t S, size_t A = alignof(void*), bool nothrow_movable = false>
struct Buffer {

    constexpr static auto size = S, alignment = A;

    alignas(alignment) char space_[size];

    template <typename T> static constexpr bool valid_type() {
        return sizeof(T) <= S && (A % alignof(T)) == 0;
    }

    template <typename T> requires( valid_type<T>() )
    T* as() noexcept { return reinterpret_cast<T*>(&space_); }
    
    template <typename T> requires( valid_type<T>() )
    const T* as() const noexcept { return const_cast<Buffer*>(this)->as<T>(); }

    Buffer() = default;

    template <typename T, typename... Args> requires( valid_type<T>() && std::constructible_from<T, Args...>)
    Buffer(std::in_place_type_t<T>, Args&& ...args) noexcept( std::is_nothrow_constructible_v<T, Args...> ) : vtable_(&vtable<Buffer, T>)
    {
        ::new (static_cast<void*>(as<T>())) T( std::forward<Args>(args)... );
    }

    template<typename T, typename... Args > requires( valid_type<T>() && std::constructible_from<T, Args...> )
    T* emplace(Args&& ...args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
    {
        if (this->vtable_) this->vtable_->deleter_(this);
        this->vtable_ = &vtable<Buffer, T>;
        return ::new (static_cast<void*>(as<T>())) T(std::forward<Args>(args)...);
    }

    Buffer(const Buffer& that) { if (that.vtable_) that.vtable_->copy_construct_(this, &that); }

    Buffer(Buffer&& that) noexcept(nothrow_movable) { if (that.vtable_) that.vtable_->move_construct_(this, &that); }
    
    Buffer& operator=(const Buffer& that) {
        if (this->vtable_) this->vtable_->deleter_(this);
        if (that.vtable_) that.vtable_->copy_construct_(this, &that);
        else this->vtable_ = nullptr;
        return *this;
    }
    
    Buffer& operator=(Buffer&& that) noexcept(nothrow_movable) {
        if (this->vtable_) this->vtable_->deleter_(this);
        if (that.vtable_) that.vtable_->move_construct_(this, &that);
        else this->vtable_ = nullptr;
        return *this;
    }

    ~Buffer() noexcept { if (this->vtable_) this->vtable_->deleter_(this); }

    void swap(Buffer& that) noexcept(nothrow_movable) {
        Buffer tmp(std::move(that));
        that = std::move(*this);
        *this = std::move(tmp);
    }

    // Destroy the object stored in the aligned space.
    void destroy() noexcept {
        if (this->vtable_) this->vtable_->deleter_(this);
        this->vtable_ = nullptr;
    }

    // Type erasure magic is here:
    // vtable_t is the virtual table containing pointers to the static functions
    // for copying, moving, and destroying the type of the object currently
    // stored in the space.
    struct vtable_t { 
        using deleter_t = void (Buffer*);
        using copy_construct_t = void (Buffer*, const Buffer*);
        using move_construct_t = void (Buffer*, Buffer*);

        deleter_t*  deleter_;
        copy_construct_t* copy_construct_;
        move_construct_t* move_construct_;
    };

    const vtable_t* vtable_ = nullptr;
    
    template <typename U, typename T> constexpr static vtable_t vtable = {
        U::template deleter<T>,
        U::template copy_construct<T>,
        U::template move_construct<T>
    };

    template <typename T>
        requires(valid_type<T>() && std::is_nothrow_destructible_v<T>)
        static void deleter(Buffer* space) {
        space->as<T>()->~T();
    }

    template <typename T>
        requires( valid_type<T>() )
        static void copy_construct(Buffer* to, const Buffer* from) 
        noexcept(std::is_nothrow_copy_constructible_v<T>)
    {
        ::new (static_cast<void*>(to->as<T>())) T(*from->as<T>());
        to->vtable_ = from->vtable_;
    }

    template <typename T>
        requires( valid_type<T>() )
        static void move_construct(Buffer* to, Buffer* from)
        noexcept(std::is_nothrow_move_constructible_v<T>)
    {
        ::new (static_cast<void*>(to->as<T>())) T(std::move(*from->as<T>()));
        to->vtable_ = from->vtable_;
    }

}; // Buffer

template<size_t S, size_t A, bool nothrow_movable> template <typename U, typename T> constexpr typename Buffer<S, A, nothrow_movable>::vtable_t Buffer<S, A, nothrow_movable>::vtable;


class Holder {

    public:

        Holder(int val) : value(val) {};


    private:

        int value;
};


int main() {


    Buffer<12> buf1;

    Buffer<12> buf2( std::in_place_type<Holder>, 12 );


    buf1 = buf2;


}
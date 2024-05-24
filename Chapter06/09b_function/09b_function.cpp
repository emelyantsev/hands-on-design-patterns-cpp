#include <iostream>
#include <string>
#include <string.h>
#include <utility>
#include <type_traits>
#include <new>
#include <typeinfo>
#include <functional>
#include <algorithm>
#include <vector>

#define REQUIRES(...) std::enable_if_t<__VA_ARGS__, int> = 0

template<typename Signature, size_t Size = sizeof(void*) *2, size_t Alignment = alignof(void*) > class Function;

template<size_t Size, size_t Alignment, typename Res, typename... Args>
class Function<Res(Args...), Size, Alignment>
{
    alignas(Alignment) char space_[Size];

    using executor_t = Res(*)(Args..., void*);
    
    static constexpr Res default_executor(Args..., void*) { throw std::bad_function_call(); }
    
    constexpr static executor_t default_executor_ = default_executor;
    
    executor_t executor_ = default_executor_;

    template<typename Callable> static Res executor(Args... args, void* this_function) {
    
        static_assert( !std::is_member_function_pointer<Callable>::value, "" );
    
        return ( *reinterpret_cast<Callable*>( static_cast<Function*>(this_function)->space_))(std::forward<Args>(args)...);
    }

public:
    // Default-constructed function is null (invocation will throw).
    Function() = default;

    // Construct a function from a callable.
    // The callable must be trivially copyable and trivially destructible.
    // The callable must not exceed the size and alignment of this function.
    template <typename CallableArg, typename Callable = std::decay_t<CallableArg> 
        //, REQUIRES(!std::is_same<Function, Callable>::value)
        > requires(!std::same_as<Function, Callable>)
    Function(CallableArg&& callable) : executor_(executor<Callable>)
    {
        static_assert(sizeof(Callable) <= Size, "Callable size is too large, increase size of the space type");
        static_assert(alignof(Callable) <= Alignment, "Callable alignment is too large, increase alignment of the space type");
        static_assert(std::is_trivially_destructible<Callable>::value, "Use Function for non-trivially-destructible callables");
        static_assert(std::is_trivially_copyable<Callable>::value, "Use Function for non-trivially-copyable callables");

        ::new ( static_cast<void*>(space_) ) Callable( std::forward<CallableArg>(callable) );
    }

    // The call operator invokes the callable.
    template <typename... CallArgs>
    Res operator()(CallArgs&&... callargs) const {
        return this->executor_( std::forward<CallArgs>(callargs)..., const_cast<Function*>(this) );
    }

}; // Function

template<size_t Size, size_t Alignment, typename Res, typename... Args>
constexpr typename Function<Res(Args...), Size, Alignment>::executor_t Function<Res(Args...), Size, Alignment>::default_executor_;

using F = int(int, int, int, int);

using FF = Function<F>;


int sum4(int x1, int x2, int x3, int x4) {
    return x1 + x2 + x3 + x4;
}

class Summer {

    public:

        Summer(int val = 0) : add_(val) {}

        int operator()(int x1, int x2, int x3, int x4) {

            return x1 + x2 + x3 + x4 + add_;
        }

    private:

        int add_ ;
};


int main() {

    FF f1 = [](int x1, int x2, int x3, int x4) {
        return x1 + x2 + x3 + x4;
    };

    std::cout << typeid(f1).name() << std::endl;
    std::cout << f1(1,2,3,4) << std::endl;

    
    //f(1,2,3,4);

    FF f2 = &sum4;

    std::cout << typeid(f2).name() << std::endl;
    std::cout << f2(1,2,3,4) << std::endl;


    std::cout << typeid(sum4).name() << std::endl;
    std::cout << typeid(&sum4).name() << std::endl;

    
    FF f3{ Summer{} } ;
    std::cout << f3(1,2,3,4) << std::endl;


    std::vector<FF> funcs {f1, f2, f3} ;

    for ( auto& func : funcs ) {

        std::cout << func(1,2,3,4) << std::endl;
    }

    return 0;
}
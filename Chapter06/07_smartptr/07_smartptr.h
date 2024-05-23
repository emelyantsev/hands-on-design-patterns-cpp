#include <type_traits>
#include <new>
#include <memory>

template <typename T, typename Deleter>
class smartptr {

  public:

  smartptr(T* p, Deleter d) : p_(p), d_(d) {}
  ~smartptr() { d_(p_); }
  T* operator->() { return p_; }
  const T* operator->() const { return p_; }

  private:
  T* p_;
  Deleter d_;
};


template <typename T>
class smartptr_te {

  struct destroy_base {
    virtual void operator()(void*) = 0;
    virtual ~destroy_base() {}
  };

  template <typename Deleter> struct destroy : public destroy_base {
    destroy(Deleter d) : d_(d) {}
    void operator()(void* p) override { d_(static_cast<T*>(p)); }
    Deleter d_;
  };

  public:
  template <typename Deleter> smartptr_te(T* p, Deleter d) : p_(p), d_(new destroy<Deleter>(d)) {}
  ~smartptr_te() { (*d_)(p_); delete d_; }
  
  T* operator->() { return p_; }
  const T* operator->() const { return p_; }
  
  private:
  T* p_;
  destroy_base* d_;
};


struct deleter {
  template <typename T> void operator()(T* p) { delete p; }
};
template <typename T>
struct deleter1 {
  void operator()(T* p) { delete p; }
};

static deleter d;

int* get_raw_ptr();
std::unique_ptr<int, deleter> get_unique_ptr();
std::shared_ptr<int> get_shared_ptr();
std::shared_ptr<int> make_shared_ptr();
smartptr<int, deleter> get_smartptr();
smartptr_te<int> get_smartptr_te();
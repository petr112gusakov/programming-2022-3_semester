#include<iostream>
//#include <memory>
template <typename>
class enable_shared_from_this;
template <typename T>
class shared_ptr {
private:
	template<typename T>
	struct contr_block {
		T value;
		size_t c ;
		size_t w;
	};
private:
	template<typename T>
	friend class weak_ptr;
	template<typename T>
	friend class enable_shared_from_this;
	template <typename T,typename ...Args>
	friend shared_ptr<T> make_shared(Args&&... args);
	size_t* count;
	T* ptr;
	contr_block<T>* cb;
public:
	shared_ptr():ptr(nullptr), count(nullptr),cb(nullptr){}
	shared_ptr(T *ptr1): count(new size_t(1)),ptr(ptr1),cb(nullptr) {
		if constexpr (std::is_base_of_v<enable_shared_from_this<T>, T>) {
			ptr->weak = *this;
		}
	}
	shared_ptr(const shared_ptr& Other): ptr(Other.ptr),count(Other.count),cb(Other.cb){
		++(*count);
	}
	shared_ptr(contr_block<T> *contr): ptr(&(contr->value)), count(&(contr->c)),cb(contr){
		++(*count);
		if constexpr (std::is_base_of_v<enable_shared_from_this<T>, T>) {
			ptr->weak = *this;
		}
	}
	shared_ptr& operator=(const shared_ptr& Other)   {
		ptr = nullptr;
		count = nullptr;
		ptr = Other.ptr;
		count = Other.count;
		++(*count);
		return (*this);
	}
	shared_ptr& operator=(const shared_ptr&& Other) {
		ptr = nullptr;
		count = nullptr;
		ptr = Other.ptr;
		count = Other.count;
		++(*count);
		Other.ptr = nullptr;
		Other.count = nullptr;
		return (*this);
	}
	shared_ptr(shared_ptr && Other) noexcept: ptr(Other.ptr), count(Other.count){
		Other.ptr=nullptr;
		Other.count = nullptr;
	}
	~shared_ptr() {
		
		if (!count) return;
		--(*count);
		if ((cb!=nullptr)&& (*count == 0) ) {
			delete (ptr);
			if (cb->w == 0) {
				delete (cb);
				return;
			}
		}
		if ((cb==nullptr) &&(*count == 0)) {
			if (ptr != nullptr) {
				delete ptr;
			}
			delete count;
		}
	}
	T operator *() {
		return *ptr;
	}
};
template <typename T, typename ...Args>
shared_ptr<T> make_shared(Args&&... args) {
	return shared_ptr<T>(new typename shared_ptr<T>::template contr_block<T>{std::forward<Args>(args)...,0});
}
template<typename T>
class weak_ptr {
	shared_ptr<T>::contr_block<T>* cb;
	T* ptr;
public:
	bool expired() const {
		return cb && cb->c == 0;
	}
	weak_ptr(const shared_ptr<T> & p):cb(p.cb),ptr(p.ptr){}
	~weak_ptr() {
		if (!cb) return;
		--cb->w;
		if (cb->w == 0 && cb->c == 0) {
			::operator delete(cb);
		}
	}
	shared_ptr<T> lock() const {
		if (!this->expired()) throw std::bad_weak_ptr();
		if (cb!=nullptr) {
			return shared_ptr<T>(cb);
		}
		else {
			return shared_ptr<T>(ptr);
		}
	}
};
template<typename T>
class enable_shared_from_this {
public:
	weak_ptr<T> weak ;
protected:
	shared_ptr<T> shared_from_this() const {
		if ((weak.expired())==false) throw std::bad_weak_ptr();
		return weak.lock();
	}
};
int main() {
	shared_ptr<int>pp=make_shared<int>(23);
	shared_ptr<int> p(new int(67));
	weak_ptr<int> w(pp);
	std::cout << *pp<<" "<<*p;


}
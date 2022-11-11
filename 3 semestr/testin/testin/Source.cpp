/*#include<iostream>
#include<memory>


class any {
	struct Base {
		virtual Base* copy() = 0;
		virtual~Base() = default;
	};
	template <typename U>
	struct Derived :public Base {
		U object;
		Derived(const U& o):object(o){}
		Base* copy() {
			return new Derived<U>(object);
		}
	};
	Base* ptr = nullptr;
public:
	template<typename T>
	any(const T& obj):ptr(new Derived<T>(obj)){}
	template<typename T>
	auto& operator =(const T& object) {
		delete ptr;
		ptr = new Derived<T>(object);
		return(*this);

	}
	template<typename T>
	any(const any& a ): ptr(a.ptr->copy){}
	~any() {
		delete ptr;
	}

};
int main() {
	any a(23);
	a = 3.56;
	a = 'd';
}*/
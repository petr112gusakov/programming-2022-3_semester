#include <iostream>
#include <memory>
#include <vector>
#define capacyti cap
#define my_vector vec


template <typename T, typename aloc=std::allocator<T>, typename al_traits= std::allocator_traits<aloc> >
class my_vector {
private:
	size_t siz;
	size_t capacyti;
	aloc al;
	T* arr;
public:
	vec() = default;
	vec(const size_t& size, const T& value = T()):siz(size), cap(size) {
		arr = al_traits::allocate(al,cap);
		for (size_t i = 0; i < siz; ++i) {
			arr[i] = value;
		}
	}
	vec (const vec& v): siz(v.siz),cap(v.cap),al(std::move(v.al)),arr(std::move(v.arr)) {}
	vec(const std:: initializer_list<T>& list):siz(list.size()),cap(list.size()){
		arr = al_traits::allocate(al, cap);
		for (size_t i = 0; i < siz; ++i) {
			arr[i] = *(list.begin()+i);
		}
	}
    vec(const vec&& v) noexcept : siz(v.siz), cap(v.cap), al(std::move(v.al)), arr(v.arr){}
	const T& operator[](const size_t i) const {
		return arr[i];
	}
	vec& operator=(const vec& v) {
		al_traits::destroy(al,arr);
		al = std::move(v.al);
		siz = v.siz;
		cap = v.cap;
		arr = v.arr;
		return *(this);
	}
	const T& at(const size_t i) const {
		if (i >= siz) throw std::out_of_range("Indecs is out of vector size");
		return arr[i];
	}
	size_t size() { return siz; }
	size_t capacity() { return cap; }
	void resize(const size_t& size, const T& value = T()){
		if (cap < size) {
			(*this).reserve(size );
		}
		while(siz<size){
			al_traits::construct(al, arr +siz, std::move_if_noexcept(value));
			++siz;
		}
	}
	~vec() {
		al_traits::deallocate(al, arr , cap);
	}
	void reserve(const size_t& new_cap) {
		if (cap >= new_cap) return;
		T* newarr = al_traits::allocate(al, new_cap);
		for (size_t i = 0; i < siz; ++i) {
			al_traits::construct(al, newarr + i, std::move_if_noexcept(arr[i]));
		}
		for (size_t i = 0; i < siz; ++i) {
			al_traits::destroy(al, arr + i);
		}
		al_traits::deallocate(al,arr, cap);
		arr = newarr;
		cap = new_cap;
	}
	void push_back(const T& value) {
		if (siz >= cap) {
			(*this).reserve(cap * 2);
		}
		al_traits::construct(al, arr + siz, std::move_if_noexcept(value));
		++siz;
		
	}
	template <typename... Args>
	void emplace_back(Args&&... args){
		if (siz >= cap) {
			(*this).reserve(cap * 2);
		}
		al_traits::construct(al, arr + siz, std::forward<Args>(args)...);
		++siz;
	}
	void pop_back() {
		al_traits::destroy(al, arr + siz);
		--siz;
	}
	void shrink_to_fit(){
		if (cap == siz) return;
		T* newarr = al_traits::allocate(al, siz);
		for (size_t i = 0; i < siz; ++i) {
			al_traits::construct(al, newarr + i, std::move_if_noexcept(arr[i]));
		}
		for (size_t i = 0; i < siz; ++i) {
			al_traits::destroy(al, arr + i);
		}
		al_traits::deallocate(al, arr, cap);
		arr = newarr;
		cap = siz;
	}
	struct iterator {
		T* ptr;
	public:
		iterator():ptr(NULL){}
		T& operator*() const {
			return (*ptr);
		}
		iterator& operator =(const iterator& iter ) {
			al_traits::destroy(al,ptr);
			ptr = iter.ptr;
			return(*this);
		}
		iterator (const iterator && it) noexcept : ptr(it.ptr){}
		iterator (const iterator& it): ptr(it.ptr){}
		iterator operator ++() {
			++ptr;
			return *this;
		}
		iterator operator --() {
			--ptr;
			return *this;
		}
		bool operator!=(const iterator& it) {
			return (it.ptr != (*this).ptr);
		}
	};
	iterator begin() {
		iterator it;
		it.ptr = arr;
		return it;
	}
	iterator end() {
		iterator it;
		it.ptr = arr + siz;
		return it;
	}
	void insert(const iterator& other, const T& value) {
		iterator it = std::move(other);
		int count = 0;
		while (it.ptr != arr +count) {
			++count;
		}
		if (siz >= cap) {
			(*this).resize(siz + 1);
		}
		int i =0;
		it.ptr = arr + count;
		*(it.ptr) = value;
		++it.ptr;
		++it.ptr;
		for (it; it != (*this).end(); ++it) {
			++i;
			*it.ptr = arr[count + i];
			
		}
		
	}
	void erase(const iterator& other) {
		iterator it = std::move(other);
		int i = 0;
		int count = 0;
		while (it.ptr != arr + count) {
			++count;
		}
		++it;
		al_traits::destroy(al, arr + count);
		for (it; it != (*this).end(); ++it.ptr) {
			arr[count+i] = *it.ptr;
			++i;
		}
		--siz;

	}
};
template<>
class vec<bool> {
private:
	uint8_t* arr;
	size_t siz;
	size_t cap;
	struct BitReference {
		uint8_t* cell;
		uint8_t bit;
		BitReference(uint8_t index, uint8_t * cell): bit(index),cell(cell) {}
		BitReference& operator=(const bool& b) {
			size_t Char_index = bit / 8;
			size_t bit_index = bit % 8;
			if (b) {
				cell[Char_index] |= 1 << bit_index;
			}
			else
				cell[Char_index] &= ~(1 << bit_index);
			return *this;
		}
		operator bool() const {
			return *cell & (1 << bit);
		}
		BitReference& operator=(const BitReference&) = default;
	};
public:
	BitReference operator[](size_t index) {
		return BitReference(index%8,arr+index/8);
	}
};
int main() {
	
	vec<int> vv = {1,2,3,6,7,8,9};
	vec <int>::iterator it=vv.begin();
	++it;
	++it;
	++it;
	int a = 9;
	vv.emplace_back(a);
	for (int el : vv) {
		std::cout << el<<' ';
	}
	std::cout << vv.size()<<" " << vv.capacity();
}
#ifndef VECTOR_H
#define VECTOR_H
#include<iostream>
#include<initializer_list>
#include<stdexcept>

template<typename T>
class Vector {
public:
	class ConstIterator;
	class Iterator;
	using value_type = T;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using iterator = Vector::Iterator;
	using const_iterator = Vector::ConstIterator;
private:
	size_type max_sz;
	size_type sz;
	pointer values;
public:
	Vector() : max_sz{0}, sz{0}, values{nullptr} {}
	Vector(const Vector& v): Vector(v.sz) {
		for(; sz < v.sz; sz++) {
			values[sz] = v.values[sz];
		}
	}
	Vector(size_type n) : max_sz{n}, sz{0}, values(new value_type[max_sz]) {}
	Vector(const std::initializer_list<value_type>& list): max_sz{list.size()}, sz{0}, values(new value_type[list.size()]) {
		for(auto l : list) {
			values[sz++] = l;
		}
	}
	~Vector() {
		delete[] values;
	}
	Vector& operator=(Vector v) {
		std::swap(sz, v.sz);
		std::swap(max_sz, v.max_sz);
		std::swap(values, v.values);
		return *this;
	}
	size_type size() const {
		return sz;
	}
	bool empty() const {
		return (sz == 0);
	}
	void clear() {
		sz = 0;
	}
	void reserve(size_type n) {
		if (n > max_sz) {
			value_type* new_values = new value_type[n];
			for (size_type i{0}; i < sz; ++i) {
				new_values[i] = values[i];
			}
			max_sz = n;
			delete[] values;
			values = new_values;
		}
	}
	void shrink_to_fit() {
		max_sz = sz;
	}
	void push_back(const_reference val) {
		if (sz >= max_sz) {
			reserve(2*max_sz+1);
		}
		values[sz++] = val;
	}
	void pop_back() {
		if(this->empty()) {
			throw std::runtime_error("Vector is empty!\n");
		}
		--sz;
	}
	reference operator[](size_type index) {
		if((index < 0) || (sz <= index)) {
			throw std::runtime_error("Index is out of bound!\n");
		}
		return values[index];
	}
	const_reference operator[](size_type index) const {
		if((index < 0) || (sz <= index)) {
			throw std::runtime_error("Index is out of bound!\n");
		}
		return values[index];
	}
	size_type capacity() const {
		return max_sz;
	}
	iterator begin() {
		if(this->empty()) {
			return end();
		}
		return iterator{values, values+sz};
	}
	iterator end() {
		return iterator{values+sz, values+sz};
	}
	const_iterator begin() const {
		if(this->empty()) {
			return end();
		}
		return const_iterator{values, values+sz};
	}
	const_iterator end() const {
		return const_iterator{values+sz, values+sz};
	}
	iterator insert(const_iterator pos, const_reference val) {
		auto diff = pos - begin();
		if(diff < 0 || static_cast<size_type>(diff) > sz) {
			throw std::runtime_error("Iterator out of bound!\n");
		}
		size_type current{static_cast<size_type>(diff)};
		if(sz >= max_sz) {
			reserve(max_sz*2+1);
		}
		for(auto i{sz}; i-->current;) {
			values[i+1] = values[i];
		}
		values[current] = val;
		++sz;
		return iterator{values + current};
	}
	iterator erase(const_iterator pos) {
		auto diff = pos - begin();
		if(diff < 0|| static_cast<size_type>(diff) >= sz) {
			throw std::runtime_error("Iterator out of bound!\n");
		}
		size_type current{static_cast<size_type>(diff)};
		for(auto i{current}; i < sz-1; i++) {
			values[i] = values[i+1];
		}
		--sz;
		return iterator{values + current};
	}
	std::ostream& print(std::ostream& o) const {
		o << '[';
		bool first{true};
		for(size_type i{0}; i < sz; i++) {
			if(first) {
				first = false;
			}
			else {
				o << ", ";
			}
			o << values[i];
		}
		o << ']' << "\n" << max_sz << "\n";
		return o;
	}
	friend std::ostream& operator<<(std::ostream& o, const Vector& v) {
		return v.print(o);
	}
	class Iterator {
		public:
			using value_type = Vector::value_type;
			using reference = Vector::reference;
			using pointer = Vector::pointer;
			using difference_type = Vector::difference_type;
			using iterator_category = std::forward_iterator_tag;
		private:
			//Instanzvariablen
			pointer ptr;
			pointer end;
		public:
			//Methoden
			Iterator() : ptr{nullptr}, end{nullptr} {}
			Iterator(pointer ptr) : ptr{ptr} {}
			Iterator(pointer begin, pointer end) : ptr{begin}, end{end} {}
			reference operator*() { //Retourniert den Wert des von ptr referenzierten Wertes
				if(ptr==end) {
					throw std::runtime_error("Error");	
				}
				return (*ptr);				
			}
			pointer operator->() { //returniert ein zeiger auf den referenzierten Wert
				if(ptr==end) {
					throw std::runtime_error("Error");	
				}
				return ptr;
			}
			bool operator==(const const_iterator& ci) const {
				return (static_cast<const_iterator>(*this) == ci);
			}
			bool operator!=(const const_iterator& ci) const {
				return (!(static_cast<const_iterator>(*this) == ci));
			}
			iterator& operator++() { //Prefix
				if(ptr != end) {
					++ptr;
				}
				return *this;
			}
			iterator operator++(int) { //Postfix
				iterator temp(ptr);
				if(ptr == end) {
					return *this;
				}
				ptr++;
				return temp;
			}
			operator const_iterator() const {
				return ConstIterator(ptr);
			}
			/*friend Vector::difference_type operator-(const Vector::ConstIterator& lop, const Vector::ConstIterator& rop) {
				return (lop.ptr - rop.ptr);
			}*/
	};
	class ConstIterator {
		public:
			using value_type = Vector::value_type;
			using reference = Vector::const_reference;
			using pointer = Vector::const_pointer;
			using difference_type = Vector::difference_type;
			using iterator_category = std::forward_iterator_tag;
		private:
			//Instanzvariablen
			const_pointer ptr;
			const_pointer end;
		public:
			//Methoden
			ConstIterator() : ptr{nullptr}, end{nullptr} {}
			ConstIterator(pointer ptr) : ptr{ptr} {}
			ConstIterator(pointer begin, pointer end) : ptr{begin}, end{end}{}
			reference operator*() const {
				if(ptr==end) {
					throw std::runtime_error("Error");	
				}
				return (*ptr);				
			}
			pointer operator->() const {
				if(ptr==end) {
					throw std::runtime_error("Error");	
				}	
				return ptr;
			}
			bool operator==(const const_iterator& ci) const {
				return (this->ptr == ci.ptr);
			}
			bool operator!=(const const_iterator& ci) const {
				return (!(this->ptr == ci.ptr));
			}
			const_iterator& operator++() {
				if(ptr != end) {
					++ptr;
				}
				return *this;
			}
			const_iterator operator++(int) { 
				const_iterator temp(ptr);
					if(ptr == end) {
						return temp;
					}
				ptr++;
				return temp;
			}
			friend Vector::difference_type operator-(const Vector::ConstIterator& lop, const Vector::ConstIterator& rop) {
				return (lop.ptr - rop.ptr);
			}
	};
};
#endif

#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

#include <iostream>
#include <exception>
#include "vector.hpp"

template<typename T, size_t ALLOC_SIZE>
class Allocator {
public:
	using value_type = T;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using is_always_equal = std::false_type;

	template<typename U>
	struct rebind {
		using other = Allocator<U, ALLOC_SIZE>;
	};

	Allocator() : begin{new char[ALLOC_SIZE]},
	end{begin + ALLOC_SIZE}, tail{begin} {}

	Allocator(const Allocator&) = delete;
	Allocator(Allocator &&) = delete;

	~Allocator() {
		delete [] begin;
		begin = end = tail = nullptr;
		freeBlocks.~Vector();
	}

	T *Allocate(size_t n) {
		if (n != 1) {
			throw std::logic_error("error: This allocator can't allocate arrays");
		}
		if (end - tail < sizeof(T)) {
			if (!freeBlocks.Empty()) {
				char *ptr = freeBlocks.Back();
				freeBlocks.PopBack();
				return reinterpret_cast<T *>(ptr);
			}
			throw std::bad_alloc();
		}
		T *result = reinterpret_cast<T *>(tail);
		tail += sizeof(T);
		return result;
	}

	void deallocate(T *ptr, size_t n) {
		if (n != 1) {
			throw std::logic_error("error: This allocator can't deallocate arrays");
		}
		if (ptr == nullptr) {
			return;
		}
		freeBlocks.PushBack(reinterpret_cast<char *>(ptr));
	}

private:
	char *begin;
	char *end;
	char *tail;
	Vector<char *> freeBlocks;
};

#endif /* ALLOCATOR_HPP */
#ifndef LIST_HPP
#define LIST_HPP

#include <iterator>
#include <memory>

template<class T, class Allocator = std::allocator<T>>
class List {
private:
    struct Element;  
    size_t size = 0; // size of the list
public:
    List() = default; 

    class forward_iterator {
    public:
        using value_type = T;
        using reference = value_type &;
        using pointer = value_type *;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;
        
        explicit forward_iterator(Element *ptr);
        T &operator*();
        forward_iterator &operator++();
        forward_iterator operator++(int);
        bool operator==(const forward_iterator &other) const;
        bool operator!=(const forward_iterator &other) const;

    private:
        Element *it_ptr;
        friend List;
    };

    forward_iterator begin();
    forward_iterator end();
    void PushBack(const T &value); // Add element at the beginning of the list
    void PushFront(const T &value); // Add element to the end of the list
    T &Front(); //Get element from the beginning of the list
    T &Back(); //Get the element from the end of the list
    void PopBack(); //Remove element from the end of the list
    void PopFront(); //Remove element from the beginning of the list
    size_t Length(); //Get size of the list
    bool Empty(); //Check emptiness of the list
    void EraseByIterator(forward_iterator d_it); //Remove element by iterator
    void EraseByNumber(size_t N); //Remove element by number
    void InsertByIterator(forward_iterator ins_it, T &value); //Add element by iterator
    void InsertByNumber(size_t N, T &value); //Add element by number
    List& operator=(List& other);
    T &operator[](size_t index);

private:
    using allocator_type = typename Allocator::template rebind<Element>::other;

	struct deleter {
	private:
		allocator_type* allocator_;
	public:
		deleter(allocator_type* allocator) : allocator_(allocator) {}

		void operator() (Element* ptr) {
			if (ptr != nullptr) {
				std::allocator_traits<allocator_type>::destroy(*allocator_, ptr);
				allocator_->deallocate(ptr, 1);
			}
		}

	};
    using unique_ptr = std::unique_ptr<Element, deleter>;
    struct Element {
        T value;
        unique_ptr next_element = { nullptr, deleter{nullptr} };
		Element* prev_element = nullptr;
		Element(const T& value_) : value(value_) {}
		forward_iterator Next();
    };

    allocator_type allocator_{};
	unique_ptr first{ nullptr, deleter{nullptr} };
    Element *tail = nullptr;
};

template<class T, class Allocator>
typename List<T, Allocator>::forward_iterator List<T, Allocator>::begin() {
    return forward_iterator(first.get());
}

template<class T, class Allocator>
typename List<T, Allocator>::forward_iterator List<T, Allocator>::end() {
    return forward_iterator(nullptr);
}

template<class T, class Allocator>
size_t List<T, Allocator>::Length() {
    return size;
}

template<class T, class Allocator>
bool List<T, Allocator>::Empty() {
    return Length() == 0;
}

template<class T, class Allocator>
void List<T, Allocator>::PushBack(const T& value) {
	Element* result = this->allocator_.allocate(1);
	std::allocator_traits<allocator_type>::construct(this->allocator_, result, value);
	if (!size) {
		first = unique_ptr(result, deleter{ &this->allocator_ });
		tail = first.get();
		size++;
		return;
	}
	tail->next_element = unique_ptr(result, deleter{ &this->allocator_ });
	Element* temp =  tail;
	tail = tail->next_element.get();
	tail->prev_element = temp;
	size++;
}

template<class T, class Allocator>
void List<T, Allocator>::PushFront(const T& value) {
	size++;
	Element* result = this->allocator_.allocate(1);
	std::allocator_traits<allocator_type>::construct(this->allocator_, result, value);
	unique_ptr tmp = std::move(first);
	first = unique_ptr(result, deleter{ &this->allocator_ });
	first->next_element = std::move(tmp);
	if(first->next_element != nullptr)
		first->next_element->prev_element = first.get();
	if (size == 1) {
		tail = first.get();
	}
	if (size == 2) {
		tail = first->next_element.get();
	}
}

template<class T, class Allocator>
void List<T, Allocator>::PopFront() {
	if (size == 1) {
		first = nullptr;
		tail = nullptr;
		size--;
		return;
	}
	unique_ptr tmp = std::move(first->next_element);
	first = std::move(tmp);
	first->prev_element = nullptr;
	size--;
}

template<class T, class Allocator>
void List<T, Allocator>::PopBack() {
	if (tail->prev_element){
		Element* tmp = tail->prev_element;
		tail->prev_element->next_element = nullptr;
		tail = tmp;
	}
	else{
		first = nullptr;
		tail = nullptr;
	}
	size--;
}

template<class T, class Allocator>
T& List<T, Allocator>::Front() {
	if (size == 0) {
		throw std::logic_error("error: list is empty");
	}
	return first->value;
}

template<class T, class Allocator>
T& List<T, Allocator>::Back() {
	if (size == 0) {
		throw std::logic_error("error: list is empty");
	}
	forward_iterator i = this->begin();
	while ( i.it_ptr->Next() != this->end()) {
		i++;
	}
	return *i;
}

template<class T, class allocator>
List<T,allocator>& List<T, allocator>::operator=(List<T, allocator>& other) {
	size = other.size;
	first = std::move(other.first);
}

template<class T, class Allocator>
void List<T, Allocator>::EraseByIterator(List<T, Allocator>::forward_iterator d_it) {
	forward_iterator i = this->begin(), end = this->end();
	if (d_it == end) throw std::logic_error("error: out of range");
	if (d_it == this->begin()) {
		this->PopFront();
		return;
	}
	if (d_it.it_ptr == tail) {
		this->PopBack();
		return;
	}

	if (d_it.it_ptr == nullptr) throw std::logic_error("error: out of range");
	auto temp = d_it.it_ptr->prev_element;
	unique_ptr temp1 = std::move(d_it.it_ptr->next_element);
	d_it.it_ptr->prev_element->next_element = std::move(temp1);
	d_it.it_ptr = d_it.it_ptr->prev_element;
	d_it.it_ptr->next_element->prev_element = temp;

	size--;
}



template<class T, class Allocator>
void List<T, Allocator>::EraseByNumber(size_t N) {
	forward_iterator it = this->begin();
	for (size_t i = 0; i < N; ++i) {
		++it;
	}
	this->EraseByIterator(it);
}

template<class T, class Allocator>
void List<T, Allocator>::InsertByIterator(List<T, Allocator>::forward_iterator ins_it, T& value) {
	Element* tmp = this->allocator_.allocate(1);
	std::allocator_traits<allocator_type>::construct(this->allocator_, tmp, value);

	forward_iterator i = this->begin();
	if (ins_it == this->begin()) {
		this->PushFront(value);
		return;
	}
	if(ins_it.it_ptr == nullptr){
		this->PushBack(value);
		return;
	}

	tmp->prev_element = ins_it.it_ptr->prev_element;
	ins_it.it_ptr->prev_element = tmp;
	tmp->next_element = unique_ptr(ins_it.it_ptr, deleter{ &this->allocator_ });
	tmp->prev_element->next_element = unique_ptr(tmp, deleter{ &this->allocator_ });

	size++;
}

template<class T, class Allocator>
void List<T, Allocator>::InsertByNumber(size_t N, T& value) {
	forward_iterator it = this->begin();
	if (N >= this->Length())
		it = this->end();
	else
		for (size_t i = 0; i < N; ++i) {
			++it;
		}
	this->InsertByIterator(it, value);
}

template<class T, class allocator>
typename List<T,allocator>::forward_iterator List<T, allocator>::Element::Next() {
	return forward_iterator(this->next_element.get());
}

template<class T, class Allocator>
List<T, Allocator>::forward_iterator::forward_iterator(List<T, Allocator>::Element *ptr) {
	it_ptr = ptr;
}

template<class T, class Allocator>
T& List<T, Allocator>::forward_iterator::operator*() {
	return this->it_ptr->value;
}


template<class T, class Allocator>
T& List<T, Allocator>::operator[](size_t index) {
	if (index < 0 || index >= size) {
		throw std::logic_error("error: out of range");
	}
	forward_iterator it = this->begin();
	for (size_t i = 0; i < index; i++) {
		it++;
	}
	return *it;
}

template<class T, class Allocator>
typename List<T, Allocator>::forward_iterator& List<T, Allocator>::forward_iterator::operator++() {
	if (it_ptr == nullptr) {
        throw std::logic_error("error: out of range");
    }
	*this = it_ptr->Next();
	return *this;
}

template<class T, class Allocator>
typename List<T, Allocator>::forward_iterator List<T, Allocator>::forward_iterator::operator++(int) {
	forward_iterator old = *this;
	++*this;
	return old;
}

template<class T, class Allocator>
bool List<T, Allocator>::forward_iterator::operator==(const forward_iterator& other) const {
	return it_ptr == other.it_ptr;
}

template<class T, class Allocator>
bool List<T, Allocator>::forward_iterator::operator!=(const forward_iterator& other) const {
	return it_ptr != other.it_ptr;
}

#endif /* LIST_HPP */
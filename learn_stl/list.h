#pragma once 

#include <memory>

namespace learn {

template <typename ValueT, typename AllocatorT = std::allocator<ValueT>> 
class list {
   public:
	using value_type = ValueT;
	using allocator_type = AllocatorT;
   	using pointer = value_type*;
	using const_pointer = const value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

	struct node {
                using value_type = typename list::value_type;
                using reference = typename list::reference;
		using pointer = typename list::pointer;
		using difference_type = typename list::difference_type; 

                node* parent_ = nullptr;
                std::unique_ptr<node> child_;

                value_type value_;
        };

	template <typename NodeT, typename IterValueT>
	class iterator_wrapper {
		public:
			using node_type = NodeT;
			using iterator_category = std::bidirectional_iterator_tag;
			using value_type = IterValueT;
			using reference = value_type&;
			using pointer = value_type*;
			using difference_type = typename node_type::difference_type;

			iterator_wrapper(node_type* ptr) : ptr_(ptr) {}

			reference operator*() const { return ptr_->value_; }
			pointer operator->() const { return addressof(ptr_->value_); }
			
			iterator_wrapper& operator++();
			iterator_wrapper operator++(int);
			iterator_wrapper& operator--();
			iterator_wrapper operator--(int);
			
			bool operator==(const iterator_wrapper& other) const { return ptr_ == other.ptr_; }
			bool operator!=(const iterator_wrapper& other) const { return !(*this == other); }
		private:
			node_type* ptr_ = nullptr;
	};

	using iterator = iterator_wrapper<node, value_type>;
	using const_iterator = iterator_wrapper<const node, const value_type>;

	// constructors	
	explicit list(const allocator_type& alloc = allocator_type());

	list(const list& other) {
		for (const auto& value: other) {
			emplace_back(value);
		}
	}

	// destructor
	~list() = default;

	// accessors
	
	// iterators
	iterator begin() noexcept { return head_.get(); }
	iterator end() noexcept { return tail_; }

	const_iterator begin() const noexcept { return head_.get(); }
	const_iterator end() const noexcept { return tail_; } 

	// capacity 
	bool empty() const noexcept { return size() == 0; }
	size_type size() const noexcept { return std::distance(begin(), end()); }

	// modifiers 
	template <class ...Args>
	reference emplace_back(Args&& ...args);
	
   private:
	std::unique_ptr<node> head_;
	node* tail_ = nullptr;
};

template <typename ValueT, typename AllocatorT> 
template <typename NodeT, typename IterValueT> 
typename list<ValueT, AllocatorT>::template iterator_wrapper<NodeT, IterValueT>& list<ValueT, AllocatorT>::iterator_wrapper<NodeT, IterValueT>::operator++() {
	if (ptr_) {
		ptr_ = ptr_->child_.get();
	}

	return *this;
}

template <typename ValueT, typename AllocatorT>
template <typename NodeT, typename IterValueT>
typename list<ValueT, AllocatorT>::template iterator_wrapper<NodeT, IterValueT> list<ValueT, AllocatorT>::iterator_wrapper<NodeT, IterValueT>::operator++(int n) {
	iterator_wrapper other(*this);

	this->operator++();
	
	return other;
}

template <typename ValueT, typename AllocatorT>
template <typename NodeT, typename IterValueT>
typename list<ValueT, AllocatorT>::template iterator_wrapper<NodeT, IterValueT>& list<ValueT, AllocatorT>::iterator_wrapper<NodeT, IterValueT>::operator--() {
	if (ptr_) {
		ptr_ = ptr_->parent_;
	}

	return *this;
}

template <typename ValueT, typename AllocatorT>
template <typename NodeT, typename IterValueT>
typename list<ValueT, AllocatorT>::template iterator_wrapper<NodeT, IterValueT> list<ValueT, AllocatorT>::iterator_wrapper<NodeT, IterValueT>::operator--(int n) {
	iterator_wrapper other(*this);
	
	this->operator--();

	return other;
}


template <typename ValueT, typename AllocatorT>
list<ValueT, AllocatorT>::list(const allocator_type& alloc) {}

template <typename ValueT, typename AllocatorT>
template <class ...Args>
typename list<ValueT, AllocatorT>::reference list<ValueT, AllocatorT>::emplace_back(Args&& ...args) {
	auto new_node = std::make_unique<node>(node{tail_, nullptr, value_type(std::forward<Args>(args)...)});

	if (!tail_) {
		head_ = std::move(new_node);
		tail_ = head_.get();
	} else {
		tail_->child_ = std::move(new_node);
		tail_ = tail_->child_.get();
	}	

	return tail_->value_;
}

} // namespace learn

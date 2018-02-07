#pragma once

#include <forward_list>
#include <utility>

namespace learn {
namespace detail {

template <class ObjectT, class HashT, class EqualT, class AllocatorT>
class Unordered {
  public:
    using Object = ObjectT;
    using Hash = HashT;
    using Equal = EqualT;
    using Allocator = AllocatorT;
    using SizeType = std::size_t;
    using List = std::forward_list<Object, Allocator>;
    using Iterator = typename List::iterator;
    using ConstIterator = typename List::const_iterator;

    explicit Unordered(SizeType bucket_count, const Hash& hash = Hash(),
                       const Equal& equal = Equal(), const Allocator& alloc = Allocator())
        : hash_(hash),
          equal_(equal),
          allocator_(alloc),
          list_(allocator_),
          bins_(bucket_count, Bin{list_.end(), list_.end()}) {}

    // iterators
    Iterator begin() noexcept { return list_.begin(); }
    Iterator end() noexcept { return list_.end(); }

    ConstIterator begin() const noexcept { return list_.begin(); }
    ConstIterator end() const noexcept { return list_.end(); }

    // capacity
    bool empty() const noexcept { return list_.empty(); }
    SizeType size() const noexcept { return list_.size(); }
    SizeType max_size() const noexcept { return list_.max_size(); }

    template <class... Args>
    std::pair<Iterator, bool> emplace(Args&&... args);

  private:
    struct Bin {
        typename List::iterator front = nullptr;
        typename List::iterator end = nullptr;
    };

    using Bins = std::vector<Bin>;

    Hash hash_;
    Equal equal_;
    Allocator allocator_;
    List list_;
    Bins bins_;
};

template <class ObjectT, class HashT, class EqualT, class AllocatorT>
template <class... Args>
std::pair<typename Unordered<ObjectT, HashT, EqualT, AllocatorT>::Iterator, bool>
Unordered<ObjectT, HashT, EqualT, AllocatorT>::emplace(Args&&... args) {
    Object object(args...);

    const auto bin_index = Hash()(object) % bins_.size();
    auto& bin = bins_[bin_index];

    Iterator iterator = bin.front;
    bool inserted = false;

    for (auto iterator = bin.front; iterator != bin.end; ++iterator) {
        if (Equal()(*iterator, object)) {
            return std::make_pair(iterator, false);
        }
    }

    bin.end = std::next(list_.insert_after(bin.end, std::move(object)));

    return std::make_pair(list_.end(), false);
}

}  // namespace detail
}  // namespace learn
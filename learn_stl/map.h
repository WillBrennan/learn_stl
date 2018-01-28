#include <memory>
#include <utility>

#include "detail/tree.h"

namespace learn {
template <class Key, class Value, class Compare = std::less<Key>,
          class Allocator = std::allocator<std::pair<const Key, T>> class map {
  public:
  private:
    class KeyCompare {
      public:
        bool operator()(const Node& lhs, const Node& rhs) {
            return Compare()(lhs.first, rhs.first);
        }
    };

    using Node = std::pair<Key, Value>;
    using Tree = detail::Tree<Node, KeyCompare, Allocator>;
};
}  // namespace learn
#pragma once

#include <utility>

#include "learn_stl/memory.h"

namespace learn {
namespace detail {
template <typename ValueT, typename CompareT>
class Tree {
  public:
    using Value = ValueT;
    using Reference = Value&;
    using ConstReference = const Value&;
    using Compare = CompareT;

    template <class... Args>
    void emplace(Args&&... args) {
        const auto node = make_unique<Node>(std::forward<Args>(args)...));
        auto& parent = findParent(node.value);

        if (!parent) {
            parent = std::move(node);
        }
    }

  private:
    struct Node {
        template <class... Args>
        Node(Args&&... args) : value(std::forward<Args>(args)...) {}

        Value value;
        NodePtr lhs = nullptr;
        NodePtr rhs = nullptr;
    };

    using NodePtr = unique_ptr<Node>;

    NodePtr root = nullptr;

    NodePtr& findRecursive(const NodePtr& node, const Value& value) {
        if (!node || value == node->value) {
            return node;
        }

        if (Compare()(node->value, value)) {
            if (!node->lhs) {
                return node->lhs;
            }

            return findRecursive(node->lhs, value);

        } else {
            if (!node->rhs) {
                return node->rhs;
            }

            return findRecursive(node->rhs, value);
        }
    }
};

}  // namespace detail
}  // namespace learn
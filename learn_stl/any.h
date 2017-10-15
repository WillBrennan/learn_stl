#pragma once

#include <memory>
#include <type_traits>
#include <utility>

namespace learn {
class any;
namespace detail {
template <typename Object>
const Object& any_cast(const any* value);

template <typename Object>
Object& any_cast(any* value);

}  // namespace detail
class any {
  public:
    any() = default;

    template <typename Object>
    any(const Object& object)
        : container_(std::make_shared<Container<std::decay_t<Object>>>(object)) {}

    template <typename Object>
    any(Object&& object)
        : container_(
              std::make_shared<Container<std::decay_t<Object>>>(std::forward<Object>(object))) {}

    template <typename Object, typename... Args>
    std::decay_t<Object>& emplace(Args&&... args) {
        container_ = std::make_shared<Container<Object>>(std::forward<Args>(args)...);
        return static_cast<any::Container<Object>&>(*container_).data;
    }

    void reset() noexcept { container_.reset(); }
    void swap(any& other) noexcept { std::swap(container_, other.container_); }

    bool has_value() const noexcept { return bool(container_); }
    const std::type_info& type() const noexcept {
        if (has_value()) {
            return container_->type();
        }

        return typeid(void);
    }

    template <typename Object>
    friend const Object& detail::any_cast(const any* value);

    template <typename Object>
    friend Object& detail::any_cast(any* value);

  private:
    struct ContainerInterface {
        virtual ~ContainerInterface() = default;
        virtual const std::type_info& type() const = 0;
    };

    template <typename Object>
    struct Container : ContainerInterface {
        explicit Container(const Object& _data) : data(_data) {}
        explicit Container(Object&& _data) : data(std::forward<Object>(_data)) {}
        Object data;

        const std::type_info& type() const final { return typeid(data); }
    };

    std::shared_ptr<ContainerInterface> container_;
};

namespace detail {
template <typename Object>
const Object& any_cast(const any* value) {
    if (typeid(Object) != value->type()) {
        throw std::bad_cast();
    }

    return static_cast<const any::Container<Object>&>(*value->container_).data;
}

template <typename Object>
Object& any_cast(any* value) {
    if (typeid(Object) != value->type()) {
        throw std::bad_cast();
    }

    return static_cast<any::Container<Object>&>(*value->container_).data;
}
}  // namespace detail

template <typename Object>
const Object& any_cast(const any& value) {
    return detail::any_cast<Object>(&value);
}

template <typename Object>
Object& any_cast(any& value) {
    return detail::any_cast<Object>(&value);
}

void swap(any& lhs, any& rhs) { lhs.swap(rhs); }

}  // namespace learn
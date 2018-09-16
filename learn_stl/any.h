#pragma once

#include <functional>
#include <type_traits>
#include <typeinfo>

#include "memory.h"
#include "utility.h"

namespace learn {
class any {
  public:
    any() = default;

    template <typename Object>
    any(const Object& object) : container_(Container<std::decay_t<Object>>::make(object)) {}

    template <typename Object>
    any(Object&& object)
        : container_(Container<std::decay_t<Object>>::make(std::forward<Object>(object))) {}

    any(const any& other) : container_(other.container_->copy()) {}
    any(any&& other) : container_(learn::move(other.container_)) { other.reset(); }

    any& operator=(const any& other) {
        container_ = other.container_->copy();
        return *this;
    }

    any& operator=(any&& other) {
        container_ = ::learn::move(other.container_);
        other.reset();
        return *this;
    }

    template <typename Object, typename... Args>
    auto& emplace(Args&&... args) {
        container_ = Container<Object>::make(std::forward<Args>(args)...);
        return static_cast<Container<Object>&>(*container_).data;
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
    friend const Object* any_cast(const any* value);

    template <typename Object>
    friend Object* any_cast(any* value);

  private:
    template <typename T>
    struct ErasedDeleter : std::function<void(T*)> {
        ErasedDeleter() : std::function<void(T*)>{[](T* p) { delete p; }} {}
    };

    template <typename T>
    using ErasedPtr = unique_ptr<T, ErasedDeleter<T>>;

    struct ContainerInterface {
        using Ptr = ErasedPtr<ContainerInterface>;

        virtual ~ContainerInterface() = default;
        virtual const std::type_info& type() const = 0;

        virtual Ptr copy() const = 0;
    };

    template <typename ObjectT>
    struct Container : ContainerInterface {
        using Object = std::decay_t<ObjectT>;

        template <typename... Args>
        static Ptr make(Args&&... args) {
            return Ptr(new Container(std::forward<Args>(args)...));
        }

        explicit Container(const Object& _data) : data(_data) {}
        explicit Container(Object&& _data) : data(std::forward<Object>(_data)) {}
        Object data;

        const std::type_info& type() const final { return typeid(data); }

        Ptr copy() const final { return make(data); }
    };

    ContainerInterface::Ptr container_;
};

template <typename Object>
const Object* any_cast(const any* value) {
    auto container_ptr = dynamic_cast<const any::Container<Object>*>(value->container_.get());

    return container_ptr ? addressof(container_ptr->data) : nullptr;
}

template <typename Object>
Object* any_cast(any* value) {
    auto container_ptr = dynamic_cast<any::Container<Object>*>(value->container_.get());
    return container_ptr ? addressof(container_ptr->data) : nullptr;
}

template <typename Object>
Object any_cast(const any& value) {
    const auto value_ptr = any_cast<Object>(&value);

    if (!value_ptr) {
        throw std::bad_cast();
    }

    return *value_ptr;
}

template <typename Object>
Object any_cast(any& value) {
    const auto value_ptr = any_cast<Object>(&value);

    if (!value_ptr) {
        throw std::bad_cast();
    }

    return *value_ptr;
}

template <typename Object>
Object any_cast(any&& value) {
    const auto value_ptr = any_cast<Object>(&value);

    if (!value_ptr) {
        throw std::bad_cast();
    }

    return move(*value_ptr);
}

void swap(any& lhs, any& rhs) { lhs.swap(rhs); }

}  // namespace learn
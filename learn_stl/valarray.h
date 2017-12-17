#include <functional>
#include <vector>

namespace learn {
namespace detail {
template <typename ObjectT>
class expression {
  public:
    using object = typename std::decay<ObjectT>::type;
    using value_type = double;
    using size_type = std::size_t;

    value_type operator[](const size_type index) const {
        return static_cast<const object&>(*this)[index];
    }
    size_type size() const { return static_cast<const object&>(*this).size(); }

    operator object&() { return static_cast<object&>(*this); }
    operator const object&() { return static_cast<const object&>(*this); }
};

template <class Op, class ArgT>
class unary_op : public expression<unary_op<Op, ArgT>> {
  public:
    using Arg = typename std::decay<ArgT>::type;
    using size_type = std::size_t;
    using value_type = typename Arg::value_type;

    unary_op(const Arg& arg) : arg_(arg) {}

    value_type operator[](const size_type index) const { return Op()(arg_[index]); }
    size_type size() const { return arg_.size(); }

  private:
    const Arg& arg_;
};

template <class LhsT, class RhsT, class Op>
class binary_op : public expression<binary_op<LhsT, RhsT, Op>> {
  public:
    using Lhs = typename std::decay<LhsT>::type;
    using Rhs = typename std::decay<RhsT>::type;
    using size_type = std::size_t;
    using value_type = typename Lhs::value_type;

    static_assert(std::is_same<typename Lhs::value_type, typename Rhs::value_type>::value);

    binary_op(const LhsT& lhs, const RhsT& rhs) : lhs_(lhs), rhs_(rhs) {}

    value_type operator[](const size_type index) const { return Op()(lhs_[index], rhs_[index]); }
    size_type size() const { return lhs_.size(); }

  private:
    const Lhs& lhs_;
    const Rhs& rhs_;
};
}  // namespace detail

template <typename Lhs, typename Rhs>
auto operator+(const Lhs& lhs, const Rhs& rhs) {
    static_assert(std::is_same<typename Lhs::value_type, typename Rhs::value_type>::value,
                  "requires same types!");
    return detail::binary_op<Lhs, Rhs, std::plus<typename Lhs::value_type>>(lhs, rhs);
}

template <typename Lhs, typename Rhs>
auto operator-(const Lhs& lhs, const Rhs& rhs) {
    static_assert(std::is_same<typename Lhs::value_type, typename Rhs::value_type>::value,
                  "requires same types!");
    return detail::binary_op<Lhs, Rhs, std::minus<typename Lhs::value_type>>(lhs, rhs);
}

template <typename Lhs, typename Rhs>
auto operator*(const Lhs& lhs, const Rhs& rhs) {
    static_assert(std::is_same<typename Lhs::value_type, typename Rhs::value_type>::value,
                  "requires same types!");
    return detail::binary_op<Lhs, Rhs, std::multiplies<typename Lhs::value_type>>(lhs, rhs);
}

template <typename Lhs, typename Rhs>
auto operator/(const Lhs& lhs, const Rhs& rhs) {
    static_assert(std::is_same<typename Lhs::value_type, typename Rhs::value_type>::value,
                  "requires same types!");
    return detail::binary_op<Lhs, Rhs, std::divides<typename Lhs::value_type>>(lhs, rhs);
}

template <typename Lhs, typename Rhs>
auto operator%(const Lhs& lhs, const Rhs& rhs) {
    static_assert(std::is_same<typename Lhs::value_type, typename Rhs::value_type>::value,
                  "requires same types!");
    return detail::binary_op<Lhs, Rhs, std::modulus<typename Lhs::value_type>>(lhs, rhs);
}

template <typename ValueT>
class valarray : public detail::expression<valarray<ValueT>> {
  public:
    using value_type = double;
    using size_type = std::size_t;

    valarray() = default;
    explicit valarray(size_type count) : data_(count) {}
    valarray(const value_type& value, std::size_t count) : data_(count, value) {}

    template <typename Expr>
    valarray(const detail::expression<Expr>& expression) {
        data_.reserve(expression.size());

        for (size_type i = 0; i < expression.size(); ++i) {
            data_.emplace_back(expression[i]);
        }
    }

  private:
    using Storage = std::vector<value_type>;
    Storage data_;
};

}  // namespace learn
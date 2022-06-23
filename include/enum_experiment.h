#ifndef ENUM_EXPERIMENT_H
#define ENUM_EXPERIMENT_H

#include <functional>
#include <type_traits>
#include <variant>

enum class Index
{
    Ok,
    Err
};

template <class T, class E>
requires (!std::is_same_v<T, E>)
struct Result : public std::variant<T, E>
{
    using base = std::variant<T, E>;
    using OkType = T;
    using ErrType = E;

    Result() = default;

    explicit Result(T&& value) : base(std::forward<T>(value)) {}

    explicit Result(E&& value) : base(std::forward<E>(value)) {}
};

template <template <class, class> class R, class T, class E>
concept ResultVariant = requires(R<T, E> r)
{
    std::is_same_v<typename R<T, E>::OkType, T>;
    std::is_same_v<typename R<T, E>::ErrType, E>;
    R<T, E>(T{});
    R<T, E>(E{});
    r.index();
};

template <class F, class T, class E, template <class, class> class R>
concept Func = requires(F f)
{
    f(R<T, E>{});
};

template <template <class, class> class R, class T, class E, class F_ok, class F_err>
requires ResultVariant<R, T, E> && Func<F_ok, T, E, R> && Func<F_err, T, E, R>
T match(R<T, E> const &expression, F_ok if_ok, F_err if_err)
{
    if (expression.index() == static_cast<unsigned>(Index::Ok))
    {
        return if_ok(expression);
    }
    return if_err(expression);
}

template <template <class, class> class R, class T, class E, class F>
requires ResultVariant<R, T, E> && Func<F, T, E, R>
void if_let(R<T, E> const &expression, Index index, F f)
{
    if (expression.index() == static_cast<unsigned>(index))
    {
        f(expression);
    }
}

#endif
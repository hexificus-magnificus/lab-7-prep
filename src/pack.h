#ifndef PACK_H
#define PACK_H

#include <type_traits>

namespace pack_detail {

template <typename... Args>
constexpr int packSize = 0;

template <typename Arg, typename... Args>
constexpr int packSize<Arg, Args...> = 1 + packSize<Args...>;

template <int QueryIndex, typename... Args>
struct _packIndexToType {
    static_assert(false);
};

template <int QueryIndex, typename Arg, typename... Args>
struct _packIndexToType<QueryIndex, Arg, Args...> {
    using type = typename _packIndexToType<QueryIndex - 1, Args...>::type;
};

template <typename Arg, typename... Args>
struct _packIndexToType<0, Arg, Args...> {
    using type = Arg;
};

template <int QueryIndex, typename... Args>
using packIndexToType = typename _packIndexToType<QueryIndex, Args...>::type;

template <typename T, typename U>
constexpr bool isSame = false;

template <typename T>
constexpr bool isSame<T, T> = true;

template <int Accumulated, typename QueryType, typename... Args>
constexpr int _packTypeToIndex = -1;

template <int Accumulated, typename QueryType, typename Arg, typename... Args>
constexpr int _packTypeToIndex<Accumulated, QueryType, Arg, Args...> = isSame<QueryType, Arg> ? Accumulated : _packTypeToIndex<Accumulated + 1, QueryType, Args...>;

template <typename QueryType, typename... Args>
constexpr int packTypeToIndex = _packTypeToIndex<0, QueryType, Args...>;
}

template <typename... Args>
struct Pack {
    constexpr static int size = pack_detail::packSize<Args...>;

    template <int QueryIndex>
    using type = pack_detail::packIndexToType<QueryIndex, Args...>;

    template <typename QueryType>
    constexpr static int index = pack_detail::packTypeToIndex<QueryType, Args...>;
};

#endif  // PACK_H

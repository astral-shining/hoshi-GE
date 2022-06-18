#pragma once 

#include <iostream>
#include <type_traits>
#include <tuple>

#include "FixedString.hpp"

template<std::size_t s>
struct MaxEntities {
    static constexpr std::size_t value { s };
};

#define AND ,

#define IS_FROM_TEMPLATE(NAME, L1, L2) \
template<template<L1> typename, typename> \
struct is_from_##NAME##_template : std::false_type {}; \
template<template<L1> typename Tm, L1> \
struct is_from_##NAME##_template<Tm, Tm<L2>> : std::true_type {}; \
template<template<L1> typename Tm, typename T> \
inline constexpr bool is_from_##NAME##_template_v = is_from_##NAME##_template<Tm, T>::value;

IS_FROM_TEMPLATE(ts, typename... Ts, Ts...);
IS_FROM_TEMPLATE(auto, auto... Vs, Vs...);
IS_FROM_TEMPLATE(fsts, FixedString Fs AND typename... Ts, Fs AND Ts...);
IS_FROM_TEMPLATE(fsvs, FixedString Fs AND auto... Vs, Fs AND Vs...);


// Convert tuple types to 
template<template<typename...> typename C, typename... T>
struct tuple_to {};

template<template<typename...> typename C, typename... T>
struct tuple_to<C, std::tuple<T...>> {
    using type = C<T...>;
};

template<template<typename...> typename C, typename T>
using tuple_to_t = typename tuple_to<C, T>::type;


template<typename Tdel, typename T1, typename T2 = std::tuple<>>
struct remove_T_tuple : std::type_identity<void> {};

template<typename Tdel, typename Result>
struct remove_T_tuple<Tdel, std::tuple<>, Result> : std::type_identity<Result> {};

template<typename Tdel, typename T, typename... Ts, typename... Ts2>
struct remove_T_tuple<Tdel, std::tuple<T, Ts...>, std::tuple<Ts2...>> : remove_T_tuple<Tdel, std::tuple<Ts...>, std::tuple<Ts2..., T>> {};

template<typename Tdel, typename... Ts, typename... Ts2>
struct remove_T_tuple<Tdel, std::tuple<Tdel, Ts...>, std::tuple<Ts2...>> : remove_T_tuple<Tdel, std::tuple<Ts...>, std::tuple<Ts2...>> {};

template<typename DelT, typename Tupl>
using remove_T_tuple_t = typename remove_T_tuple<DelT, Tupl>::type;

#define TO() template operator()

#define constexpr_for(ini, cmp, inc, expr) [&] () constexpr {   \
    const auto f = [&] <ini> (const auto& fref) constexpr {     \
        if constexpr (cmp) {                                    \
            expr;                                               \
            return fref.template operator()<inc>(fref);         \
        }                                                       \
    };                                                          \
    return f.template operator()(f);                            \
}()

#define constexpr_for_noref(ini, cmp, inc, expr) [] () constexpr {  \
    const auto f = [] <ini> (const auto& fref) constexpr {          \
        if constexpr (cmp) {                                        \
            expr;                                                   \
            return fref.template operator()<inc>(fref);             \
        }                                                           \
    };                                                              \
    return f.template operator()(f);                                \
}()



/*template<typename T, typename... Ts>
constexpr auto remove_Tvalue_tuple(const std::tuple<Ts...>& t) {
    remove_T_tuple_t<T, std::tuple<Ts...>> result;

    constexpr_for(std::size_t c1 = 0 AND std::size_t c2 = 0, c1 < sizeof...(Ts), c1+1 AND c2, 
        if constexpr (!std::is_same_v<std::tuple_element_t<c1, std::tuple<Ts...>>, T>) {
            std::cout << "tuple: " << typeid(decltype(std::tuple_element_t<c1, std::tuple<Ts...>>())).name() << " newtuple: " << typeid(decltype(std::tuple_element_t<c2, decltype(result)>())).name() << std::endl;
            //std::tuple_element_t<c2 AND decltype(result)> v = std::get<c2>(result);

            //v = std::get<c1>(t);
            //std::get<c2>(result) = std::get<c1>(t);
            FOR_NEXT(c1+1 AND c2+1);
        } 
    );

    return result;
}
*/


#define FOR_NEXT(a) fref.template operator()<a>(fref); return;

template<typename, typename>
struct exists_T_tuple : std::false_type {};

template<typename T1, typename... Ts>
struct exists_T_tuple<T1, std::tuple<T1, Ts...>> : std::true_type {};

template<typename T1, typename T2, typename... Ts>
struct exists_T_tuple<T1, std::tuple<T2, Ts...>> : exists_T_tuple<T1, std::tuple<Ts...>> {};

template<typename T, typename Tuple>
inline constexpr bool exists_T_tuple_v = exists_T_tuple<T, Tuple>::value;

// Join tuple 
template<typename T, typename...>
struct join_tuple : std::type_identity<T> {};

template<typename... Ts1, typename... Ts2, typename... Ts3>
struct join_tuple<std::tuple<Ts1...>, std::tuple<Ts2...>, Ts3...> : join_tuple<std::tuple<Ts1..., Ts2...>, Ts3...> {};

template<typename... Ts>
using join_tuple_t = typename join_tuple<Ts...>::type;

// Lessthan
template<auto v1, auto v2>
inline constexpr bool less_than = v1 < v2;

// Slice tuple
template<typename, std::size_t, std::size_t, typename>
struct slice_tuple {};

template<typename T, std::size_t start, std::size_t end, typename... Ts>
struct slice_tuple<T, start, end, std::tuple<Ts...>> : 
    std::conditional_t<less_than<start, end>, slice_tuple<T, start+1, end, std::tuple<Ts..., std::tuple_element_t<start, T>>>,
    std::type_identity<std::tuple<Ts..., std::tuple_element_t<start, T>>>> {};

template<typename T, std::size_t start, std::size_t end = std::tuple_size_v<T>>
using slice_tuple_t = typename slice_tuple<T, start, end-1, std::tuple<>>::type;

// Reverse tuple
template<typename, typename R = std::tuple<>>
struct reverse_tuple : std::type_identity<R> {};

template<typename T, typename... Ts1, typename... Ts2>
struct reverse_tuple<std::tuple<T, Ts1...>, std::tuple<Ts2...>> : reverse_tuple<std::tuple<Ts1...>, std::tuple<T, Ts2...>> {
};

template<typename T>
using reverse_tuple_t = typename reverse_tuple<T>::type;

// Conditional constexpr
#define conditional_constexpr(cond, type1, type2) decltype([] () consteval { \
    if constexpr (cond) { \
        return std::type_identity<type1>{}; \
    } else { \
        return std::type_identity<type2>{}; \
    } \
}())::type

// No repeated tuple
template<typename T, std::size_t i = 0>
struct norepeated_tuple : 
    conditional_constexpr(less_than<i AND std::tuple_size_v<T>-1> && std::tuple_size_v<T>, norepeated_tuple<join_tuple_t<
        slice_tuple_t<T AND 0 AND i+1> 
        AND 
        remove_T_tuple_t<std::tuple_element_t<i AND T> AND slice_tuple_t<T AND i+1>
    >> AND i+1
>, std::type_identity<T>) {};

template<typename T>
using norepeated_tuple_t = typename norepeated_tuple<T>::type;

// Filter type from tuple 
template<typename, auto, typename T>
struct filter_tuple : std::type_identity<T> {};

template<auto f, typename T, typename... Ts, typename... TsF>
struct filter_tuple<std::tuple<T, Ts...>, f, std::tuple<TsF...>> :
    std::conditional_t<
        f.TO()<T>(),
            filter_tuple<std::tuple<Ts...>, f, std::tuple<TsF..., T>>,
            filter_tuple<std::tuple<Ts...>, f, std::tuple<TsF...>>> 
        {};

template<typename T, auto f>
using filter_tuple_t = typename filter_tuple<T, f, std::tuple<>>::type;

template<typename, auto> 
struct filter1_tuple : std::type_identity<void> {};

template<auto f, typename T, typename... Ts>
struct filter1_tuple<std::tuple<T, Ts...>, f> :
    std::conditional_t<
        f.TO()<T>(),
            std::type_identity<T>,
            filter1_tuple<std::tuple<Ts...>, f>> 
        {};

template<typename T, auto f>
using filter1_tuple_t = typename filter1_tuple<T, f>::type;

#define FILTER_TUPLE(NAME, EXPR)                                                    \
template<typename, typename T>                                                      \
struct filter_tuple_##NAME : std::type_identity<T> {};                              \
template<typename T, typename... Ts, typename... TsF>                               \
struct filter_tuple_##NAME<std::tuple<T, Ts...>, std::tuple<TsF...>> :              \
    std::conditional_t<                                                             \
        EXPR,                                                                       \
            filter_tuple_##NAME<std::tuple<Ts...>, std::tuple<TsF..., T>>,          \
            filter_tuple_##NAME<std::tuple<Ts...>, std::tuple<TsF...>>>             \
        {};                                                                         \
template<typename T>                                                                \
using filter_tuple_##NAME##_t = typename filter_tuple_##NAME<T, std::tuple<>>::type;

constexpr auto find_tuple(auto&& t, const auto& filter_fn, const auto& fn) {
    constexpr std::size_t tupl_size = std::tuple_size_v<std::remove_reference_t<decltype(t)>>;
    auto f = [&] <auto i=0, std::size_t... s> (const auto& fref) constexpr {
        if constexpr (i < tupl_size) {
            using iType = std::remove_reference_t<decltype(std::get<i>(t))>;
            if constexpr (filter_fn.TO()<iType>()) {
                return fref.TO()<i+1, s..., i>(fref);
            } else {
                return fref.TO()<i+1, s...>(fref);
            }
        } else {
            return fn.TO()<s...>();
        }
    };

    return f(f);
}

constexpr auto find1_tuple(auto&& t, const auto& filter_fn, const auto& fn) {
    constexpr std::size_t tupl_size = std::tuple_size_v<std::remove_reference_t<decltype(t)>>;
    auto f = [&] <auto i=0> (const auto& fref) constexpr {
        if constexpr (i < tupl_size) {
            using iType = std::remove_reference_t<decltype(std::get<i>(t))>;
            if constexpr (filter_fn.TO()<iType>()) {
                return fn.TO()<i>();
            } else {
                return fref.TO()<i+1>(fref);
            }
        }
    };

    return f(f);
}


// Overload
template<typename... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

// Valid expr
#define validexpr(EXPR) overloaded { \
        [] <typename = void> requires requires() { \
            EXPR; \
        } () consteval { \
            return true; \
        }, \
        [] <typename = void> () consteval { \
            return false; \
        } \
    }() 

#define validexprT(EXPR) overloaded { \
        [] <typename T_> requires requires(T_ t) { \
            EXPR; \
        } () consteval { \
            return true; \
        }, \
        [] <typename T_> () consteval { \
            return false; \
        } \
    }.TO

#define has_function(TYPE, FNAME) validexpr(std::is_member_function_pointer_v<decltype(&TYPE::FNAME)>)

#define evalif_validexpr(EXPR, ...) if constexpr (validexpr(EXPR)) { \
    EXPR; \
} else { \
   __VA_ARGS__; \
}

#define SEQ(N, EXPR) [&] <std::size_t... i> (std::index_sequence<i...>) { EXPR; }.template operator()(std::make_index_sequence<N>());

// Get function parameter types
template<typename>
struct function_args_types : std::type_identity<void>{};

template<typename F, typename... args>
struct function_args_types<F(args...)> : std::type_identity<std::tuple<args...>> {};

//template<typename F>
//using function_args_types_t = function_args_types<F>::type;

// If has args
template<typename T, typename... args>
consteval bool has_args_T(auto (T::*) (args...)) {
    return sizeof...(args) > 0;
}

template<typename... args>
consteval bool has_args(auto (*) (args...)) {
    return sizeof...(args) > 0;
}

/*template<typename... Ts>
constexpr auto pretty_tuple(const std::tuple<Ts...>& t) {
    std::cout << "std::tuple<";
    constexpr_for(std::size_t i = 0, i < sizeof...(Ts), i+1, 
        std::string str;
        using Tv = decltype(std::get<i>(t));
        if constexpr (validexpr({ std::declval<Tv>().to_string() } -> std::same_as<std::string>)) {
            str = std::get<i>(t).to_string();    
        } else if constexpr (validexpr(std::to_string(std::get<i>(t)))) {
            str = std::to_string(std::get<i>(t));
        } else {
            str = "?";
        }
        std::cout << typeid(std::tuple_element_t<i, std::tuple<Ts...>>).name() 
        << "(" << str << ")";
        if constexpr (i < sizeof...(Ts)-1) {
            std::cout << ", ";
        }
    );
    std::cout << ">\n";
}*/

template<std::size_t e = 1, typename... S> 
requires std::conjunction_v<std::is_convertible<S, std::string_view>...>
void terminate(const S... str) {
    if constexpr (sizeof...(S) > 0)
        (std::operator<<((1 ? std::cerr : std::cout) << str, " "), ...) << "\n";
    
    std::exit(e);
}

void pretty(auto&&) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

#define START_BENCH(NAME) { const char* name {NAME}; auto start = std::chrono::steady_clock::now();
#define END_BENCH \
    auto end = std::chrono::steady_clock::now(); \
    std::cout << name << ": " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n"; \
    }

void printMem(auto t) {
    using T = decltype(t);
    constexpr std::size_t s = sizeof(T);
    T* main_ptr = &t;

    const uint8_t* uptr = reinterpret_cast<const uint8_t*>(main_ptr);
    for (const uint8_t* i=uptr; i<uptr+s; i++) {
        std::printf("%02X ", *i);
    }
    std::printf("\n");
}

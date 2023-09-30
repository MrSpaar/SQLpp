//
// Created by mrspaar on 7/15/23.
//

#ifndef SQLPP_TYPES_H
#define SQLPP_TYPES_H

#include <string>


namespace sqlpp {
    [[maybe_unused]] typedef int INTEGER;
    [[maybe_unused]] typedef double REAL;
    [[maybe_unused]] typedef const char* TEXT;
    [[maybe_unused]] typedef std::string BLOB;

    namespace types {
        struct SQLTable;
        template<typename T> struct SQLCol;
    }

    namespace traits {
        template<typename V, typename T>
        struct is_compatible_with_col: std::false_type {};
        template<typename V, typename T>
        struct is_compatible_with_col<types::SQLCol<V>, types::SQLCol<T>>: std::is_convertible<V, T> {};
        template<typename V, typename T>
        struct is_compatible_with_col<V, types::SQLCol<T>>: std::conditional_t<std::is_same_v<V, T>,
                                                                               std::true_type,
                                                                               std::is_convertible<V, T>> {};

        template<typename V, typename  T>
        inline constexpr bool is_compatible_v = is_compatible_with_col<V, types::SQLCol<T>>::value;
    }
}


#endif //SQLPP_TYPES_H

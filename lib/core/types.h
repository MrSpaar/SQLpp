//
// Created by mrspaar on 7/15/23.
//

#ifndef SQLPP_TYPES_H
#define SQLPP_TYPES_H

#include <string>


namespace sqlpp {
    typedef int INTEGER;
    typedef double REAL;
    typedef const char* TEXT;
    typedef std::string BLOB;

    namespace types {
        struct SQLTable;
        template<typename T> struct SQLCol;
    }

    namespace traits {
        inline constexpr bool always_false_v = false;

        template<typename V, typename T>
        struct is_matching_col: std::false_type {};
        template<typename V, typename T>
        struct is_matching_col<V, types::SQLCol<T>>: std::is_same<V, T> {};
        template<typename V>
        struct is_matching_col<V, types::SQLCol<TEXT>>: std::is_convertible<V, TEXT> {};

        template<typename V, typename T>
        struct is_equivalent_cols: std::false_type {};
        template<typename V, typename T>
        struct is_equivalent_cols<types::SQLCol<V>, types::SQLCol<T>>: std::is_convertible<V, T> {};

        template<typename V, typename T>
        inline constexpr bool is_matching_col_v = is_matching_col<V, T>::value;
        template<typename V, typename T>
        inline constexpr bool is_equivalent_cols_v = is_equivalent_cols<V, T>::value;
        template<typename V, typename  T>
        inline constexpr bool is_compatible_v =
                is_matching_col_v<V, types::SQLCol<T>> || is_equivalent_cols_v<V, types::SQLCol<T>>;
    }
}


#endif //SQLPP_TYPES_H

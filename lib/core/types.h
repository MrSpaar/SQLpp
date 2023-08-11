//
// Created by mrspaar on 7/15/23.
//

#ifndef SQLPP_TYPES_H
#define SQLPP_TYPES_H

#include <string>
#include <vector>
#include <variant>


namespace sqlpp {
    typedef int INTEGER;
    typedef double REAL;
    typedef std::string TEXT;
    typedef std::vector<unsigned char> BLOB;

    enum SQLFlag {
        PRIMARY_KEY   = (1 << 0),
        AUTOINCREMENT = (1 << 1),
        NOT_NULL      = (1 << 2),
        UNIQUE        = (1 << 3),
    };

    namespace types {
        struct Runnable;
        struct SQLForeignKey;
        template<typename T> struct SQLCol;

        typedef std::variant<
                SQLCol<INTEGER>*, SQLCol<REAL>*, SQLCol<TEXT>*, SQLCol<BLOB>*
        > SQLColTypePtr;

        typedef std::vector<SQLForeignKey*> SQLForeignKeyList;
        typedef std::vector<SQLColTypePtr> SQLColList;
    }

    namespace traits {
        template<typename>
        struct always_false: std::false_type {};

        template<typename T>
        struct is_sql_col: std::false_type {};
        template<typename T>
        struct is_sql_col<types::SQLCol<T>>: std::true_type {};

        template<typename T>
        struct is_numeric_col: std::is_convertible<T, INTEGER> {};

        template<typename T, typename V>
        struct is_matching_col_type: std::false_type {};
        template<typename T, typename V>
        struct is_matching_col_type<types::SQLCol<T>&, V>: std::is_same<T, V> {};
        template<typename V>
        struct is_matching_col_type<types::SQLCol<TEXT>&, V>: std::is_convertible<V, TEXT> {};
    }
}


#endif //SQLPP_TYPES_H

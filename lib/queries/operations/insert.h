//
// Created by mrspaar on 7/27/23.
//

#ifndef SQLPP_INSERT_H
#define SQLPP_INSERT_H

#include "queries/base.h"

namespace sqlpp::keywords::Insert {
    template<typename... ItemTypes>
    struct Values: types::Runnable {
        using Runnable::operator,;

        Values& prepend(const std::string &str) {
            sql.insert(0, str);
            return *this;
        }

        template<typename T>
        void addItem(const T& item) {
            if constexpr (std::is_convertible_v<T, TEXT>)
                sql.append("'").append(item).append("', ");
            else
                sql.append(std::to_string(item)).append(", ");
        }

        explicit Values(ItemTypes... values): Runnable(" VALUES (") {
            (addItem(values), ...);
            sql.pop_back(); sql.pop_back();
            sql.append(")");
        }
    };

    struct Default: types::Runnable {
        using Runnable::operator,;
        explicit Default(const std::string &sql): Runnable(sql, " DEFAULT VALUES") {}
    };

    template<typename... ColTypes>
    struct ValuesIntermediate: types::Keyword {
        using Keyword::operator,;
        explicit ValuesIntermediate(std::string sql): Keyword(std::move(sql)) {}

        template<typename... ValTypes>
        Values<ValTypes...>& operator,(Values<ValTypes...>&& values) {
            if constexpr ((... && traits::is_matching_col_type<ColTypes, ValTypes>::value))
                return values.prepend(sql);
            else
                static_assert(traits::always_false<Values<ValTypes...>>::value, "Column and value types must match");
        }

        Default operator,(keywords::DefaultValues) { return Default(sql); };
    };

    struct Into: types::Keyword {
        using Keyword::operator,;
        explicit Into(const std::string &sql): Keyword(sql, "INTO ") {}

        template<typename... ColTypes>
        ValuesIntermediate<ColTypes...> operator,(const expr::CommaExpr<ColTypes...> &expr) {
            sql.append(expr.sql);
            return ValuesIntermediate<ColTypes...>(sql);
        };
    };

    struct Insert: types::Keyword {
        using Keyword::operator,;

        explicit Insert(): Keyword("INSERT ") {}
        explicit Insert(const std::string &sql): Keyword(sql) {}

        Into operator,(keywords::Into) { return Into(sql); };
    };

    struct InsertOr: types::Keyword {
        using Keyword::operator,;
        explicit InsertOr(): Keyword("INSERT ") {}

        template<typename T>
        Insert operator,(T) {
            if constexpr (std::is_same_v<T, keywords::Abort>)
                sql.append("OR ABORT ");
            else if constexpr (std::is_same_v<T, keywords::Fail>)
                sql.append("OR FAIL ");
            else if constexpr (std::is_same_v<T, keywords::Ignore>)
                sql.append("OR IGNORE ");
            else if constexpr (std::is_same_v<T, keywords::Replace>)
                sql.append("OR REPLACE ");
            else if constexpr (std::is_same_v<T, keywords::Rollback>)
                sql.append("OR ROLLBACK ");
            else
                static_assert(traits::always_false<T>::value, "Unsuported keyword");

            return Insert(sql);
        };
    };
}

#endif //SQLPP_INSERT_H

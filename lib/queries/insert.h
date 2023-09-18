//
// Created by mrspaar on 7/27/23.
//

#ifndef SQLPP_INSERT_H
#define SQLPP_INSERT_H

#include "queries/base.h"


namespace sqlpp::keywords::insert {
    struct Default: Keyword {
        Default& morph() {
            *source << " DEFAULT VALUES";
            return *this;
        }
    };

    template<typename...>
    struct Values: Keyword {
        template<typename Item, typename... Items>
        Values& morph(const Item &value, const Items&... values) {
            *source << "VALUES (";
            append(value, ""); (append(values), ...);
            *source << ")";

            return *this;
        }

        void append(const char *str, const char *sep = ", ") {
            *source << sep << '\'' << str << '\'';
        }
        void append(const std::string &str, const char *sep = ", ") {
            *source << sep << "X'" << str << '\'';
        }
        template<typename T>
        void append(const types::SQLCol<T> &col, const char *sep = ", ") {
            *source << sep << col.name;
        }
        template<typename T>
        void append(const T& value, const char *sep = ", ") {
            *source << sep << value;
        }
    };

    template<typename... ColTypes>
    struct Into: Keyword {
        Into<ColTypes...>& morph(const expr::TableExpr<ColTypes...> &expr) {
            *source << "INTO " << expr.sql.str();
            return *this;
        }

        template<typename... ValTypes>
        Values<ValTypes...>& values(const ValTypes&... values) {
            if constexpr ((!traits::is_matching_col_v<ValTypes, ColTypes> || ...))
                static_assert(traits::always_false_v, "Column type mismatch");

            return ((Values<ValTypes...>*) this)->morph(std::forward<const ValTypes&>(values)...);
        }

        Default& default_() {
            return ((Default*) this)->morph();
        }
    };

    struct Or: Keyword {
        Or &morph(const char *token) {
            *source << "OR " << token;
            return *this;
        }

        Or& next() {
            return *this;
        }

        template<typename... Columns>
        Into<Columns...>& into(const expr::TableExpr<Columns...> &expr) {
            return ((Into<Columns...>*) this)->morph(expr);
        }
    };

    struct Insert: Query {
        explicit Insert(): Query() {
            sql << "INSERT ";
        }

        Or& or_(const char *token) {
            return ((Or*) this)->morph(token);
        }

        template<typename... Columns>
        Into<Columns...>& into(const expr::TableExpr<Columns...> &expr) {
            return ((Into<Columns...>*) this)->morph(expr);
        }
    };
}

#endif //SQLPP_INSERT_H

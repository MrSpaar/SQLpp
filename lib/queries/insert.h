//
// Created by mrspaar on 7/27/23.
//

#ifndef SQLPP_INSERT_H
#define SQLPP_INSERT_H

#include "queries/base.h"


namespace sqlpp::keywords::insert {
    struct Default: Runnable {
        Default& morph() {
            append(" DEFAULT VALUES");
            return *this;
        }
    };

    template<typename...>
    struct Values: Runnable {
        template<typename Item, typename... Items>
        Values& morph(const Item &value, const Items&... values) {
            append(" VALUES (");
            add(value); ((append(", "), add(values)), ...);
            append(")");
            return *this;
        }

        template<typename T>
        [[maybe_unused]] void add(const types::SQLCol<T> &col) { append(col); }
        template<typename T>
        [[maybe_unused]] void add(const T& value) { append(std::to_string(value)); }
        [[maybe_unused]] void add(const char *str) { append("'").append(str).append("'"); }
        [[maybe_unused]] void add(const std::string &str) { append("X'").append(str).append("'"); }
    };

    template<typename... ColTypes>
    struct Into: Keyword {
        Into<ColTypes...>& morph(const expr::TableExpr<ColTypes...> &expr) {
            append("INTO ").append(expr);
            return *this;
        }

        template<typename... ValTypes>
        Values<ValTypes...>& values(const ValTypes&... values) {
            static_assert(
                    (traits::is_compatible_v<ValTypes, ColTypes> && ...),
                    "Column type mismatch"
            );

            return ((Values<ValTypes...>*) this)->morph(values...);
        }

        Default& default_() {
            return ((Default*) this)->morph();
        }
    };

    struct Or: Keyword {
        Or &morph(const char *token) {
            append("OR ").append(token);
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

    struct Insert: Keyword {
        Insert(): Keyword("INSERT ") {}

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

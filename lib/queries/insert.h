//
// Created by mrspaar on 7/27/23.
//

#ifndef SQLPP_INSERT_H
#define SQLPP_INSERT_H

#include "queries/base.h"


namespace sqlpp::keywords::insert {
    struct Default: Runnable {
        Default& morph() {
            source->append(" DEFAULT VALUES");
            return *this;
        }
    };

    template<typename...>
    struct Values: Runnable {
        template<typename Item, typename... Items>
        Values& morph(const Item &value, const Items&... values) {
            source->append(" VALUES (");
            append(value, ""); (append(values), ...);
            source->append(")");
            return *this;
        }

        void append(const char *str, const char *sep = ", ") {
            source->append(sep).append("'").append(str).append("'");
        }
        void append(const std::string &str, const char *sep = ", ") {
            source->append(sep).append("X'").append(str).append("'");
        }
        template<typename T>
        void append(const types::SQLCol<T> &col, const char *sep = ", ") {
            source->append(sep).append(col.name);
        }
        template<typename T>
        void append(const T& value, const char *sep = ", ") {
            source->append(sep).append(std::to_string(value));
        }
    };

    template<typename... ColTypes>
    struct Into: Keyword {
        Into<ColTypes...>& morph(const expr::TableExpr<ColTypes...> &expr) {
            source->append("INTO ").append(expr);
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
            source->append("OR ").append(token);
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
        explicit Insert(): Query() { sql.append("INSERT "); }

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

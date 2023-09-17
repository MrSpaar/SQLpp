//
// Created by mrspaar on 7/27/23.
//

#ifndef SQLPP_INSERT_H
#define SQLPP_INSERT_H

#include "queries/base.h"


namespace sqlpp::keywords::insert {
    struct Default: Keyword {
        inline Default& morph() {
            *source << " DEFAULT VALUES";
            return *this;
        }
    };

    template<typename...>
    struct Values: Keyword {
        template<typename Item, typename... Items>
        inline Values& morph(const Item &value, const Items&... values) {
            *source << "VALUES (";
            append(value, ""); (append(values), ...);
            *source << ")";

            return *this;
        }

        template<typename ValType>
        inline void append(const ValType& value, const char *sep = ", ") {
            *source << sep;

            if constexpr (std::is_same_v<ValType, BLOB>)
                *source << "X'" << value << '\'';
            else if constexpr (std::is_convertible_v<ValType, std::string>)
                *source << '\'' << value << '\'';
            else if constexpr (traits::is_sql_col_v<ValType>)
                *source << value.name;
            else
                *source << value;
        }
    };

    template<typename... ColTypes>
    struct Into: Keyword {
        inline Into<ColTypes...>& morph(const expr::TableExpr<ColTypes...> &expr) {
            *source << "INTO " << expr.sql.str();
            return *this;
        }

        template<typename... ValTypes>
        inline Values<ValTypes...>& values(const ValTypes&... values) {
            if constexpr ((!traits::is_matching_col_v<ValTypes, ColTypes> || ...))
                static_assert(traits::always_false_v, "Column type mismatch");

            return ((Values<ValTypes...>*) this)->morph(std::forward<const ValTypes&>(values)...);
        }

        inline Default& default_() {
            return ((Default*) this)->morph();
        }
    };

    struct Or: Keyword {
        inline Or &morph(const char *token) {
            *source << "OR " << token;
            return *this;
        }

        inline Or& next() {
            return *this;
        }

        template<typename... Columns>
        inline Into<Columns...>& into(const expr::TableExpr<Columns...> &expr) {
            return ((Into<Columns...>*) this)->morph(expr);
        }
    };

    struct Insert: Query {
        explicit Insert(): Query() {
            sql << "INSERT ";
        }

        inline Or& or_(const char *token) {
            return ((Or*) this)->morph(token);
        }

        template<typename... Columns>
        inline Into<Columns...>& into(const expr::TableExpr<Columns...> &expr) {
            return ((Into<Columns...>*) this)->morph(expr);
        }
    };
}

#endif //SQLPP_INSERT_H

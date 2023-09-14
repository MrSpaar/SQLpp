//
// Created by mrspaar on 7/27/23.
//

#ifndef SQLPP_INSERT_H
#define SQLPP_INSERT_H

#include "queries/base.h"

namespace sqlpp::keywords::insert {
    struct Default: Keyword {
        inline Default& morph() override {
            source->append(" DEFAULT VALUES");
            return *this;
        }

        inline Keyword& values() {
            return ((Keyword*) this)->morph();
        }
    };

    template<typename... ValTypes>
    struct Values: Keyword {
        inline Values& morph(const ValTypes&... values) {
            source->append(" VALUES (");
            append(values...);
            source->append(")");

            return *this;
        }

        template<typename ValType, typename... Vals>
        inline void append(const ValType& value, const Vals&... values) {
            if constexpr (std::is_convertible_v<ValType, std::string>)
                source->append("'").append(value).append("'");
            else if constexpr (traits::is_sql_col<ValType>::value)
                source->append(value.name);
            else
                source->append(std::to_string(value));

            source->append(", ");
            append(std::forward<const Vals&>(values)...);
        }

        inline void append() {
            source->pop_back();
            source->pop_back();
        }
    };

    template<typename... ColTypes>
    struct Into: Keyword {
        inline Into<ColTypes...>& morph(const expr::TableExpr<ColTypes...> &expr) {
            source->append("INTO ").append(expr.sql);
            return *this;
        }

        template<typename... ValTypes>
        inline Values<ValTypes...>& values(const ValTypes&... values) {
            if constexpr ((!traits::is_matching_col_type<ColTypes, ValTypes>::value || ...))
                static_assert(traits::always_false<ColTypes...>::value, "Column type mismatch");

            return ((Values<ValTypes...>*) this)->morph(std::forward<const ValTypes&>(values)...);
        }

        inline Default& default_() {
            return ((Default*) this)->morph();
        }
    };

    struct Or: Keyword {
        inline Or &morph(const std::string &token) {
            source->append("OR ").append(token);
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
        explicit Insert(): Query() { sql.append("INSERT "); }

        inline Or& or_(const std::string &token) {
            return ((Or*) this)->morph(token);
        }

        template<typename... ColTypes>
        inline Into<ColTypes...>& into(const expr::TableExpr<ColTypes...> &expr) {
            return ((Into<ColTypes...>*) this)->morph(expr);
        }
    };

}

#endif //SQLPP_INSERT_H

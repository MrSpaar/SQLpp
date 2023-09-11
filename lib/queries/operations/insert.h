//
// Created by mrspaar on 7/27/23.
//

#ifndef SQLPP_INSERT_H
#define SQLPP_INSERT_H

#include "queries/base.h"

namespace sqlpp::keywords::insert {
    struct DefaultValues: Keyword {
        inline DefaultValues& morph() {
            source->append(" DEFAULT VALUES");
            return *this;
        }
    };

    template<typename... ValTypes>
    struct Values: Keyword {
        static inline Values& morph(Keyword *kw, ValTypes... values) {
            kw->source->append(" VALUES (");
            append(kw, values...);
            kw->source->append(")");

            return (Values &) *kw;
        }

        template<typename ValType, typename... Vals>
        static inline void append(Keyword *kw, ValType value, Vals... values) {
            if constexpr (std::is_convertible_v<ValType, std::string>)
                kw->source->append("'").append(value).append("'");
            else if constexpr (traits::is_sql_col<ValType>::value)
                kw->source->append(value.name);
            else
                kw->source->append(std::to_string(value));

            kw->source->append(", ");
            append(kw, values...);
        }

        static inline void append(Keyword *kw) {
            kw->source->pop_back();
            kw->source->pop_back();
        }
    };

    template<typename... ColTypes>
    struct Into: Keyword {
        inline Into<ColTypes...>& morph(const expr::TableExpr<ColTypes...> &expr) {
            source->append("INTO ").append(expr.sql);
            return *this;
        }

        template<typename... ValTypes>
        inline Values<ValTypes...>& values(ValTypes... values) {
            if constexpr ((!traits::is_matching_col_type<ColTypes, ValTypes>::value || ...))
                static_assert(traits::always_false<ColTypes...>::value, "Column type mismatch");

            return ((Values<ValTypes...>*) this)->morph(this, values...);
        }

        inline DefaultValues& defaultValues() {
            return ((DefaultValues*) this)->morph();
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

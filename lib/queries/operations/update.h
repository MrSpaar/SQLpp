//
// Created by mrspaar on 7/25/23.
//

#ifndef SQLPP_UPDATE_H
#define SQLPP_UPDATE_H

#include "queries/base.h"


namespace sqlpp::keywords::update {
    struct Where: Keyword {
        inline Where& morph(const expr::CondExpr &expr) {
            source->append(" WHERE ").append(expr.sql);
            return *this;
        }

        inline Where& and_(const expr::CondExpr &expr) {
            source->append(" AND ").append(expr.sql);
            return *this;
        }

        inline Where& or_(const expr::CondExpr &expr) {
            source->append(" OR ").append(expr.sql);
            return *this;
        }
    };

    struct From: Keyword {
        inline From& morph(const types::SQLTable &table) {
            source->append(" FROM ").append(table.name);
            return *this;
        }

        inline Where& where(const expr::CondExpr &expr) {
            return ((Where*) this)->morph(expr);
        }
    };

    struct Set: Keyword {
        template<typename... Items>
        inline Set& morph(const Items&... items) {
            if constexpr ((!std::is_same_v<Items, expr::EqExpr> || ...))
                static_assert(
                        traits::always_false<Items...>::value,
                        "SET only accepts \"col = val\" expressions"
                );

            source->append(" SET ");
            ((source->append(items.sql).append(", ")), ...);
            source->pop_back(); source->pop_back();

            return *this;
        }

        inline From& from(const types::SQLTable &table) {
            return ((From*) this)->morph(table);
        }

        inline Where& where(const expr::CondExpr &expr) {
            return ((Where*) this)->morph(expr);
        }
    };

    struct Or: Keyword {
        inline Or& morph(const std::string &token) {
            source->append("OR ").append(token);
            return *this;
        }

        inline Or& next(const types::SQLTable &table) {
            source->append(table.name);
            return *this;
        }

        template<typename... Items>
        inline Set& set(const Items&... items) {
            return ((Set*) this)->morph(std::forward<const Items&>(items)...);
        }
    };

    struct Update: Query {
        Update(): Query() { source->append("UPDATE "); };
        explicit Update(const types::SQLTable &table): Query() { source->append("UPDATE ").append(table.name); }

        inline Or& or_(const std::string &token) {
            return ((Or*) this)->morph(token);
        }

        inline Set& set(const expr::EqExpr &expr) {
            return ((Set*) this)->morph(expr);
        }
    };
}


#endif //SQLPP_UPDATE_H

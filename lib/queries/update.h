//
// Created by mrspaar on 7/25/23.
//

#ifndef SQLPP_UPDATE_H
#define SQLPP_UPDATE_H

#include "queries/base.h"


namespace sqlpp::keywords::update {
    struct Where: Keyword {
        inline Where& morph(const expr::ConditionExpr &expr) {
            *source << " WHERE " << expr.sql.str();
            return *this;
        }

        inline Where& and_(const expr::ConditionExpr &expr) {
            *source << " AND " << expr.sql.str();
            return *this;
        }

        inline Where& or_(const expr::ConditionExpr &expr) {
            *source << " OR " << expr.sql.str();
            return *this;
        }
    };

    struct From: Keyword {
        inline From& morph(const types::SQLTable &table) {
            *source << " FROM " << table.name;
            return *this;
        }

        inline From& morph(const SubQuery &subQuery) {
            *source << " FROM (" << subQuery.source->str() << ")";
            return *this;
        }

        inline Where& where(const expr::ConditionExpr &expr) {
            return ((Where*) this)->morph(expr);
        }
    };

    struct Set: Keyword {
        template<typename Item, typename... Items>
        inline Set& morph(const Item &item, const Items&... items) {
            if constexpr ((!std::is_same_v<Items, expr::EqExpr> || ...) || !std::is_same_v<Item, expr::EqExpr>)
                static_assert(traits::always_false_v, "SET only accepts \"col = val\" expressions");

            *source << " SET " << item.sql.str();
            (..., (*source << ", " << items.sql));

            return *this;
        }

        inline From& from(const types::SQLTable &table) {
            return ((From*) this)->morph(table);
        }

        inline From& from(const SubQuery &subQuery) {
            return ((From*) this)->morph(subQuery);
        }

        inline Where& where(const expr::ConditionExpr &expr) {
            return ((Where*) this)->morph(expr);
        }
    };

    struct Or: Keyword {
        inline Or& morph(const std::string &token) {
            *source << "OR " << token;
            return *this;
        }

        inline Or& next(const types::SQLTable &table) {
            *source << table.name;
            return *this;
        }

        template<typename... Items>
        inline Set& set(const Items&... items) {
            return ((Set*) this)->morph(std::forward<const Items&>(items)...);
        }
    };

    struct Update: Query {
        Update(): Query() { sql << "UPDATE "; };
        explicit Update(const types::SQLTable &table): Query() { sql << "UPDATE " << table.name; }

        inline Or& or_(const std::string &token) {
            return ((Or*) this)->morph(token);
        }

        inline Set& set(const expr::EqExpr &expr) {
            return ((Set*) this)->morph(expr);
        }
    };
}


#endif //SQLPP_UPDATE_H
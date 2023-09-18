//
// Created by mrspaar on 7/25/23.
//

#ifndef SQLPP_UPDATE_H
#define SQLPP_UPDATE_H

#include "queries/base.h"


namespace sqlpp::keywords::update {
    struct Where: Keyword {
        Where& morph(const expr::ConditionExpr &expr) {
            source->append(" WHERE ").append(expr.sql);
            return *this;
        }

        Where& and_(const expr::ConditionExpr &expr) {
            source->append(" AND ").append(expr.sql);
            return *this;
        }

        Where& or_(const expr::ConditionExpr &expr) {
            source->append(" OR ").append(expr.sql);
            return *this;
        }
    };

    struct From: Keyword {
        From& morph(const types::SQLTable &table) {
            source->append(" FROM ").append(table.name);
            return *this;
        }

        From& morph(const SubQuery &subQuery) {
            source->append(" FROM (").append(*subQuery.source).append(")");
            return *this;
        }

        Where& where(const expr::ConditionExpr &expr) {
            return ((Where*) this)->morph(expr);
        }
    };

    struct Set: Keyword {
        template<typename Item, typename... Items>
        Set& morph(const Item &item, const Items&... items) {
            source->append(" SET ");
            append(item, "");
            (append(items), ...);
            return *this;
        }

        void append(const expr::EqExpr &expr, const char *sep = ", ") {
            source->append(sep).append(expr.sql);
        }

        From& from(const types::SQLTable &table) {
            return ((From*) this)->morph(table);
        }

        From& from(const SubQuery &subQuery) {
            return ((From*) this)->morph(subQuery);
        }

        Where& where(const expr::ConditionExpr &expr) {
            return ((Where*) this)->morph(expr);
        }
    };

    struct Or: Keyword {
        Or& morph(const std::string &token) {
            source->append("OR ").append(token);
            return *this;
        }

        Or& next(const types::SQLTable &table) {
            source->append(table.name);
            return *this;
        }

        template<typename... Items>
        Set& set(const Items&... items) {
            return ((Set*) this)->morph(std::forward<const Items&>(items)...);
        }
    };

    struct Update: Query {
        Update(): Query() { sql.append("UPDATE "); };
        explicit Update(const types::SQLTable &table): Query() { sql.append("UPDATE ").append(table.name); }

        Or& or_(const std::string &token) {
            return ((Or*) this)->morph(token);
        }

        template<typename... Exprs>
        Set& set(const Exprs&... exprs) {
            return ((Set*) this)->morph(exprs...);
        }
    };
}


#endif //SQLPP_UPDATE_H

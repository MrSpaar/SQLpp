//
// Created by mrspaar on 7/13/23.
//

#ifndef SQLPP_SELECT_H
#define SQLPP_SELECT_H

#include "queries/base.h"


namespace sqlpp::keywords::select {
    struct Offset: SubQuery {
        Offset& morph(int offset) {
            source->append(" OFFSET ").append(std::to_string(offset));
            return *this;
        }
    };

    struct Limit: SubQuery {
        Limit& morph(int limit) {
            source->append(" LIMIT ").append(std::to_string(limit));
            return *this;
        }

        Offset& morph(int limit, int offset) { return morph(limit).offset(offset); }
        Offset& offset(int offset) { return ((Offset*) this)->morph(offset); }
    };

    struct OrderBy: SubQuery {
        template<typename Item, typename... Items>
        OrderBy& morph(const Item &item, const Items&... items) {
            append(item, "");
            (append(items), ...);
            return *this;
        }

        template<typename T>
        void append(const types::SQLCol<T> &col, const char *sep = ", ") {
            source->append(sep).append(col.name);
        }
        void append(const expr::OrderExpr &expr, const char *sep = ", ") {
            source->append(sep).append(expr);
        }

        Limit& limit(int limit) { return ((Limit*) this)->morph(limit); }
        Offset& limit(int limit, int offset) { return ((Limit*) this)->morph(limit, offset); }
    };

    struct Order: Keyword {
        Order& morph() {
            return *this;
        }

        template<typename... Items>
        OrderBy& by(const Items&... items) {
            source->append(" ORDER BY ");
            return ((OrderBy*) this)->morph(std::forward<const Items&>(items)...);
        }
    };

    struct Having: SubQuery {
        Having& morph(const expr::ConditionExpr &expr) {
            source->append(" HAVING ").append(expr);
            return *this;
        }

        Order& order() { return ((Order*) this)->morph(); }
        Limit& limit(int limit) { return ((Limit*) this)->morph(limit); }
        Offset& limit(int limit, int offset) { return ((Limit*) this)->morph(limit, offset); }
    };

    struct GroupBy: SubQuery {
        template<typename Item, typename... Items>
        GroupBy& morph(const Item& item, const Items&... items) {
            append(item, "");
            (append(items), ...);
            return *this;
        }

        template<typename T>
        void append(const types::SQLCol<T> &col, const char *sep = ", ") {
            source->append(sep).append(col.name);
        }

        Having& having(const expr::ConditionExpr &expr) { return ((Having*) this)->morph(expr); }
        Order& order() { return ((Order*) this)->morph(); }
        Limit& limit(int limit) { return ((Limit*) this)->morph(limit); }
        Offset& limit(int limit, int offset) { return ((Limit*) this)->morph(limit, offset); }
    };

    struct Group: Keyword {
        Group& morph() {
            return *this;
        }

        template<typename... Items>
        GroupBy& by(const Items&... items) {
            source->append(" GROUP BY ");
            return ((GroupBy*) this)->morph(std::forward<const Items&>(items)...);
        }
    };

    struct Where: SubQuery {
        Where& morph(const expr::ConditionExpr &expr) {
            source->append(" WHERE ").append(expr);
            return *this;
        }

        Where& and_(const expr::ConditionExpr &expr) {
            source->append(" AND ").append(expr);
            return *this;
        }

        Where& or_(const expr::ConditionExpr &expr) {
            source->append(" OR ").append(expr);
            return *this;
        }

        Group& group() { return ((Group*) this)->morph(); }
        Having& having(const expr::ConditionExpr &expr) { return ((Having*) this)->morph(expr); }
        Order& order() { return ((Order*) this)->morph(); }
        Limit& limit(int limit) { return ((Limit*) this)->morph(limit); }
        Offset& limit(int limit, int offset) { return ((Limit*) this)->morph(limit, offset); }
    };

    struct On: SubQuery {
        On& morph(const expr::ConditionExpr &expr) {
            source->append(" ON ").append(expr);
            return *this;
        }

        On& and_(const expr::ConditionExpr &expr) {
            source->append(" AND ").append(expr);
            return *this;
        }

        On& or_(const expr::ConditionExpr &expr) {
            source->append(" OR ").append(expr);
            return *this;
        }

        Where& where(const expr::ConditionExpr &expr) { return ((Where*) this)->morph(expr); }
        Group& group() { return ((Group*) this)->morph(); }
        Order& order() { return ((Order*) this)->morph(); }
        Limit& limit(int limit) { return ((Limit*) this)->morph(limit); }
        Offset& limit(int limit, int offset) { return ((Limit*) this)->morph(limit, offset); }
    };

    struct Join: Keyword {
        Join& morph(const types::SQLTable &table) {
            source->append(" JOIN ").append(table);
            return *this;
        }

        On& on(const expr::ConditionExpr &expr) {
            return ((On*) this)->morph(expr);
        }
    };

    struct JoinOp: Keyword {
        JoinOp& morph(const char *joinOp) {
            source->append(joinOp);
            return *this;
        }

        Join& join(const types::SQLTable &table) {
            return ((Join*) this)->morph(table);
        }
    };

    struct From: SubQuery {
        From& morph(const types::SQLTable &table) {
            source->append(" FROM ").append(table);
            return *this;
        }

        From& morph(const SubQuery &subQuery) {
            source->append(" FROM (").append(*subQuery.source).append(")");
            return *this;
        }

        JoinOp& joinOp(const char *joinOp) { return ((JoinOp*) this)->morph(joinOp); }
        Where& where(const expr::ConditionExpr &expr) { return ((Where*) this)->morph(expr); }
        Group& group() { return ((Group*) this)->morph(); }
        Order& order() { return ((Order*) this)->morph(); }
        Limit& limit(int limit) { return ((Limit*) this)->morph(limit); }
        Offset& limit(int limit, int offset) { return ((Limit*) this)->morph(limit, offset); }
    };

    struct Select: Query {
        template<typename Item, typename... Items>
        explicit Select(const Item& item, const Items&... items) {
            sql.append("SELECT ");
            append(item, "");
            (append(items), ...);
        }

        template<typename T>
        void append(const types::SQLCol<T> &col, const char *sep = ", ") {
            sql.append(sep).append(col);
        }
        template<typename T>
        void append(const T& item, const char *sep = ", ") {
            sql.append(sep).append(item);
        }
        void append(const char *str, const char *sep = ", ") {
            sql.append(sep).append("'").append(str).append("'");
        }
        void append(const expr::AsExpr &expr, const char *sep = ", ") {
            sql.append(sep).append(expr);
        }
        void append(const expr::NumExpr &expr, const char *sep = ", ") {
            sql.append(sep).append(expr);
        }
        void append(const SubQuery &subQuery, const char *sep = ", ") {
            sql.append(sep).append("(").append(*subQuery.source).append(")");
        }

        From& from(const types::SQLTable &table) {
            return ((From*) this)->morph(table);
        };

        From& from(const SubQuery &subQuery) {
            return ((From*) this)->morph(subQuery);
        };
    };
}

#endif //SQLPP_SELECT_H

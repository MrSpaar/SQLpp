//
// Created by mrspaar on 7/13/23.
//

#ifndef SQLPP_SELECT_H
#define SQLPP_SELECT_H

#include "queries/base.h"


namespace sqlpp::keywords::select {
    struct Offset: SubQuery {
        Offset& morph(int offset) {
            *source << " OFFSET " << offset;
            return *this;
        }
    };

    struct Limit: SubQuery {
        Limit& morph(int limit) {
            *source << " LIMIT " << limit;
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
            *source << sep << col.name;
        }
        void append(const expr::OrderExpr &expr, const char *sep = ", ") {
            *source << sep << expr.sql.str();
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
            *source << " ORDER BY ";
            return ((OrderBy*) this)->morph(std::forward<const Items&>(items)...);
        }
    };

    struct Having: SubQuery {
        Having& morph(const expr::ConditionExpr &expr) {
            *source << " HAVING " << expr.sql.str();
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
            *source << sep << col.name;
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
            *source << " GROUP BY ";
            return ((GroupBy*) this)->morph(std::forward<const Items&>(items)...);
        }
    };

    struct Where: SubQuery {
        Where& morph(const expr::ConditionExpr &expr) {
            *source << " WHERE " << expr.sql.str();
            return *this;
        }

        Where& and_(const expr::ConditionExpr &expr) {
            *source << " AND " << expr.sql.str();
            return *this;
        }

        Where& or_(const expr::ConditionExpr &expr) {
            *source << " OR " << expr.sql.str();
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
            *source << " ON " << expr.sql.str();
            return *this;
        }

        On& and_(const expr::ConditionExpr &expr) {
            *source << " AND " << expr.sql.str();
            return *this;
        }

        On& or_(const expr::ConditionExpr &expr) {
            *source << " OR " << expr.sql.str();
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
            *source << " JOIN " << table.name;
            return *this;
        }

        On& on(const expr::ConditionExpr &expr) {
            return ((On*) this)->morph(expr);
        }
    };

    struct JoinOp: Keyword {
        JoinOp& morph(const char *joinOp) {
            *source << joinOp;
            return *this;
        }

        Join& join(const types::SQLTable &table) {
            return ((Join*) this)->morph(table);
        }
    };

    struct From: SubQuery {
        From& morph(const types::SQLTable &table) {
            *source << " FROM " << table.name;
            return *this;
        }

        From& morph(const SubQuery &subQuery) {
            *source << " FROM (" << subQuery.source->str() << ")";
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
            sql << "SELECT ";
            append(item, "");
            (append(items), ...);
        }

        template<typename T>
        void append(const types::SQLCol<T> &col, const char *sep = ", ") {
            sql << sep << col.name;
        }
        template<typename T>
        void append(const T& item, const char *sep = ", ") {
            sql << sep << item;
        }
        void append(const char *str, const char *sep = ", ") {
            sql << sep << '\'' << str << '\'';
        }
        void append(const expr::AsExpr &expr, const char *sep = ", ") {
            sql << sep << expr.sql.str();
        }
        void append(const expr::NumExpr &expr, const char *sep = ", ") {
            sql << sep << expr.sql.str();
        }
        void append(const SubQuery &subQuery, const char *sep = ", ") {
            sql << sep << "(" << subQuery.source->str() << ")";
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

//
// Created by mrspaar on 7/13/23.
//

#ifndef SQLPP_SELECT_H
#define SQLPP_SELECT_H

#include "queries/base.h"


namespace sqlpp::keywords::select {
    struct Offset: SubQuery {
        inline Offset& morph(int offset) {
            *source << " OFFSET " << offset;
            return *this;
        }
    };

    struct Limit: SubQuery {
        inline Limit& morph(int limit) {
            *source << " LIMIT " << limit;
            return *this;
        }

        inline Offset& morph(int limit, int offset) { return morph(limit).offset(offset); }
        inline Offset& offset(int offset) { return ((Offset*) this)->morph(offset); }
    };

    struct OrderBy: SubQuery {
        template<typename Item, typename... Items>
        inline OrderBy& morph(const Item &item, const Items&... items) {
            append(item, "");
            (append(items), ...);
            return *this;
        }

        template<typename T>
        inline void append(const types::SQLCol<T> &col, const char *sep = ", ") {
            *source << sep << col.name;
        }
        inline void append(const expr::OrderExpr &expr, const char *sep = ", ") {
            *source << sep << expr.sql.str();
        }

        inline Limit& limit(int limit) { return ((Limit*) this)->morph(limit); }
        inline Offset& limit(int limit, int offset) { return ((Limit*) this)->morph(limit, offset); }
    };

    struct Order: Keyword {
        inline Order& morph() {
            return *this;
        }

        template<typename... Items>
        inline OrderBy& by(const Items&... items) {
            *source << " ORDER BY ";
            return ((OrderBy*) this)->morph(std::forward<const Items&>(items)...);
        }
    };

    struct Having: SubQuery {
        inline Having& morph(const expr::ConditionExpr &expr) {
            *source << " HAVING " << expr.sql.str();
            return *this;
        }

        inline Order& order() { return ((Order*) this)->morph(); }
        inline Limit& limit(int limit) { return ((Limit*) this)->morph(limit); }
        inline Offset& limit(int limit, int offset) { return ((Limit*) this)->morph(limit, offset); }
    };

    struct GroupBy: SubQuery {
        template<typename Item, typename... Items>
        inline GroupBy& morph(const Item& item, const Items&... items) {
            append(item, "");
            (append(items), ...);
            return *this;
        }

        template<typename T>
        inline void append(const types::SQLCol<T> &col, const char *sep = ", ") {
            *source << sep << col.name;
        }

        inline Having& having(const expr::ConditionExpr &expr) { return ((Having*) this)->morph(expr); }
        inline Order& order() { return ((Order*) this)->morph(); }
        inline Limit& limit(int limit) { return ((Limit*) this)->morph(limit); }
        inline Offset& limit(int limit, int offset) { return ((Limit*) this)->morph(limit, offset); }
    };

    struct Group: Keyword {
        inline Group& morph() {
            return *this;
        }

        template<typename... Items>
        inline GroupBy& by(const Items&... items) {
            *source << " GROUP BY ";
            return ((GroupBy*) this)->morph(std::forward<const Items&>(items)...);
        }
    };

    struct Where: SubQuery {
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

        inline Group& group() { return ((Group*) this)->morph(); }
        inline Having& having(const expr::ConditionExpr &expr) { return ((Having*) this)->morph(expr); }
        inline Order& order() { return ((Order*) this)->morph(); }
        inline Limit& limit(int limit) { return ((Limit*) this)->morph(limit); }
        inline Offset& limit(int limit, int offset) { return ((Limit*) this)->morph(limit, offset); }
    };

    struct On: SubQuery {
        inline On& morph(const expr::ConditionExpr &expr) {
            *source << " ON " << expr.sql.str();
            return *this;
        }

        inline On& and_(const expr::ConditionExpr &expr) {
            *source << " AND " << expr.sql.str();
            return *this;
        }

        inline On& or_(const expr::ConditionExpr &expr) {
            *source << " OR " << expr.sql.str();
            return *this;
        }

        inline Where& where(const expr::ConditionExpr &expr) { return ((Where*) this)->morph(expr); }
        inline Group& group() { return ((Group*) this)->morph(); }
        inline Order& order() { return ((Order*) this)->morph(); }
        inline Limit& limit(int limit) { return ((Limit*) this)->morph(limit); }
        inline Offset& limit(int limit, int offset) { return ((Limit*) this)->morph(limit, offset); }
    };

    struct Join: Keyword {
        inline Join& morph(const types::SQLTable &table) {
            *source << " JOIN " << table.name;
            return *this;
        }

        inline On& on(const expr::ConditionExpr &expr) {
            return ((On*) this)->morph(expr);
        }
    };

    struct JoinOp: Keyword {
        inline JoinOp& morph(const char *joinOp) {
            *source << joinOp;
            return *this;
        }

        inline Join& join(const types::SQLTable &table) {
            return ((Join*) this)->morph(table);
        }
    };

    struct From: SubQuery {
        inline From& morph(const types::SQLTable &table) {
            *source << " FROM " << table.name;
            return *this;
        }

        inline From& morph(const SubQuery &subQuery) {
            *source << " FROM (" << subQuery.source->str() << ")";
            return *this;
        }

        inline JoinOp& joinOp(const char *joinOp) { return ((JoinOp*) this)->morph(joinOp); }
        inline Where& where(const expr::ConditionExpr &expr) { return ((Where*) this)->morph(expr); }
        inline Group& group() { return ((Group*) this)->morph(); }
        inline Order& order() { return ((Order*) this)->morph(); }
        inline Limit& limit(int limit) { return ((Limit*) this)->morph(limit); }
        inline Offset& limit(int limit, int offset) { return ((Limit*) this)->morph(limit, offset); }
    };

    struct Select: Query {
        template<typename Item, typename... Items>
        explicit Select(const Item& item, const Items&... items) {
            sql << "SELECT ";
            append(item, "");
            (append(items), ...);
        }

        template<typename T>
        inline void append(const types::SQLCol<T> &col, const char *sep = ", ") {
            sql << sep << col.name;
        }
        template<typename T>
        inline void append(const T& item, const char *sep = ", ") {
            sql << sep << item;
        }
        inline void append(const char *str, const char *sep = ", ") {
            sql << sep << '\'' << str << '\'';
        }
        inline void append(const expr::AsExpr &expr, const char *sep = ", ") {
            sql << sep << expr.sql.str();
        }
        inline void append(const expr::NumExpr &expr, const char *sep = ", ") {
            sql << sep << expr.sql.str();
        }
        inline void append(const SubQuery &subQuery, const char *sep = ", ") {
            sql << sep << "(" << subQuery.source->str() << ")";
        }

        inline From& from(const types::SQLTable &table) {
            return ((From*) this)->morph(table);
        };

        inline From& from(const SubQuery &subQuery) {
            return ((From*) this)->morph(subQuery);
        };
    };
}

#endif //SQLPP_SELECT_H

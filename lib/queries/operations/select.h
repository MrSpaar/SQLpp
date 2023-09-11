//
// Created by mrspaar on 7/13/23.
//

#ifndef SQLPP_SELECT_H
#define SQLPP_SELECT_H

#include "queries/base.h"


namespace sqlpp::keywords::select {
    struct Offset: Keyword {
        inline Offset& morph(int offset) {
            source->append(" OFFSET ").append(std::to_string(offset));
            return *this;
        }
    };

    struct Limit: Keyword {
        inline Limit& morph(int limit) {
            source->append(" LIMIT ").append(std::to_string(limit));
            return *this;
        }

        inline Offset& morph(int limit, int offset) {
            return morph(limit).offset(offset);
        }

        inline Offset& offset(int offset) {
            return ((Offset*) this)->morph(offset);
        }
    };

    struct OrderBy: Keyword {
        template<typename... Items>
        inline OrderBy& morph(const Items&... items) {
            if constexpr ((!std::is_same_v<Items, expr::OrderExpr> || ...))
                static_assert(
                        traits::always_false<Items...>::value,
                        "ORDER BY only accepts SQL columns and order expressions"
                );

            (source->append(items.sql).append(", "), ...);
            source->pop_back(); source->pop_back();

            return *this;
        }

        inline Limit& limit(int limit) {
            return ((Limit*) this)->morph(limit);
        }

        inline Offset& limit(int limit, int offset) {
            return ((Limit*) this)->morph(limit, offset);
        }
    };

    struct Order: Keyword {
        inline Order& morph() {
            source->append(" ORDER");
            return *this;
        }

        template<typename... Items>
        inline OrderBy& by(const Items&... items) {
            source->append(" BY ");
            return ((OrderBy*) this)->morph(std::forward<const Items&>(items)...);
        }
    };

    struct Having: Keyword {
        inline Having& morph(const expr::CondExpr &expr) {
            source->append(" HAVING ").append(expr.sql);
            return *this;
        }

        inline Order& order() {
            return ((Order*) this)->morph();
        }

        inline Limit& limit(int limit) {
            return ((Limit*) this)->morph(limit);
        }

        inline Offset& limit(int limit, int offset) {
            return ((Limit*) this)->morph(limit, offset);
        }
    };

    struct GroupBy: Keyword {
        template<typename... Items>
        inline GroupBy& morph(const Items&... items) {
            if constexpr ((!traits::is_sql_col<Items>::value || ...))
                static_assert(
                        traits::always_false<Items...>::value,
                        "GROUP BY only accepts SQL columns"
                );

            (source->append(items.name).append(", "), ...);
            source->pop_back(); source->pop_back();

            return *this;
        }

        inline Having& having(const expr::CondExpr &expr) {
            return ((Having*) this)->morph(expr);
        }

        inline Order& order() {
            return ((Order*) this)->morph();
        }

        inline Limit& limit(int limit) {
            return ((Limit*) this)->morph(limit);
        }

        inline Offset& limit(int limit, int offset) {
            return ((Limit*) this)->morph(limit, offset);
        }
    };

    struct Group: Keyword {
        inline Group& morph() {
            source->append(" GROUP");
            return *this;
        }

        template<typename... Items>
        inline GroupBy& by(const Items&... items) {
            source->append(" BY ");
            return ((GroupBy*) this)->morph(std::forward<const Items&>(items)...);
        }
    };

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

        inline Group& group() {
            return ((Group*) this)->morph();
        }

        inline Having& having(const expr::CondExpr &expr) {
            return ((Having*) this)->morph(expr);
        }

        inline Order& order() {
            return ((Order*) this)->morph();
        }

        inline Limit& limit(int limit) {
            return ((Limit*) this)->morph(limit);
        }

        inline Offset& limit(int limit, int offset) {
            return ((Limit*) this)->morph(limit, offset);
        }
    };

    struct From: Keyword {
        inline Where& where(const expr::CondExpr &expr) {
            return ((Where*) this)->morph(expr);
        }

        inline Group& group() {
            return ((Group*) this)->morph();
        }

        inline Order& order() {
            return ((Order*) this)->morph();
        }

        inline Limit& limit(int limit) {
            return ((Limit*) this)->morph(limit);
        }

        inline Offset& limit(int limit, int offset) {
            return ((Limit*) this)->morph(limit, offset);
        }
    };

    struct Join: From {
        inline Join& on(const expr::CondExpr &expr) {
            source->append(" ON ").append(expr.sql);
            return *this;
        }

        inline Join& and_(const expr::CondExpr &expr) {
            source->append(" AND ").append(expr.sql);
            return *this;
        }

        inline Join& or_(const expr::CondExpr &expr) {
            source->append(" OR ").append(expr.sql);
            return *this;
        }
    };

    struct FromJoin: From {
        inline FromJoin& morph(const types::SQLTable &table) {
            source->append(" FROM ").append(table.name);
            return *this;
        }

        inline From& crossJoin(const types::SQLTable &table) {
            source->append(" CROSS JOIN ").append(table.name);
            return (From&) *this;
        }

        inline Join& join(const std::string &join, const types::SQLTable &table) {
            source->append(join).append(table.name);
            return (Join&) *this;
        }
    };

    struct Select: Query {
        template<typename... Items>
        explicit Select(const Items&... items) {
            sql.append("SELECT ");
            (append(items), ...);
        }

        template<typename T>
        inline void append(const types::SQLCol<T> &col) { sql.append(col.name).append(", "); }
        inline void append(const expr::AsExpr &expr) { sql.append(expr.sql).append(", "); }
        inline void append(const expr::AggregateExpr &expr) { sql.append(expr.sql).append(", "); }

        inline FromJoin& from(const types::SQLTable &table) {
            sql.pop_back(); sql.pop_back();
            return ((FromJoin*) this)->morph(table);
        };
    };
}

#endif //SQLPP_SELECT_H

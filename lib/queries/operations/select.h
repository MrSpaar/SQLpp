//
// Created by mrspaar on 7/13/23.
//

#ifndef SQLPP_SELECT_H
#define SQLPP_SELECT_H

#include "queries/base.h"


namespace sqlpp::keywords::select {
    struct Offset: SubQuery {
        inline Offset& morph(int offset) {
            source->append(" OFFSET ").append(std::to_string(offset));
            return *this;
        }
    };

    struct Limit: SubQuery {
        inline Limit& morph(int limit) {
            source->append(" LIMIT ").append(std::to_string(limit));
            return *this;
        }

        inline Offset& morph(int limit, int offset) { return morph(limit).offset(offset); }
        inline Offset& offset(int offset) { return ((Offset*) this)->morph(offset); }
    };

    struct OrderBy: SubQuery {
        template<typename... Items>
        inline OrderBy& morph(const Items&... items) {
            if constexpr (((!traits::is_sql_col<Items>::value && !std::is_same_v<Items, expr::OrderExpr>) || ...))
                static_assert(
                        traits::always_false<Items...>::value,
                        "ORDER BY only accepts SQL columns and order expressions"
                );

            (append(items), ...);
            source->pop_back(); source->pop_back();

            return *this;
        }

        template<typename ColType>
        inline void append(const types::SQLCol<ColType> &col) { source->append(col.name).append(", "); }
        inline void append(const expr::OrderExpr &expr) { source->append(expr.sql).append(", "); }

        inline Limit& limit(int limit) { return ((Limit*) this)->morph(limit); }
        inline Offset& limit(int limit, int offset) { return ((Limit*) this)->morph(limit, offset); }
    };

    struct Order: Keyword {
        inline Order& morph() override {
            source->append(" ORDER");
            return *this;
        }

        template<typename... Items>
        inline OrderBy& by(const Items&... items) {
            source->append(" BY ");
            return ((OrderBy*) this)->morph(std::forward<const Items&>(items)...);
        }
    };

    struct Having: SubQuery {
        inline Having& morph(const expr::CondExpr &expr) {
            source->append(" HAVING ").append(expr.sql);
            return *this;
        }

        inline Order& order() { return ((Order*) this)->morph(); }
        inline Limit& limit(int limit) { return ((Limit*) this)->morph(limit); }
        inline Offset& limit(int limit, int offset) { return ((Limit*) this)->morph(limit, offset); }
    };

    struct GroupBy: SubQuery {
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

        inline Having& having(const expr::CondExpr &expr) { return ((Having*) this)->morph(expr); }
        inline Order& order() { return ((Order*) this)->morph(); }
        inline Limit& limit(int limit) { return ((Limit*) this)->morph(limit); }
        inline Offset& limit(int limit, int offset) { return ((Limit*) this)->morph(limit, offset); }
    };

    struct Group: Keyword {
        inline Group& morph() override {
            source->append(" GROUP");
            return *this;
        }

        template<typename... Items>
        inline GroupBy& by(const Items&... items) {
            source->append(" BY ");
            return ((GroupBy*) this)->morph(std::forward<const Items&>(items)...);
        }
    };

    struct Where: SubQuery {
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

        inline Group& group() { return ((Group*) this)->morph(); }
        inline Having& having(const expr::CondExpr &expr) { return ((Having*) this)->morph(expr); }
        inline Order& order() { return ((Order*) this)->morph(); }
        inline Limit& limit(int limit) { return ((Limit*) this)->morph(limit); }
        inline Offset& limit(int limit, int offset) { return ((Limit*) this)->morph(limit, offset); }
    };

    struct On: SubQuery {
        inline On& morph(const expr::CondExpr &expr) {
            source->append(" ON ").append(expr.sql);
            return *this;
        }

        inline On& and_(const expr::CondExpr &expr) {
            source->append(" AND ").append(expr.sql);
            return *this;
        }

        inline On& or_(const expr::CondExpr &expr) {
            source->append(" OR ").append(expr.sql);
            return *this;
        }

        inline Where& where(const expr::CondExpr &expr) { return ((Where*) this)->morph(expr); }
        inline Group& group() { return ((Group*) this)->morph(); }
        inline Order& order() { return ((Order*) this)->morph(); }
        inline Limit& limit(int limit) { return ((Limit*) this)->morph(limit); }
        inline Offset& limit(int limit, int offset) { return ((Limit*) this)->morph(limit, offset); }
    };

    struct Join: Keyword {
        inline Join& morph(const types::SQLTable &table) {
            source->append(" JOIN ").append(table.name);
            return *this;
        }

        inline On& on(const expr::CondExpr &expr) {
            return ((On*) this)->morph(expr);
        }
    };

    struct JoinOp: Keyword {
        inline JoinOp& morph(const std::string &joinOp) {
            source->append(joinOp);
            return *this;
        }

        inline Join& join(const types::SQLTable &table) {
            return ((Join*) this)->morph(table);
        }
    };

    struct From: SubQuery {
        inline From& morph(const types::SQLTable &table) {
            source->append(" FROM ").append(table.name);
            return *this;
        }

        inline From& morph(const SubQuery &subQuery) {
            source->append(" FROM (").append(*subQuery.source).append(")");
            return *this;
        }

        inline JoinOp& joinOp(const std::string &joinOp) { return ((JoinOp*) this)->morph(joinOp); }
        inline Where& where(const expr::CondExpr &expr) { return ((Where*) this)->morph(expr); }
        inline Group& group() { return ((Group*) this)->morph(); }
        inline Order& order() { return ((Order*) this)->morph(); }
        inline Limit& limit(int limit) { return ((Limit*) this)->morph(limit); }
        inline Offset& limit(int limit, int offset) { return ((Limit*) this)->morph(limit, offset); }
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
        inline void append(const SubQuery &subQuery) { sql.append("(").append(*subQuery.source).append("), "); }

        inline From& from(const types::SQLTable &table) {
            sql.pop_back(); sql.pop_back();
            return ((From*) this)->morph(table);
        };

        inline From& from(const SubQuery &subQuery) {
            sql.pop_back(); sql.pop_back();
            return ((From*) this)->morph(subQuery);
        };
    };
}

#endif //SQLPP_SELECT_H

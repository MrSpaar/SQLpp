//
// Created by mrspaar on 7/13/23.
//

#ifndef SQLPP_SELECT_H
#define SQLPP_SELECT_H

#include "base.h"


namespace sqlpp::keywords::select {
    struct Offset: SubQuery {
        Offset& morph(int offset) {
            append(" OFFSET ").append(std::to_string(offset));
            return *this;
        }
    };

    struct Limit: SubQuery {
        Limit& morph(int limit) {
            append(" LIMIT ").append(std::to_string(limit));
            return *this;
        }

        Offset& morph(int limit, int offset) { return morph(limit).offset(offset); }
        Offset& offset(int offset) { return ((Offset*) this)->morph(offset); }
    };

    struct OrderBy: SubQuery {
        template<typename T, typename... Ts>
        OrderBy& morph(const T &item, const Ts&... items) {
            static_assert((
                      (std::is_same_v<T, expr::OrderExpr> || traits::is_sql_col_v<T>)
                      && ... && (std::is_same_v<Ts, expr::OrderExpr> || traits::is_sql_col_v<Ts>)
                  ), "Items must be of type expr::OrderExpr"
            );

            append(item);
            ((append(", "), append(items)), ...);

            return *this;
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
            append(" ORDER BY ");
            return ((OrderBy*) this)->morph(items...);
        }
    };

    struct Having: SubQuery {
        Having& morph(const expr::ConditionExpr &expr) {
            append(" HAVING ").append(expr);
            return *this;
        }

        Order& order() { return ((Order*) this)->morph(); }
        Limit& limit(int limit) { return ((Limit*) this)->morph(limit); }
        Offset& limit(int limit, int offset) { return ((Limit*) this)->morph(limit, offset); }
    };

    struct GroupBy: SubQuery {
        template<typename Item, typename... Items>
        GroupBy& morph(const Item& item, const Items&... items) {
            static_assert(
                    (traits::is_sql_col_v<Item> && ... && traits::is_sql_col_v<Items>),
                    "Items must be of type types::SQLCol"
            );

            append(item);
            ((append(", "), append(items)), ...);

            return *this;
        }

        Order& order() { return ((Order*) this)->morph(); }
        Having& having(const expr::ConditionExpr &expr) { return ((Having*) this)->morph(expr); }
        Limit& limit(int limit) { return ((Limit*) this)->morph(limit); }
        Offset& limit(int limit, int offset) { return ((Limit*) this)->morph(limit, offset); }
    };

    struct Group: Keyword {
        Group& morph() {
            return *this;
        }

        template<typename... Items>
        GroupBy& by(const Items&... items) {
            append(" GROUP BY ");
            return ((GroupBy*) this)->morph(items...);
        }
    };

    struct Where: SubQuery {
        Where& morph(const expr::ConditionExpr &expr) {
            append(" WHERE ").append(expr);
            return *this;
        }

        Where& and_(const expr::ConditionExpr &expr) {
            append(" AND ").append(expr);
            return *this;
        }

        Where& or_(const expr::ConditionExpr &expr) {
            append(" OR ").append(expr);
            return *this;
        }

        Group& group() { return ((Group*) this)->morph(); }
        Order& order() { return ((Order*) this)->morph(); }
        Having& having(const expr::ConditionExpr &expr) { return ((Having*) this)->morph(expr); }
        Limit& limit(int limit) { return ((Limit*) this)->morph(limit); }
        Offset& limit(int limit, int offset) { return ((Limit*) this)->morph(limit, offset); }
    };

    struct On: SubQuery {
        On& morph(const expr::ConditionExpr &expr) {
            append(" ON ").append(expr);
            return *this;
        }

        On& and_(const expr::ConditionExpr &expr) {
            append(" AND ").append(expr);
            return *this;
        }

        On& or_(const expr::ConditionExpr &expr) {
            append(" OR ").append(expr);
            return *this;
        }

        Where& where(const expr::ConditionExpr &expr) { return ((Where*) this)->morph(expr); }
        Group& group() { return ((Group*) this)->morph(); }
        Order& order() { return ((Order*) this)->morph(); }
        Having& having(const expr::ConditionExpr &expr) { return ((Having*) this)->morph(expr); }
        Limit& limit(int limit) { return ((Limit*) this)->morph(limit); }
        Offset& limit(int limit, int offset) { return ((Limit*) this)->morph(limit, offset); }
    };

    struct Join: Keyword {
        Join& morph(const types::SQLTable &table) {
            append(" JOIN ").append(table);
            return *this;
        }

        On& on(const expr::ConditionExpr &expr) {
            return ((On*) this)->morph(expr);
        }
    };

    struct JoinOp: Keyword {
        JoinOp& morph(const char *joinOp) {
            append(joinOp);
            return *this;
        }

        Join& join(const types::SQLTable &table) {
            return ((Join*) this)->morph(table);
        }
    };

    struct From: SubQuery {
        From& morph(const types::SQLTable &table) {
            append(" FROM ").append(table);
            return *this;
        }

        From& morph(const SubQuery &subQuery) {
            append(" FROM (").append(subQuery).append(")");
            return *this;
        }

        JoinOp& joinOp(const char *joinOp) { return ((JoinOp*) this)->morph(joinOp); }
        Where& where(const expr::ConditionExpr &expr) { return ((Where*) this)->morph(expr); }
        Group& group() { return ((Group*) this)->morph(); }
        Order& order() { return ((Order*) this)->morph(); }
        Limit& limit(int limit) { return ((Limit*) this)->morph(limit); }
        Offset& limit(int limit, int offset) { return ((Limit*) this)->morph(limit, offset); }
    };

    struct Select: SubQuery {
        template<typename Item, typename... Items>
        explicit Select(const Item& item, const Items&... items) {
            append("SELECT ");
            add(item);
            ((append(", "), add(items)), ...);
        }

        template<typename T>
        [[maybe_unused]] void add(const types::SQLCol<T> &col) { append(col); }
        template<typename T, typename V>
        [[maybe_unused]] void add(const expr::MathExpr<T, V> &expr) { append(expr); }
        [[maybe_unused]] void add(const expr::AsExpr &expr) { append(expr); }
        [[maybe_unused]] void add(int value) { append(std::to_string(value)); }
        [[maybe_unused]] void add(double value) { append(std::to_string(value)); }
        [[maybe_unused]] void add(const char *str) { append("'").append(str).append("'"); }
        [[maybe_unused]] void add(const SubQuery &subQuery) { append("(").append(subQuery).append(")"); }

        From& from(const types::SQLTable &table) {
            return ((From*) this)->morph(table);
        };
        From& from(const SubQuery &subQuery) {
            return ((From*) this)->morph(subQuery);
        };
    };
}


#endif //SQLPP_SELECT_H

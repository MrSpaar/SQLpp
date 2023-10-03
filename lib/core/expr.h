//
// Created by mrspaar on 7/15/23.
//

#ifndef SQLPP_EXPR_H
#define SQLPP_EXPR_H

#include "types.h"


namespace sqlpp::expr {
    struct Expr: std::string {
        [[maybe_unused]] void add(const char *item) { append("'").append(item).append("'"); }
        [[maybe_unused]] void add(const std::string &item) { append("X'").append(item).append("'"); }

        template<typename T>
        [[maybe_unused]] void add(const T& item) {
            if constexpr (std::is_base_of_v<std::string, T>)
                append(item);
            else
                append(std::to_string(item));
        }
    };

    struct EqExpr: Expr {
        template<typename T>
        EqExpr(const std::string &colName, const T& value) {
            append(colName).append(" = "); add(value);
        }
    };

    struct AsExpr: Expr {
        AsExpr(const std::string &colName, const char *alias) {
            append(colName).append(" AS '").append(alias).append("'");
        }

        AsExpr(std::string *source, const char *alias) {
            append("(").append(*source).append(") AS '").append(alias).append("'");
        }

        AsExpr& morph(const char *alias) {
            append(") AS '").append(alias).append("'");
            return *this;
        }
    };

    struct OrderExpr: Expr {
        OrderExpr(const std::string &colName, const std::string &order) {
            append(colName).append(" ").append(order);
        }
    };

    template<typename Col, typename... Cols>
    struct TableExpr: Expr {
        explicit TableExpr(const std::string &tableName, const Col& col, const Cols&... cols) {
            append(tableName).append("(").append(col);
            ((append(", ").append(cols)), ...);
            append(")");
        }
    };

    struct ConditionExpr: Expr {
        ConditionExpr() = default;

        ConditionExpr(const std::string &colName, const char *value) {
            append(colName).append(" LIKE ").append(value);
        }

        template<typename T>
        ConditionExpr(const std::string &colName, const char *op, const T& value) {
            append(colName).append(" ").append(op).append(" "); add(value);
        }
    };

    template<typename ItemType, typename ReturnType>
    struct MathExpr: Expr {
        explicit MathExpr(const char *func) {
            append(func).append("()");
        }

        template<typename T>
        MathExpr(const std::string &colName, const char *op, const T& item) {
            append(colName).append(" ").append(op).append(" "); add(item);
        }

        template<typename T, typename... Ts>
        MathExpr(const char *func, const T& item, const Ts&... items) {
            static_assert(
                    (traits::is_compatible_v<T, ItemType> && ... && traits::is_compatible_v<Ts, ItemType>),
                    "Incompatible types"
            );

            append(func).append("(");
            add(item); ((append(", "), add(items)), ...);
            append(")");
        }

        AsExpr operator|=(const char *alias) {
            insert(0, "(");
            return ((AsExpr*) this)->morph(alias);
        }

        template<typename T>
        MathExpr& operator+(const T& item) { return op(" + ", item); }
        template<typename T>
        MathExpr& operator-(const T& item) { return op(" - ", item); }
        template<typename T>
        MathExpr& operator*(const T& item) { return op(" * ", item); }
        template<typename T>
        MathExpr& operator/(const T& item) { return op(" / ", item); }
        template<typename T>
        MathExpr& operator%(const T& item) { return op(" % ", item); }

        template<typename T>
        ConditionExpr& operator==(const T& item) { return cond(" = ", item); }
        template<typename T>
        ConditionExpr& operator!=(const T& item) { return cond(" != ", item); }
        template<typename T>
        ConditionExpr& operator<(const T& item) { return cond(" < ", item); }
        template<typename T>
        ConditionExpr& operator>(const T& item) { return cond(" > ", item); }
        template<typename T>
        ConditionExpr& operator<=(const T& item) { return cond(" <= ", item); }
        template<typename T>
        ConditionExpr& operator>=(const T& item) { return cond(" >= ", item); }

        template<typename T, typename V>
        expr::ConditionExpr& between(const T &lower, const V &upper) {
            static_assert(
                    traits::is_compatible_v<T, ReturnType> && traits::is_compatible_v<V, ReturnType>,
                    "Incompatible types"
            );

            append(" BETWEEN "); add(lower); append(" AND "); add(upper);
            return *(expr::ConditionExpr*) this;
        }

        template<typename T, typename... Ts>
        expr::ConditionExpr& in(const T &value, const Ts&... values) {
            static_assert((traits::is_compatible_v<Ts, ReturnType> && ...), "Incompatible types");

            append(" IN ("); add(value);
            ((append(", "), add(values)), ...);
            append(")");

            return *(expr::ConditionExpr*) this;
        }

        template<typename T>
        ConditionExpr& cond(const char *op, const T& item) {
            static_assert(traits::is_compatible_v<T, ReturnType>, "Incompatible types");
            append(op); add(item);
            return *(ConditionExpr*) this;
        }

        template<typename T>
        MathExpr& op(const char *op, const T& item) {
            static_assert(traits::is_compatible_v<T, ReturnType>, "Incompatible types");
            append(op); add(item);
            return *this;
        }
    };
}


#endif //SQLPP_EXPR_H

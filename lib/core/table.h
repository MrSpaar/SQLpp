//
// Created by mrspaar on 7/15/23.
//

#ifndef SQLPP_TABLE_H
#define SQLPP_TABLE_H

#include "expr.h"


struct sqlpp::types::SQLTable: std::string {
    template<typename... Cols>
    expr::TableExpr<Cols...> operator()(const Cols&... cols) {
        return expr::TableExpr<Cols...>(*this, cols...);
    }
};


template<typename ColType>
struct sqlpp::types::SQLCol: std::string {
    [[nodiscard]] expr::OrderExpr asc() const { return {*this, "ASC" }; }
    [[nodiscard]] expr::OrderExpr desc() const { return {*this, "DESC" }; }
    [[nodiscard]] expr::AsExpr operator|=(const char *alias) const { return {*this, alias}; }

    template<typename ValType>
    [[nodiscard]] expr::MathExpr<ColType> operator+(const ValType &value) const { return op("+", value); }
    template<typename ValType>
    [[nodiscard]] expr::MathExpr<ColType> operator-(const ValType &value) const { return op("-", value); }
    template<typename ValType>
    [[nodiscard]] expr::MathExpr<ColType> operator*(const ValType &value) const { return op("*", value); }
    template<typename ValType>
    [[nodiscard]] expr::MathExpr<ColType> operator/(const ValType &value) const { return op("/", value); }
    template<typename ValType>
    [[nodiscard]] expr::MathExpr<ColType> operator%(const ValType &value) const { return op("%", value); }

    template<typename ValType>
    [[nodiscard]] expr::ConditionExpr operator==(const ValType &value) const { return cond("=", value); }
    template<typename ValType>
    [[nodiscard]] expr::ConditionExpr operator!=(const ValType &value) const { return cond("!=", value); }
    template<typename ValType>
    [[nodiscard]] expr::ConditionExpr operator<(const ValType &value) const { return cond("<", value); }
    template<typename ValType>
    [[nodiscard]] expr::ConditionExpr operator>(const ValType &value) const { return cond(">", value); }
    template<typename ValType>
    [[nodiscard]] expr::ConditionExpr operator<=(const ValType &value) const { return cond("<=", value); }
    template<typename ValType>
    [[nodiscard]] expr::ConditionExpr operator>=(const ValType &value) const { return cond(">=", value); }
    [[nodiscard]] expr::ConditionExpr operator%=(const char *value) const { return {*this, value}; }

    template<typename T, typename V>
    [[nodiscard]] expr::ConditionExpr between(const T &lower, const V &upper) const {
        static_assert(
                traits::is_compatible_v<T, ColType> && traits::is_compatible_v<V, ColType>,
                "Invalid BETWEEN clause"
        );

        expr::ConditionExpr expr;

        expr.append(*this).append(" BETWEEN ");
        expr.add(lower);
        expr.append(" AND ");
        expr.add(upper);

        return expr;
    }

    template<typename T, typename... Ts>
    [[nodiscard]] expr::ConditionExpr in(const T &value, const Ts&... values) const {
        static_assert(
                (traits::is_compatible_v<T, ColType> && ... && traits::is_compatible_v<Ts, ColType>),
                "Invalid IN clause"
        );

        expr::ConditionExpr expr;

        expr.append(*this).append(" IN (");
        expr.add(value);
        ((expr.append(", "), expr.add(values)), ...);
        expr.append(")");

        return expr;
    }

    template<typename T>
    [[nodiscard]] expr::EqExpr operator=(const T& value) {
        static_assert(traits::is_compatible_v<T, ColType>, "Invalid assignment");
        return {*this, value};
    }

    template<typename T>
    expr::MathExpr<ColType> op(const char *op, const T& value) const {
        static_assert(traits::is_compatible_v<T, ColType>, "Invalid arithmetic operation");
        return {*this, op, value};
    }

    template<typename T>
    expr::ConditionExpr cond(const char *cond, const T& value) const {
        static_assert(traits::is_compatible_v<T, ColType>, "Invalid condition");
        return {*this, cond, value};
    }
};


#endif //SQLPP_TABLE_H

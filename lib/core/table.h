//
// Created by mrspaar on 7/15/23.
//

#ifndef SQLPP_TABLE_H
#define SQLPP_TABLE_H

#include "expr.h"


struct sqlpp::types::SQLTable {
    const char *name;
    explicit SQLTable(const char *name): name(name) {}

    template<typename... Cols>
    expr::TableExpr<Cols...> operator()(const Cols&... cols) {
        return expr::TableExpr<Cols...>(name, cols...);
    }
};


template<typename ColType>
struct sqlpp::types::SQLCol {
    const char *name;
    explicit SQLCol(const char *name): name(name) {}

    [[nodiscard]] expr::OrderExpr asc() const { return {name, "ASC" }; }
    [[nodiscard]] expr::OrderExpr desc() const { return {name, "DESC" }; }
    [[nodiscard]] expr::AsExpr operator|=(const char *alias) const { return {name, alias}; }

    template<typename ValType>
    [[nodiscard]] expr::NumExpr operator+(const ValType &value) const { return op("+", value); }
    template<typename ValType>
    [[nodiscard]] expr::NumExpr operator-(const ValType &value) const { return op("-", value); }
    template<typename ValType>
    [[nodiscard]] expr::NumExpr operator*(const ValType &value) const { return op("*", value); }
    template<typename ValType>
    [[nodiscard]] expr::NumExpr operator/(const ValType &value) const { return op("/", value); }
    template<typename ValType>
    [[nodiscard]] expr::NumExpr operator%(const ValType &value) const { return op("%", value); }

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

    [[nodiscard]] expr::ConditionExpr between(const ColType &lower, const ColType &upper) const {
        expr::ConditionExpr expr;
        expr.append(name).append(" BETWEEN "); expr.add(lower).append(" AND "); expr.add(upper);
        return expr;
    }

    [[nodiscard]] expr::ConditionExpr in(const std::initializer_list<ColType> &values) const {
        expr::ConditionExpr expr;
        expr.append(name).append(" IN (");

        for (const ColType &value : values)
            expr.add(value).append(", ");

        expr.pop_back(); expr.pop_back();
        expr.append(")");
        return expr;
    }

    template<typename T>
    [[nodiscard]] expr::EqExpr operator=(const T& value) {
        if constexpr (!traits::is_compatible_v<T, ColType>)
            static_assert(traits::always_false_v, "Invalid assignment");

        return {name, value};
    }

    template<typename T>
    expr::NumExpr op(const char *op, const T& value) const {
        if constexpr (!traits::is_compatible_v<T, ColType>)
            static_assert(traits::always_false_v, "Invalid arithmetic operation");

        return {name, op, value};
    }

    template<typename T>
    expr::ConditionExpr cond(const char *cond, const T& value) const {
        if constexpr (!traits::is_compatible_v<T, ColType>)
            static_assert(traits::always_false_v, "Invalid condition");

        return {name, cond, value};
    }
};


#endif //SQLPP_TABLE_H

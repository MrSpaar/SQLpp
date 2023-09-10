//
// Created by mrspaar on 7/15/23.
//

#ifndef SQLPP_COLUMN_H
#define SQLPP_COLUMN_H

#include "foreignkey.h"
#include "queries/expression.h"


template<typename T>
struct sqlpp::types::SQLCol {
    std::string name;
    int flags = 0;

    explicit SQLCol(SQLColList &cols, const std::string& colName, int flags) {
        this->name = colName;
        this->flags = flags;
        cols.push_back(this);
    }

    std::string serialize() const {
        std::string col = name;

        if constexpr (std::is_same_v<T, TEXT>) col += " TEXT";
        else if constexpr (std::is_same_v<T, INTEGER>) col += " INTEGER";
        else if constexpr (std::is_same_v<T, REAL>) col += " REAL";
        else if constexpr (std::is_same_v<T, BLOB>) col += " BLOB";

        if (flags & PRIMARY_KEY) col += " PRIMARY KEY";
        if (flags & AUTOINCREMENT) col += " AUTOINCREMENT";
        if (flags & NOT_NULL) col += " NOT NULL";
        if (flags & UNIQUE) col += " UNIQUE";

        return col;
    }

    expr::AsExpr operator|=(const std::string &alias) const {
        return expr::AsExpr{name, alias };
    }

    expr::OrderExpr asc() const { return expr::OrderExpr{name, "ASC" }; }
    expr::OrderExpr desc() const { return expr::OrderExpr{name, "DESC" }; }

    template<typename V>
    expr::EqExpr operator=(const V& value) {
        if constexpr (!std::is_same_v<T, V> && !(std::is_same_v<T, TEXT> && std::is_convertible_v<V, TEXT>))
            static_assert(traits::always_false<V>::value, "Invalid assignment");
        else if constexpr (std::is_convertible_v<V, TEXT>)
            return expr::EqExpr{name + " = '" + value + "'" };
        else if constexpr (std::is_same_v<V, BLOB>)
            return expr::EqExpr{name + " = '" + std::string(value.begin(), value.end()) + "'" };
        else
            return expr::EqExpr{name + " = " + std::to_string(value) };
    }

    template<typename V>
    expr::CompareExpr formatArithOp(const std::string &op, const V &item) {
        if constexpr (!std::is_convertible_v<T, REAL>)
            static_assert(traits::always_false<V>::value, "SQLCol must be numeric to perform arithmetic keywords");
        else if constexpr (traits::is_sql_col<V>::value)
            return expr::CompareExpr{name + " " + op + " " + item.name };
        else if constexpr (std::is_same_v<V, INTEGER> || std::is_same_v<V, REAL>)
            return expr::CompareExpr{name + " " + op + " " + std::to_string(item) };
        else
            static_assert(traits::always_false<V>::value, "Invalid arithmetic operation");
    }

    template<typename V>
    expr::CompareExpr operator+(const V &item) { return formatArithOp("+", item); }
    template<typename V>
    expr::CompareExpr operator-(const V &item) { return formatArithOp("-", item); }
    template<typename V>
    expr::CompareExpr operator*(const V &item) { return formatArithOp("*", item); }
    template<typename V>
    expr::CompareExpr operator/(const V &item) { return formatArithOp("/", item); }
    template<typename V>
    expr::CompareExpr operator%(const V &item) { return formatArithOp("%", item); }

    template<typename V>
    expr::ChainedExpr formatLogicOp(const std::string &op, const V &item) const {
        if constexpr (traits::is_sql_col<V>::value)
            return expr::ChainedExpr{name + " " + op + " " + item.name };
        else if constexpr (!std::is_same_v<T, V> && !(std::is_same_v<T, TEXT> && std::is_convertible_v<V, TEXT>))
            static_assert(traits::always_false<V>::value, "Invalid comparison");
        else if constexpr (std::is_convertible_v<V, TEXT>)
            return expr::ChainedExpr{name + " " + op + " '" + item + "'" };
        else if constexpr (std::is_same_v<V, BLOB>)
            return expr::ChainedExpr{name + " " + op + " '" + std::string(item.begin(), item.end()) + "'" };
        else
            return expr::ChainedExpr{name + " " + op + " " + std::to_string(item) };
    }

    template<typename V>
    expr::ChainedExpr operator<(const V &value) const { return formatLogicOp("<", value); }
    template<typename V>
    expr::ChainedExpr operator>(const V &value) const { return formatLogicOp(">", value); }
    template<typename V>
    expr::ChainedExpr operator<=(const V &value) const { return formatLogicOp("<=", value); }
    template<typename V>
    expr::ChainedExpr operator>=(const V &value) const { return formatLogicOp(">=", value); }
    template<typename V>
    expr::ChainedExpr operator!=(const V &value) const { return formatLogicOp("!=", value); }
    template<typename V>
    expr::ChainedExpr operator==(const V &value) const { return formatLogicOp("==", value); }
    expr::ChainedExpr operator^(const std::string & value) const { return formatLogicOp("LIKE", value); }

    expr::ChainedExpr between(const T& min, const T& max) const {
        if constexpr (std::is_same_v<T, TEXT>)
            return expr::ChainedExpr{name + " BETWEEN '" + min + "' AND '" + max + "'" };
        else
            return expr::ChainedExpr{name + " BETWEEN " + std::to_string(min) + " AND " + std::to_string(max) };
    }

    expr::ChainedExpr in(std::initializer_list<T> values) const {
        std::string res = " IN (";

        for (const T &value : values) {
            if constexpr (std::is_same_v<T, TEXT>) res += "'" + value + "', ";
            else res += std::to_string(value) + ", ";
        }

        res.pop_back(); res.pop_back();
        return expr::ChainedExpr{name + res + ")"};
    }
};


#endif //SQLPP_COLUMN_H

//
// Created by mrspaar on 7/15/23.
//

#ifndef SQLPP_EXPRESSION_H
#define SQLPP_EXPRESSION_H

#include <string>


namespace sqlpp::expr {
    struct Expr {
        std::string sql;
        explicit Expr(std::string sql): sql(std::move(sql)) {};
    };

    struct EqExpr: Expr {
        using Expr::Expr;

        EqExpr& operator,(const EqExpr &other) {
            this->sql.append(", ").append(other.sql);
            return *this;
        }
    };

    struct AsExpr: Expr {
        explicit AsExpr(const std::string &colName, const std::string &alias): Expr(colName) {
            this->sql.append(" AS '").append(alias).append("'");
        }
    };

    struct OrderExpr: Expr {
        explicit OrderExpr(const std::string &colName, const std::string &order): Expr(colName) {
            this->sql.append(" ").append(order);
        }
    };

    struct AggregateExpr: Expr {
        explicit AggregateExpr(const std::string &func, const std::string &item): Expr(item) {
            this->sql.insert(0, func + "(").append(")");
        }

        AsExpr operator|=(const std::string &alias) const {
            return AsExpr{ this->sql, alias };
        }
    };

    template<typename... Columns>
    struct CommaExpr: Expr {
        explicit CommaExpr(const std::string &tableName, Columns&&... cols): Expr(tableName+"(") {
            (this->sql.append(cols.name).append(", "), ...);
            this->sql.pop_back(); this->sql.pop_back();
            this->sql.append(")");
        }
    };

    struct ChainedExpr: Expr {
        using Expr::Expr;

        ChainedExpr& operator&&(const ChainedExpr &other) {
            this->sql.append(" AND ").append(other.sql);
            return *this;
        }

        ChainedExpr& operator||(const ChainedExpr &other) {
            this->sql.append(" OR ").append(other.sql);
            return *this;
        }
    };

    struct CompareExpr: ChainedExpr {
        using ChainedExpr::ChainedExpr;

        AsExpr operator|=(const std::string &alias) const { return AsExpr{ this->sql, alias }; }
        template<typename T>
        AsExpr operator|=(const types::SQLCol<T> &col) { return AsExpr{ this->sql, col.name }; }

        template<typename ReturnType, typename ItemType>
        ReturnType format(const std::string &op, const ItemType &item) const {
            if constexpr (traits::is_sql_col<ItemType>::value)
                return ReturnType{ this->sql + " " + op + " " + item.name };
            else if constexpr (std::is_same_v<ItemType, CompareExpr> || std::is_base_of_v<types::Runnable, ItemType>)
                return ReturnType{ this->sql + " " + op + " (" + item.sql + ")" };
            else if constexpr (std::is_same_v<ItemType, INTEGER> || std::is_same_v<ItemType, REAL>)
                return ReturnType{ this->sql + " " + op + " " + std::to_string(item) };
            else
                static_assert(
                        traits::always_false<ItemType>::value,
                        "Can only be used with INTEGER, REAL or SQLCol types"
                );
        }

        template<typename T>
        ChainedExpr operator==(const T &other) const { return format<ChainedExpr>("==", other); }
        template<typename T>
        ChainedExpr operator!=(const T &other) const { return format<ChainedExpr>("!=", other); }
        template<typename T>
        ChainedExpr operator<(const T &other) const { return format<ChainedExpr>("<", other); }
        template<typename T>
        ChainedExpr operator>(const T &other) const { return format<ChainedExpr>(">", other); }
        template<typename T>
        ChainedExpr operator<=(const T &other) const { return format<ChainedExpr>("<=", other); }
        template<typename T>
        ChainedExpr operator>=(const T &other) const { return format<ChainedExpr>(">=", other); }

        template<typename T>
        CompareExpr operator+(const T &other) const { return format<CompareExpr>("+", other); }
        template<typename T>
        CompareExpr operator-(const T &other) const { return format<CompareExpr>("-", other); }
        template<typename T>
        CompareExpr operator*(const T &other) const { return format<CompareExpr>("*", other); }
        template<typename T>
        CompareExpr operator/(const T &other) const { return format<CompareExpr>("/", other); }
        template<typename T>
        CompareExpr operator%(const T &other) const { return format<CompareExpr>("%", other); }
    };
}


namespace sqlpp::math {
    template<typename T>
    expr::AggregateExpr aggregateFunc(const std::string &func, const T &item) {
        if constexpr (std::is_same_v<T, INTEGER> || std::is_same_v<T, REAL>)
            return expr::AggregateExpr(func, std::to_string(item));
        else if constexpr (traits::is_sql_col<T>::value)
            return expr::AggregateExpr(func, item.name);
        else
            static_assert(traits::always_false<T>::value, "Can only be used with INTEGER, REAL or SQLCol types");
    }

    template<typename T>
    expr::CompareExpr numericFunc(const std::string &func, const T &item) {
        if constexpr (std::is_same_v<T, INTEGER> || std::is_same_v<T, REAL>)
            return expr::CompareExpr(func + "(" + std::to_string(item) + ")");
        else if constexpr (traits::is_numeric_col<T>::value)
            return expr::CompareExpr(func + "(" + item.name + ")");
        else
            static_assert(
                    traits::always_false<T>::value,
                    "Can only be used with INTEGER, REAL or numeric SQLCol types"
            );
    }

    template<typename T, typename V>
    expr::CompareExpr twoArgNumericFunc(const std::string &func, const T &item1, const V &item2) {
        std::string sql = func + "(";

        if constexpr (std::is_same_v<T, INTEGER> || std::is_same_v<T, REAL>)
            sql += std::to_string(item1);
        else if constexpr (traits::is_numeric_col<T>::value)
            sql += item1.name;
        else
            static_assert(
                    traits::always_false<T>::value,
                    "Can only be used with INTEGER, REAL or numeric SQLCol types"
            );

        sql += ", ";

        if constexpr (std::is_same_v<V, INTEGER> || std::is_same_v<V, REAL>)
            sql += std::to_string(item2);
        else if constexpr (traits::is_numeric_col<V>::value)
            sql += item2.name;
        else
            static_assert(
                    traits::always_false<V>::value,
                    "Can only be used with INTEGER, REAL or numeric SQLCol types"
            );

        return expr::CompareExpr{sql + ")"};
    }
}


namespace sqlpp {
    expr::Expr operator ""_SQL(const char *name, size_t) { return expr::Expr{name }; }
}


#endif //SQLPP_EXPRESSION_H

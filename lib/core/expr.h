//
// Created by mrspaar on 7/15/23.
//

#ifndef SQLPP_EXPR_H
#define SQLPP_EXPR_H

#include "types.h"


namespace sqlpp::expr {
    struct Expr {
        std::stringstream sql;

        template<typename T>
        const char* add(const T& item) {
            if constexpr (traits::is_sql_col_v<T>)
                sql << item.name;
            else if constexpr (std::is_same_v<T, BLOB>)
                sql << "X'" << item << "'";
            else if constexpr (std::is_convertible_v<T, std::string>)
                sql << "'" << item << "'";
            else
                sql << item;

            return "";
        }
    };

    struct EqExpr: Expr {
        template<typename T>
        EqExpr(const char *colName, const T& value) {
            sql << colName << " = " << add(value);
        }
    };

    struct AsExpr: Expr {
        AsExpr(const char *colName, const char *alias) {
            sql << colName << " AS '" << alias << "'";
        }

        AsExpr(std::stringstream *source, const char *alias, bool isSubQuery = false) {
            if (isSubQuery)
                sql << "(" << source->str() << ") AS '" << alias << "'";
            else
                sql << source->str() << " AS '" << alias << "'";
        }
    };

    struct OrderExpr: Expr {
        OrderExpr(const std::string &colName, const std::string &order) {
            sql << colName << " " << order;
        }
    };

    template<typename... Columns>
    struct TableExpr: Expr {
        explicit TableExpr(const char *tableName, const Columns&... cols) {
            sql << tableName << "(";
            ((sql << cols.name << ", "), ...);
            sql.seekp(-2, std::ios_base::end) << ")";
        }
    };

    struct NumericExpr: Expr {
        template<typename T>
        NumericExpr(const char *func, const T& item) {
            sql << func << "(" << add(item) << ")";
        }

        template<typename T1, typename T2>
        NumericExpr(const char *func, const T1& item1, const T2& item2) {
            sql << func << "(" << add(item1) << ", " << add(item2) << ")";
        }

        template<typename T>
        NumericExpr(const char *colName, const char *op, const T& value) {
            sql << colName << " " << op << " " << add(value);
        }

        AsExpr operator|=(const char *alias) {
            return {&sql, alias};
        }
    };

    struct ConditionExpr: Expr {
        ConditionExpr() = default;
        explicit ConditionExpr(std::stringstream &&sql): Expr{std::move(sql)} {}

        template<typename T>
        ConditionExpr(const char *colName, const char *op, const T& value) {
            sql << colName << " " << op << " " << add(value);
        }
    };

    template<typename ItemType>
    struct BetweenExpr: Expr {
        BetweenExpr(const char *colName, const ItemType &min) {
            sql << colName << " BETWEEN " << min;
        }

        ConditionExpr and_(const ItemType &max) {
            return ConditionExpr{
                std::move(sql << " AND " << max)
            };
        }
    };
}


#endif //SQLPP_EXPR_H
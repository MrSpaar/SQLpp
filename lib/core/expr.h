//
// Created by mrspaar on 7/15/23.
//

#ifndef SQLPP_EXPR_H
#define SQLPP_EXPR_H

#include "types.h"


namespace sqlpp::expr {
    struct Expr {
        std::string sql;

        template<typename T>
        auto add(const types::SQLCol<T>& item) {
            return sql.append(item.name);
        }
        auto add(const Expr &expr) {
            return sql.append(expr.sql);
        }
        auto add(const std::string &item) {
            return sql.append("X'").append(item).append("'");
        }
        auto add(const char *item) {
            return sql.append("'").append(item).append("'");
        }
        template<typename T>
        auto add(const T& item) {
            return sql.append(std::to_string(item));
        }
    };

    struct EqExpr: Expr {
        template<typename T>
        EqExpr(const char *colName, const T& value) {
            sql.append(colName).append(" = ").append(add(value));
        }
    };

    struct AsExpr: Expr {
        AsExpr(const char *colName, const char *alias) {
            sql.append(colName).append(" AS '").append(alias).append("'");
        }

        AsExpr(std::string *source, const char *alias, bool isSubQuery = false) {
            if (isSubQuery)
                sql.append("(").append(*source).append(") AS '").append(alias).append("'");
            else
                sql.append(*source).append(" AS '").append(alias).append("'");
        }
    };

    struct OrderExpr: Expr {
        OrderExpr(const std::string &colName, const std::string &order) {
            sql.append(colName).append(" ").append(order);
        }
    };

    template<typename Col, typename... Cols>
    struct TableExpr: Expr {
        explicit TableExpr(const char *tableName, const Col& col, const Cols&... cols) {
            sql.append(tableName).append("(").append(col.name);
            ((sql.append(", ").append(cols.name)), ...);
            sql.append(")");
        }
    };

    struct ConditionExpr: Expr {
        ConditionExpr() = default;

        template<typename T>
        ConditionExpr(const char *colName, const char *op, const T& value) {
            sql.append(colName).append(" ").append(op).append(" "); add(value);
        }

        template<typename T>
        ConditionExpr& morph(const char *op, const T& value) {
            sql.append(" ").append(op).append(" "); add(value);
            return *this;
        }
    };

    struct NumExpr: Expr {
        template<typename T>
        NumExpr(const char *func, const T& item) {
            sql.append(func).append("("); add(item).append(")");
        }

        template<typename T1, typename T2>
        NumExpr(const char *func, const T1& item1, const T2& item2) {
            sql.append(func).append("("); add(item1).append(", "); add(item2).append(")");
        }

        template<typename T>
        NumExpr(const char *colName, const char *op, const T& item) {
            sql.append(colName).append(" ").append(op).append(" "); add(item);
        }

        AsExpr operator|=(const char *alias) {
            return {&sql, alias};
        }

        template<typename T>
        ConditionExpr& operator==(const T& item) { return ((ConditionExpr*) this)->morph("=", item); }
        template<typename T>
        ConditionExpr& operator!=(const T& item) { return ((ConditionExpr*) this)->morph("!=", item); }
        template<typename T>
        ConditionExpr& operator<(const T& item) { return ((ConditionExpr*) this)->morph("<", item); }
        template<typename T>
        ConditionExpr& operator>(const T& item) { return ((ConditionExpr*) this)->morph(">", item); }
        template<typename T>
        ConditionExpr& operator<=(const T& item) { return ((ConditionExpr*) this)->morph("<=", item); }
        template<typename T>
        ConditionExpr& operator>=(const T& item) { return ((ConditionExpr*) this)->morph(">=", item); }

        template<typename T>
        NumExpr& operator+(const T& item) { sql.append(" + "); add(item); return *this; }
        template<typename T>
        NumExpr& operator-(const T& item) { sql.append(" - "); add(item); return *this; }
        template<typename T>
        NumExpr& operator*(const T& item) { sql.append(" * "); add(item); return *this; }
        template<typename T>
        NumExpr& operator/(const T& item) { sql.append(" / "); add(item); return *this; }
        template<typename T>
        NumExpr& operator%(const T& item) { sql.append(" % "); add(item); return *this; }
    };
}


#endif //SQLPP_EXPR_H

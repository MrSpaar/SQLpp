//
// Created by mrspaar on 7/15/23.
//

#ifndef SQLPP_EXPR_H
#define SQLPP_EXPR_H

#include "types.h"


namespace sqlpp::expr {
    struct Expr: std::string {
        void add(const Expr &expr) { append(expr); }
        void add(const char *item) { append("'").append(item).append("'"); }
        void add(const std::string &item) { append("X'").append(item).append("'"); }
        template<typename T> void add(const T& item) { append(std::to_string(item)); }
        template<typename T> void add(const types::SQLCol<T>& item) { append(item.name); }
        template<typename T> void add(const T& item, const char *sep) { append(sep); add(item); }
    };

    struct EqExpr: Expr {
        template<typename T>
        EqExpr(const char *colName, const T& value) {
            append(colName).append(" = "); add(value);
        }
    };

    struct AsExpr: Expr {
        AsExpr(const char *colName, const char *alias) {
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
        explicit TableExpr(const char *tableName, const Col& col, const Cols&... cols) {
            append(tableName).append("(").append(col.name);
            ((append(", ").append(cols.name)), ...);
            append(")");
        }
    };

    struct ConditionExpr: Expr {
        template<typename T>
        ConditionExpr(const char *colName, const char *op, const T& value) {
            append(colName).append(" ").append(op).append(" "); add(value);
        }

        template<typename T>
        ConditionExpr& morph(const char *op, const T& value) {
            append(" ").append(op).append(" "); add(value);
            return *this;
        }
    };

    struct NumExpr: Expr {
        explicit NumExpr(const char *func) { append(func); }

        template<typename T, typename... Ts>
        NumExpr(const char *func, const T& item, const Ts&... items) {
            append(func).append("(");
            add(item); ((add(items, ", ")), ...);
            append(")");
        }

        template<typename T>
        NumExpr(const char *colName, const char *op, const T& item) {
            append(colName).append(" ").append(op).append(" "); add(item);
        }

        template<typename T>
        NumExpr& operator+(const T& item) { append(" + "); add(item); return *this; }
        template<typename T>
        NumExpr& operator-(const T& item) { append(" - "); add(item); return *this; }
        template<typename T>
        NumExpr& operator*(const T& item) { append(" * "); add(item); return *this; }
        template<typename T>
        NumExpr& operator/(const T& item) { append(" / "); add(item); return *this; }
        template<typename T>
        NumExpr& operator%(const T& item) { append(" % "); add(item); return *this; }

        AsExpr operator|=(const char *alias) {
            insert(0, "(");
            return ((AsExpr*) this)->morph(alias);
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
    };
}


#endif //SQLPP_EXPR_H

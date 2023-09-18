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
        const char* add(const types::SQLCol<T>& item) {
            sql << item.name; return "";
        }
        const char* add(const Expr &expr) {
            sql << expr.sql.str(); return "";
        }
        const char* add(const std::string &item) {
            sql << "X'" << item << "'"; return "";
        }
        const char* add(const char *item) {
            sql << "'" << item << "'"; return "";
        }
        template<typename T>
        const char* add(const T& item) {
            sql << item; return "";
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

    struct ConditionExpr: Expr {
        ConditionExpr() = default;

        template<typename T>
        ConditionExpr(const char *colName, const char *op, const T& value) {
            sql << colName << " " << op << " " << add(value);
        }

        template<typename T>
        ConditionExpr& morph(const char *op, const T& value) {
            sql << " " << op << " " << add(value);
            return *this;
        }
    };

    struct NumExpr: Expr {
        template<typename T>
        NumExpr(const char *func, const T& item) {
            sql << func << "(" << add(item) << ")";
        }

        template<typename T1, typename T2>
        NumExpr(const char *func, const T1& item1, const T2& item2) {
            sql << func << "(" << add(item1) << ", " << add(item2) << ")";
        }

        template<typename T>
        NumExpr(const char *colName, const char *op, const T& item) {
            sql << colName << " " << op << " " << add(item);
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
        NumExpr& operator+(const T& item) { sql << " + " << add(item); return *this; }
        template<typename T>
        NumExpr& operator-(const T& item) { sql << " - " << add(item); return *this; }
        template<typename T>
        NumExpr& operator*(const T& item) { sql << " * " << add(item); return *this; }
        template<typename T>
        NumExpr& operator/(const T& item) { sql << " / " << add(item); return *this; }
        template<typename T>
        NumExpr& operator%(const T& item) { sql << " % " << add(item); return *this; }
    };
}


#endif //SQLPP_EXPR_H

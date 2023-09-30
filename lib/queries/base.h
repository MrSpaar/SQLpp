//
// Created by mrspaar on 7/25/23.
//

#ifndef SQLPP_BASE_H
#define SQLPP_BASE_H

#include "core/conn.h"


namespace sqlpp {
    namespace keywords { struct Runnable; }
    using SQLQuery [[maybe_unused]] = keywords::Runnable;
}

namespace sqlpp::keywords {
    struct Keyword: std::string {
        using std::string::string;

        void cout() const {
            std::cout << *this << ";" << std::endl;
        }
    };

    struct Runnable: Keyword {
        [[nodiscard]] SQLResult run(Connection &conn) {
            return conn.run(*this);
        }
    };

    struct SubQuery: Runnable {
        [[nodiscard]] expr::AsExpr operator|=(const char *alias) {
            return {*this, alias};
        }
    };

    struct Where: Runnable {
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
    };

    struct From: Runnable {
        From& morph(const types::SQLTable &table) {
            append(" FROM ").append(table);
            return *this;
        }

        From& morph(const SubQuery &subQuery) {
            append(" FROM (").append(subQuery).append(")");
            return *this;
        }

        Where& where(const expr::ConditionExpr &expr) {
            return ((Where*) this)->morph(expr);
        }
    };
}


#endif //SQLPP_BASE_H

//
// Created by mrspaar on 7/25/23.
//

#ifndef SQLPP_BASE_H
#define SQLPP_BASE_H

#include "core/conn.h"


namespace sqlpp::keywords {
    struct Keyword {
        std::string *source = nullptr;

        void cout() const {
            std::cout << *source << ";" << std::endl;
        }
    };

    struct Runnable: Keyword {
        [[nodiscard]] SQLResult exec(Connection &conn) {
            return conn.exec(source);
        }
    };

    struct SubQuery: Runnable {
        [[nodiscard]] expr::AsExpr operator|=(const char *alias) {
            return {source, alias};
        }
    };

    struct Query: Keyword {
        std::string sql;
        Query() { source = &sql; }
    };

    struct Where: Runnable {
        Where& morph(const expr::ConditionExpr &expr) {
            source->append(" WHERE ").append(expr);
            return *this;
        }

        Where& and_(const expr::ConditionExpr &expr) {
            source->append(" AND ").append(expr);
            return *this;
        }

        Where& or_(const expr::ConditionExpr &expr) {
            source->append(" OR ").append(expr);
            return *this;
        }
    };

    struct From: Runnable {
        From& morph(const types::SQLTable &table) {
            source->append(" FROM ").append(table);
            return *this;
        }

        From& morph(const SubQuery &subQuery) {
            source->append(" FROM (").append(*subQuery.source).append(")");
            return *this;
        }

        Where& where(const expr::ConditionExpr &expr) {
            return ((Where*) this)->morph(expr);
        }
    };
}


#endif //SQLPP_BASE_H

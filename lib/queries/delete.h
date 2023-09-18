//
// Created by mrspaar on 7/25/23.
//

#ifndef SQLPP_DELETE_H
#define SQLPP_DELETE_H

#include "queries/base.h"


namespace sqlpp::keywords::del {
    struct Where: Keyword {
        Where& morph(const expr::ConditionExpr &expr) {
            source->append(" WHERE ").append(expr.sql);
            return *this;
        }

        Where& and_(const expr::ConditionExpr &expr) {
            source->append(" AND ").append(expr.sql);
            return *this;
        }

        Where& or_(const expr::ConditionExpr &expr) {
            source->append(" OR ").append(expr.sql);
            return *this;
        }
    };

    struct From: Keyword {
        From& morph(const types::SQLTable &table) {
            source->append("FROM ").append(table.name);
            return *this;
        }

        Where& where(const expr::ConditionExpr &expr) {
            return ((Where*) this)->morph(expr);
        }
    };

    struct Delete: Query {
        Delete(): Query() { sql.append("DELETE "); }

        From& from(const types::SQLTable &table) {
            return ((From*) this)->morph(table);
        }
    };
}


#endif //SQLPP_DELETE_H

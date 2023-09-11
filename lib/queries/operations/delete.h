//
// Created by mrspaar on 7/25/23.
//

#ifndef SQLPP_DELETE_H
#define SQLPP_DELETE_H

#include "queries/base.h"


namespace sqlpp::keywords::del {
    struct Where: Keyword {
        inline Where& morph(const expr::CondExpr &expr) {
            source->append(" WHERE ").append(expr.sql);
            return *this;
        }

        inline Where& and_(const expr::CondExpr &expr) {
            source->append(" AND ").append(expr.sql);
            return *this;
        }

        inline Where& or_(const expr::CondExpr &expr) {
            source->append(" OR ").append(expr.sql);
            return *this;
        }
    };

    struct From: Keyword {
        inline From& morph(const types::SQLTable &table) {
            source->append("FROM ").append(table.name);
            return *this;
        }

        inline Where& where(const expr::CondExpr &expr) {
            return ((Where*) this)->morph(expr);
        }
    };

    struct Delete: Query {
        Delete(): Query() { sql.append("DELETE "); }

        inline From& from(const types::SQLTable &table) {
            return ((From*) this)->morph(table);
        }
    };
}


#endif //SQLPP_DELETE_H

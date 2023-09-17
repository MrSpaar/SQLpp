//
// Created by mrspaar on 7/25/23.
//

#ifndef SQLPP_DELETE_H
#define SQLPP_DELETE_H

#include "queries/base.h"


namespace sqlpp::keywords::del {
    struct Where: Keyword {
        inline Where& morph(const expr::ConditionExpr &expr) {
            *source << " WHERE " << expr.sql.str();
            return *this;
        }

        inline Where& and_(const expr::ConditionExpr &expr) {
            *source << " AND " << expr.sql.str();
            return *this;
        }

        inline Where& or_(const expr::ConditionExpr &expr) {
            *source << " OR " << expr.sql.str();
            return *this;
        }
    };

    struct From: Keyword {
        inline From& morph(const types::SQLTable &table) {
            *source << "FROM " << table.name;
            return *this;
        }

        inline Where& where(const expr::ConditionExpr &expr) {
            return ((Where*) this)->morph(expr);
        }
    };

    struct Delete: Query {
        Delete(): Query() { sql << "DELETE "; }

        inline From& from(const types::SQLTable &table) {
            return ((From*) this)->morph(table);
        }
    };
}


#endif //SQLPP_DELETE_H

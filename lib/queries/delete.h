//
// Created by mrspaar on 7/25/23.
//

#ifndef SQLPP_DELETE_H
#define SQLPP_DELETE_H

#include "queries/base.h"


namespace sqlpp::keywords::del {
    struct Where: Keyword {
        Where& morph(const expr::ConditionExpr &expr) {
            *source << " WHERE " << expr.sql.str();
            return *this;
        }

        Where& and_(const expr::ConditionExpr &expr) {
            *source << " AND " << expr.sql.str();
            return *this;
        }

        Where& or_(const expr::ConditionExpr &expr) {
            *source << " OR " << expr.sql.str();
            return *this;
        }
    };

    struct From: Keyword {
        From& morph(const types::SQLTable &table) {
            *source << "FROM " << table.name;
            return *this;
        }

        Where& where(const expr::ConditionExpr &expr) {
            return ((Where*) this)->morph(expr);
        }
    };

    struct Delete: Query {
        Delete(): Query() { sql << "DELETE "; }

        From& from(const types::SQLTable &table) {
            return ((From*) this)->morph(table);
        }
    };
}


#endif //SQLPP_DELETE_H

//
// Created by mrspaar on 7/25/23.
//

#ifndef SQLPP_DELETE_H
#define SQLPP_DELETE_H

#include "queries/base.h"


namespace sqlpp::keywords::Delete {
    struct Where: types::Runnable {
        explicit Where(const std::string &sql): Runnable(sql, "\nWHERE ") {}

        Runnable operator,(const expr::ChainedExpr &expr) {
            sql.append(expr.sql);
            return Runnable{sql, ""};
        };
    };

    struct DeleteIntermediate: types::Intermediate<Where, keywords::Where> {
        using types::Intermediate<Where, keywords::Where>::Intermediate;
    };

    struct DeleteFrom: types::Keyword {
        using Keyword::operator,;
        explicit DeleteFrom(): Keyword("DELETE FROM ") {}

        DeleteIntermediate operator,(const types::SQLTable &table) {
            sql.append(table.name);
            return DeleteIntermediate{sql};
        }
    };
}


#endif //SQLPP_DELETE_H

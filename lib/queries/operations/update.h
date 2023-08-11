//
// Created by mrspaar on 7/25/23.
//

#ifndef SQLPP_UPDATE_H
#define SQLPP_UPDATE_H

#include "queries/base.h"


namespace sqlpp::keywords::Update {
    struct Where: types::Runnable {
        using Runnable::operator,;
        explicit Where(const std::string &sql): Runnable(sql, "\nWHERE ") {}

        Runnable operator,(const expr::ChainedExpr &expr) {
            return Runnable{sql, expr.sql};
        }
    };

    struct Set: types::Runnable {
        using Runnable::operator,;
        explicit Set(const std::string &sql): Runnable(sql, "\nSET ") {}

        Set& operator,(const expr::EqExpr &expr) {
            sql.append(expr.sql).append(", ");
            return *this;
        };

        Where operator,(keywords::Where) { return Where(sql); }
    };

    struct SetIntermediate: types::Intermediate<Set, keywords::Set> {
        using types::Intermediate<Set, keywords::Set>::Intermediate;
        using types::Intermediate<Set, keywords::Set>::operator,;
    };

    struct Update: types::Keyword {
        using Keyword::Keyword;
        using Keyword::operator,;
        explicit Update(): Keyword("UPDATE ") {}

        SetIntermediate operator,(const types::SQLTable &table) {
            sql.append(table.name);
            return SetIntermediate{sql};
        }
    };

    struct UpdateOr: types::Keyword {
        using Keyword::operator,;
        explicit UpdateOr(): Keyword("UPDATE OR ") {}

        template<typename T>
        Update operator,(T) {
            if constexpr (std::is_same_v<T, keywords::Abort>)
                sql.append("ABORT ");
            else if constexpr (std::is_same_v<T, keywords::Fail>)
                sql.append("FAIL ");
            else if constexpr (std::is_same_v<T, keywords::Ignore>)
                sql.append("IGNORE ");
            else if constexpr (std::is_same_v<T, keywords::Replace>)
                sql.append("REPLACE ");
            else if constexpr (std::is_same_v<T, keywords::Rollback>)
                sql.append("ROLLBACK ");
            else
                static_assert(traits::always_false<T>::value, "Unsuported keyword");
            return Update{sql};
        };
    };
}


#endif //SQLPP_UPDATE_H

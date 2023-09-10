//
// Created by mrspaar on 7/13/23.
//

#ifndef SQLPP_SELECT_H
#define SQLPP_SELECT_H

#include "queries/base.h"


namespace sqlpp::keywords::select {
    constexpr char const havingStr[] = " HAVING ";
    struct Having: Keyword<Having, havingStr> {
        using Keyword::operator,;

        Having& operator,(const expr::ChainedExpr &expr) {
            source->append(expr.sql);
            return *this;
        }

        OrderBy& operator,(OrderBy&& token) { return token(source); };
        Limit& operator,(Limit&& token) { return token(source); };
    };


    constexpr char const groupByStr[] = " GROUP BY ";
    struct GroupBy: Keyword<GroupBy, groupByStr> {
        using Keyword::operator,;

        template<typename T>
        GroupBy& operator,(const types::SQLCol<T> &col) {
            source->append(col.name).append(", ");
            return *this;
        }

        Having& operator,(Having&& token) { return token(source); }
        OrderBy& operator,(OrderBy&& token) { return token(source); }
        Limit& operator,(Limit&& token) { return token(source);  }
    };


    struct SWhere: Where {
        using Where::operator,;

        SWhere& operator,(const expr::ChainedExpr &expr) override {
            source->append(" WHERE ").append(expr.sql);
            return *this;
        }

        GroupBy& operator,(GroupBy&& token) { return token(source); }
        Having& operator,(Having&& token) { return token(source); }
    };


    constexpr char const onStr[] = " ON ";
    struct On: Keyword<On, onStr> {
        using Keyword::operator,;

        On& operator,(const expr::ChainedExpr &expr) {
            source->append(expr.sql);
            return *this;
        }

        SWhere& operator,(Where&& token) { return (SWhere&) token(source); }
        GroupBy& operator,(GroupBy&& token) { return token(source); }
        Having& operator,(Having&& token) { return token(source); }
        OrderBy& operator,(OrderBy&& token) { return token(source); }
        Limit& operator,(Limit&& token) { return token(source); }
    } on;

    constexpr char const innerJoinStr[] = " INNER JOIN ";
    constexpr char const leftJoinStr[] = " LEFT JOIN ";
    constexpr char const crossJoinStr[] = " CROSS JOIN ";

    template<char const *str>
    struct Join: Keyword<Join<str>, str> {
        using Keyword<Join<str>, str>::operator,;

        Join& operator,(const types::SQLTable &table) {
            this->source->append(table.name);
            return *this;
        };

        On& operator,(On&& token) { return token(this->source); };
    };


    struct SFrom: From {
        using From::operator,;

        SFrom& operator,(const types::SQLTable &table) override {
            source->append(table.name);
            return *this;
        }

        template<char const *str>
        Join<str>& operator,(Join<str>&& token) { return token(source); };
        SWhere& operator,(Where&& token) override { return (SWhere&) token(source); };
        GroupBy& operator,(GroupBy&& token) { return token(source); };
        Having& operator,(Having&& token) { return token(source); };
        OrderBy& operator,(OrderBy&& token) { return token(source); };
        Limit& operator,(Limit&& token) { return token(source); };
    };


    constexpr char const selectStr[] = "SELECT ";
    struct Select: Keyword<Select, selectStr> {
        using Keyword::operator,;

        std::string sql;
        Select(): Keyword<Select, selectStr>() { operator()(&sql); }

        template<typename T>
        Select& operator,(const types::SQLCol<T> &col) {
            source->append(col.name).append(", ");
            return *this;
        }

        Select& operator,(const expr::AsExpr &expr) {
            source->append(expr.sql).append(", ");
            return *this;
        }

        Select& operator,(const expr::AggregateExpr &expr) {
            source->append(expr.sql).append(", ");
            return *this;
        }

        SFrom& operator,(From&& token) { return (SFrom&) token(source); };
    };
}

#endif //SQLPP_SELECT_H

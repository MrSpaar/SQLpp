//
// Created by mrspaar on 7/25/23.
//

#ifndef SQLPP_UPDATE_H
#define SQLPP_UPDATE_H

#include "queries/base.h"


namespace sqlpp::keywords::update {
    constexpr char const setStr[] = " SET ";
    struct Set: Keyword<Set, setStr> {
        using Keyword::operator,;

        Set& operator,(const expr::EqExpr &expr) {
            source->append(expr.sql);
            return *this;
        }

        From& operator,(From&& token) { return token(source); };
        Where& operator,(Where&& token) { return token(source); };
    };


    template<char const *str>
    struct UpdateOr: Or<str> {
        using Or<str>::operator,;

        UpdateOr& operator,(const types::SQLTable &table) {
            this->source->append(table.name);
            return *this;
        };

        Set& operator,(Set&& token) { return token(this->source); };
    };


    constexpr char const updateStr[] = "UPDATE ";
    struct Update: Keyword<Update, updateStr> {
        using Keyword::operator,;

        std::string sql;
        Update(): Keyword<Update, updateStr>() { operator()(&sql); }

        Update& operator,(const types::SQLTable &table) {
            sql.append(table.name);
            return *this;
        };

        template<char const *str>
        UpdateOr<str>& operator,(Or<str>&& token) { return (UpdateOr<str>&) token(source); };
        Set& operator,(Set&& token) { return token(source); };
    };
}


#endif //SQLPP_UPDATE_H

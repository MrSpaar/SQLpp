//
// Created by mrspaar on 7/25/23.
//

#ifndef SQLPP_DELETE_H
#define SQLPP_DELETE_H

#include "queries/base.h"


namespace sqlpp::keywords::del {
    constexpr char const deleteStr[] = "DELETE";
    struct Delete: Keyword<Delete, deleteStr> {
        using Keyword::operator,;

        std::string sql;
        Delete(): Keyword<Delete, deleteStr>() { operator()(&sql); }

        From& operator,(From&& token) { return token(source); };
    };
}


#endif //SQLPP_DELETE_H

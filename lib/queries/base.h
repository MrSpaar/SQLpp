//
// Created by mrspaar on 7/25/23.
//

#ifndef SQLPP_BASE_H
#define SQLPP_BASE_H

#include "core/db.h"


namespace sqlpp::keywords {
    struct Keyword {
        std::string *source = nullptr;

        virtual Keyword& morph() { return *this; }
        void cout() const { std::cout << *source << ";" << std::endl; }
    };

    struct SubQuery: Keyword {
        SubQuery& operator|=(const std::string &alias) {
            source->insert(0, "(")
                  .append(") AS ")
                  .append(alias);

            return *this;
        }
    };

    struct Query: Keyword {
        std::string sql;
        Query() { source = &sql; }
    };
}


#endif //SQLPP_BASE_H

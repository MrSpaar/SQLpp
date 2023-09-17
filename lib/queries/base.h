//
// Created by mrspaar on 7/25/23.
//

#ifndef SQLPP_BASE_H
#define SQLPP_BASE_H

#include <iostream>
#include "core/table.h"


namespace sqlpp::keywords {
    struct Keyword {
        std::stringstream *source = nullptr;

        void cout() const {
            std::cout << source->str() << ";" << std::endl;
        }
    };

    struct SubQuery: Keyword {
        expr::AsExpr operator|=(const char *alias) {
            return {source, alias, true};
        }
    };

    struct Query: Keyword {
        std::stringstream sql;
        Query() { source = &sql; }
    };
}


#endif //SQLPP_BASE_H
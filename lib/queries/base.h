//
// Created by mrspaar on 7/25/23.
//

#ifndef SQLPP_BASE_H
#define SQLPP_BASE_H

#include "core/conn.h"


namespace sqlpp::keywords {
    struct Keyword {
        std::string *source = nullptr;

        void cout() const {
            std::cout << *source << ";" << std::endl;
        }
    };

    struct Runnable: Keyword {
        [[nodiscard]] SQLResult exec(Connection &conn) {
            return conn.exec(source);
        }
    };

    struct SubQuery: Runnable {
        [[nodiscard]] expr::AsExpr operator|=(const char *alias) {
            return {source, alias, true};
        }
    };

    struct Query: Keyword {
        std::string sql;
        Query() { source = &sql; }
    };
}


#endif //SQLPP_BASE_H

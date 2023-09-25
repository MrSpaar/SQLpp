//
// Created by mrspaar on 7/25/23.
//

#ifndef SQLPP_DELETE_H
#define SQLPP_DELETE_H

#include "queries/base.h"


namespace sqlpp::keywords::del {
    struct Delete: Query {
        Delete(): Query() { sql.append("DELETE "); }

        From& from(const types::SQLTable &table) {
            return ((From*) this)->morph(table);
        }
    };
}


#endif //SQLPP_DELETE_H

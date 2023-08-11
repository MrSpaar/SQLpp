//
// Created by mrspaar on 7/15/23.
//

#ifndef SQLPP_FOREIGNKEY_H
#define SQLPP_FOREIGNKEY_H

#include "core/types.h"


struct sqlpp::types::SQLForeignKey {
    std::string col1;
    std::string tableName;
    std::string col2;

    explicit SQLForeignKey(
            SQLForeignKeyList &fkList,
            const std::string &col1Name,
            const std::string &foreignName,
            const std::string &col2Name
    ) {
        this->tableName = foreignName;
        this->col1 = col1Name;
        this->col2 = col2Name;
        fkList.push_back(this);
    }

    explicit operator std::string() const {
        return "FOREIGN KEY (" + col1 + ") REFERENCES " + tableName + "(" + col2 + ")";
    }
};


#endif //SQLPP_FOREIGNKEY_H

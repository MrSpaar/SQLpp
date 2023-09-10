//
// Created by mrspaar on 7/18/23.
//

#ifndef SQLPP_RESULT_H
#define SQLPP_RESULT_H

#include <map>
#include <iostream>
#include <sqlite3.h>
#include "table/table.h"

namespace sqlpp {
    struct SQLRow {
        std::map<std::string, std::string> row;
        auto begin() const { return row.begin(); }
        auto end() const { return row.end(); }

        template<typename T = TEXT>
        T get(const std::string &colName) const {
            try {
                if constexpr (std::is_same_v<T, INTEGER>)
                    return std::stoi(row.at(colName));
                else if constexpr (std::is_same_v<T, REAL>)
                    return std::stod(row.at(colName));
                else if constexpr (std::is_same_v<T, TEXT>)
                    return row.at(colName);
                else if constexpr (std::is_same_v<T, BLOB>)
                    return BLOB(row.at(colName).begin(), row.at(colName).end());
                else
                    static_assert(traits::always_false<T>::value, "Unsupported type");
            } catch (std::out_of_range &e) {
                return T();
            }
        }

        template<typename T = TEXT>
        T operator[](const types::SQLCol<T> &col) const {
            return get<T>(col.name);
        }
    };

    struct SQLResult {
        std::vector<SQLRow> rows;
        auto begin() const { return rows.begin(); }
        auto end() const { return rows.end(); }
    };
}

#endif //SQLPP_RESULT_H

//
// Created by mrspaar on 7/13/23.
//

#ifndef SQLPP_TABLE_H
#define SQLPP_TABLE_H

#include "column.h"

namespace sqlpp::types {
    struct SQLTable {
        std::string name;
        SQLColList columns;
        SQLForeignKeyList foreignKeys;

        explicit SQLTable(std::string name) {
            this->name = std::move(name);
        }

        std::string serialize() const {
            std::string sql = "CREATE TABLE IF NOT EXISTS " + name + " (\n    ";

            for (int i = 0; i < columns.size(); i++) {
                sql += std::visit([](auto &&item) -> std::string { return item->serialize(); }, columns[i]);

                if (i != columns.size() - 1)
                    sql += ",\n    ";
            }

            for (const auto &foreignKey: foreignKeys)
                sql += ",\n    " + std::string(*foreignKey);

            return sql + "\n);";
        }

        template<typename... Cols>
        expr::TableExpr<Cols...> operator()(Cols&&... cols) {
            return expr::TableExpr<Cols...>(name, std::forward<Cols>(cols)...);
        }
    };
}

#endif //SQLPP_TABLE_H

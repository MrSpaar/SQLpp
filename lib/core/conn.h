//
// Created by mrspaar on 9/19/23.
//

#ifndef SQLPP_CONN_H
#define SQLPP_CONN_H

#include <map>
#include <iostream>
#include <sqlite3.h>

#include "table.h"


namespace sqlpp {
    struct SQLRow {
        std::map<std::string, std::string> data;

        template<typename T>
        T get(const std::string &key) {
            if (data.find(key) == data.end())
                throw std::runtime_error("Key not found");

            try {
                if constexpr (std::is_same_v<T, int>)
                    return std::stoi(data[key]);
                else if constexpr (std::is_same_v<T, double>)
                    return std::stod(data[key]);
                else if constexpr (std::is_same_v<T, std::string>)
                    return data[key];

                throw std::runtime_error("Unsupported type");
            } catch (std::exception &e) {
                throw std::runtime_error("Can't convert value");
            }
        }

        template<typename T>
        T operator[](const types::SQLCol<T> &col) {
            return get<T>(col.name);
        }
    };

    struct SQLResult {
        char *errMsg = nullptr;
        std::vector<SQLRow> data;

        [[nodiscard]] auto begin() { return data.begin(); }
        [[nodiscard]] auto end() { return data.end(); }
        [[nodiscard]] bool empty() const { return data.empty(); }
        [[nodiscard]] bool bad() const { return errMsg != nullptr; }
        [[nodiscard]] SQLRow& operator[](int i) { return data[i]; }

        ~SQLResult() { sqlite3_free(errMsg); }
    };

    struct Connection {
        sqlite3 *ptr = nullptr;
        ~Connection() { sqlite3_close(ptr); }

        explicit Connection(const char *path = ":memory:") {
            int rc = sqlite3_open_v2(path, &ptr, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);

            if (rc != SQLITE_OK) {
                std::cerr << "Can't open database: " << sqlite3_errmsg(ptr) << std::endl;
                exit(1);
            }
        }

        static int fetch(void *res, int argc, char **argv, char **colName) {
            if (argc == 0)
                return 0;

            auto *result = (SQLResult*) res;
            SQLRow row;

            for (int i = 0; i < argc; i++)
                row.data[colName[i]] = argv[i] ? argv[i] : "NULL";

            result->data.push_back(row);
            return 0;
        }

        SQLResult exec(std::string *source) const {
            char *errMsg = nullptr;
            SQLResult res;

            (void) sqlite3_exec(
                    ptr, source->c_str(),
                    fetch, &res, &errMsg
            );

            res.errMsg = errMsg;
            return res;
        }
    };
}


#endif //SQLPP_CONN_H

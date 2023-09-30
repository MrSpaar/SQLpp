//
// Created by mrspaar on 9/19/23.
//

#ifndef SQLPP_CONN_H
#define SQLPP_CONN_H

#include <map>
#include <vector>
#include <charconv>
#include <iostream>
#include <sqlite3.h>

#include "table.h"


namespace sqlpp {
    struct SQLValue: std::string {
        template<typename T>
        T as() const {
            T value;
            auto res = std::from_chars(data(), data() + size(), value);

            if (res.ec != std::errc())
                throw std::runtime_error(
                        "Can't convert '" + *this + "' to '" + typeid(T).name() + "' type"
                );

            return value;
        }
    };

    struct SQLRow {
        std::map<std::string, SQLValue> data;

        template<typename T>
        [[nodiscard]] T operator[](const types::SQLCol<T> &col) const {
            return data.at(col).template as<T>();
        }

        [[nodiscard]] SQLValue& operator[](const types::SQLCol<TEXT> &col) {
            return data.at(col);
        }

        [[nodiscard]] SQLValue& operator[](const char *key) {
            return data.at(key);
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

        [[nodiscard]] SQLResult run(const std::string &source) const {
            char *errMsg = nullptr;
            SQLResult res;

            (void) sqlite3_exec(
                    ptr, source.c_str(),
                    fetch, &res, &errMsg
            );

            res.errMsg = errMsg;
            return res;
        }

        static int fetch(void *res, int argc, char **argv, char **colName) {
            if (argc == 0)
                return 0;

            auto *result = (SQLResult*) res;
            SQLRow row;

            for (int i = 0; i < argc; i++)
                row.data[colName[i]] = SQLValue{argv[i]};

            result->data.push_back(row);
            return 0;
        }
    };
}


#endif //SQLPP_CONN_H

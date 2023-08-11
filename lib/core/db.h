//
// Created by mrspaar on 7/29/23.
//

#ifndef SQLPP_DB_H
#define SQLPP_DB_H

#include <memory>
#include "queries/result.h"


namespace sqlpp {
    class Database {
    private:
        sqlite3 *ptr = nullptr;

        static int fetch(void *data, int argc, char **argv, char **azColName) {
            auto *result = (SQLResult *) data;
            if (argc == 0)
                return 0;

            SQLRow row;
            for (int i = 0; i < argc; i++)
                row.row[azColName[i]] = argv[i];

            result->rows.push_back(row);
            return 0;
        }
    public:
        explicit Database(const char *path) {
            int status = sqlite3_open(path, &ptr);

            if (status != SQLITE_OK) {
                std::cerr << "Error opening database: \"" << sqlite3_errmsg(ptr) << '"' << std::endl;
                sqlite3_close(ptr);
                exit(1);
            }
        }

        std::unique_ptr<SQLResult> rawQuery(const std::string &sql) const {
            char *errMsg = nullptr;
            auto result = std::make_unique<SQLResult>();
            sqlite3_exec(ptr, sql.c_str(), Database::fetch, result.get(), &errMsg);

            if (errMsg == nullptr)
                return result;

            std::cerr << "Error running query: \"" << errMsg << '"' << std::endl;
            sqlite3_free(errMsg);
            return result;
        }



        ~Database() {
            sqlite3_close(ptr);
        }
    };
}


#endif //SQLPP_DB_H

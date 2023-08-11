//
// Created by mrspaar on 7/25/23.
//

#ifndef SQLPP_BASE_H
#define SQLPP_BASE_H

#include "core/db.h"


namespace sqlpp::keywords {
    struct From {} from;
    struct LeftJoin {} leftJoin;
    struct InnerJoin {} innerJoin;
    struct CrossJoin {} crossJoin;
    struct On {} on;
    struct Where {} where;
    struct OrderBy {} orderBy;
    struct GroupBy {} groupBy;
    struct Having {} having;
    struct Limit {} limit;
    struct Set {} set;
    struct Abort {} abort;
    struct Fail {} fail;
    struct Ignore {} ignore;
    struct Replace {} replace;
    struct Rollback {} rollback;
    struct Into {} into;
    struct DefaultValues {} defaultValues;
    struct Cout {} cout;
    struct Run{} run;
    struct Fetch{} fetch;
}


namespace sqlpp::types {
    struct Keyword {
        std::string sql;
        explicit Keyword() = default;
        explicit Keyword(std::string sql): sql(std::move(sql)) {}

        explicit Keyword(const std::string &sql, const std::string &op) {
            this->sql = sql;
            removeComma();
            this->sql.append(op);
        }

        template<typename T>
        void operator,(const T&) const {
            static_assert(traits::always_false<T>::value, "Invalid operation");
        }

        void removeComma() {
            if (sql.size() > 1 && this->sql[sql.size() - 2] == ',') {
                this->sql.pop_back();
                this->sql.pop_back();
            }
        }
    };

    struct Runnable: Keyword {
        using Keyword::Keyword;
        using Keyword::operator,;

        void operator,(keywords::Cout) {
            removeComma();
            std::cout << sql << ";" << std::endl;
        }

        std::unique_ptr<SQLResult> operator,(Database &db) {
            removeComma();
            return db.rawQuery(sql);
        }
    };

    template<typename ReturnType, typename Operator>
    struct Intermediate: Keyword {
        using Keyword::Keyword;
        using Keyword::operator,;
        ReturnType operator,(Operator) const { return ReturnType(sql); }
    };
}


#endif //SQLPP_BASE_H

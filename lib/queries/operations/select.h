//
// Created by mrspaar on 7/13/23.
//

#ifndef SQLPP_SELECT_H
#define SQLPP_SELECT_H

#include <utility>
#include "queries/base.h"


namespace sqlpp::keywords::Select {
    struct Offset: types::Runnable {
        using types::Runnable::operator,;
        explicit Offset(const std::string &sql): Runnable(sql, ", ") {}

        Runnable operator,(int x) {
            sql.append(std::to_string(x));
            return Runnable{sql, ""};
        }
    };

    struct Limit: types::Runnable {
        using types::Runnable::operator,;
        explicit Limit(const std::string &sql): Runnable(sql, "\nLIMIT ") {}

        Offset operator,(int x) {
            sql.append(std::to_string(x));
            return Offset(sql);
        }
    };

    struct OrderBy: types::Runnable {
        using types::Runnable::operator,;
        explicit OrderBy(const std::string &sql): Runnable(sql, "\nORDER BY ") {}

        OrderBy& operator,(const expr::OrderExpr &expr) {
            sql.append(expr.sql);
            return *this;
        }

        template<typename T>
        OrderBy& operator,(const types::SQLCol<T> &col) {
            sql.append(col.name).append(", ");
            return *this;
        }

        OrderBy& operator,(const expr::AsExpr &expr) {
            sql.append(expr.sql).append(", ");
            return *this;
        }

        Limit operator,(keywords::Limit) { return Limit(sql); }
    };

    struct Having: types::Runnable {
        using types::Runnable::operator,;
        explicit Having(const std::string &sql): Runnable(sql, "\nHAVING ") {}

        Having& operator,(const expr::ChainedExpr &expr) {
            sql.append(expr.sql);
            return *this;
        }

        OrderBy operator,(keywords::OrderBy) { return OrderBy(sql); }
        Limit operator,(keywords::Limit) { return Limit(sql); }
    };

    struct GroupBy: types::Runnable {
        using types::Runnable::operator,;
        explicit GroupBy(const std::string &sql): Runnable(sql, "\nGROUP BY ") {}

        template<typename T>
        GroupBy& operator,(const types::SQLCol<T> &col) {
            sql.append(col.name).append(", ");
            return *this;
        }

        OrderBy operator,(keywords::OrderBy) { return OrderBy(sql); }
        Having operator,(keywords::Having) { return Having(sql); }
        Limit operator,(keywords::Limit) { return Limit(sql); }
    };

    struct Where: types::Runnable {
        using types::Runnable::operator,;
        explicit Where(const std::string &sql): Runnable(sql, "\nWHERE ") {}

        Where& operator,(const expr::ChainedExpr &expr) {
            sql.append(expr.sql);
            return *this;
        }

        OrderBy operator,(keywords::OrderBy) { return OrderBy(sql); }
        Having operator,(keywords::Having) { return Having(sql); }
        Limit operator,(keywords::Limit) { return Limit(sql); }
    };

    struct Join: types::Runnable {
        using types::Runnable::operator,;
        explicit Join(const std::string &sql): Runnable(sql, "") {}

        Where operator,(keywords::Where) { return Where(sql); }
        GroupBy operator,(keywords::GroupBy) { return GroupBy(sql); }
        OrderBy operator,(keywords::OrderBy) { return OrderBy(sql); }
        Limit operator,(keywords::Limit) { return Limit(sql); }
    };

    struct On: types::Keyword {
        using types::Keyword::operator,;
        explicit On(const std::string &sql): Keyword(sql, "ON ") {}

        Join operator,(const expr::ChainedExpr &expr) {
            sql.append(expr.sql).append(" ");
            return Join(sql);
        }
    };

    struct JoinIntermediate : types::Intermediate<On, keywords::On> {
        using Intermediate<On, keywords::On>::Intermediate;
        using Intermediate<On, keywords::On>::operator,;
    };

    template<typename ReturnType>
    struct JoinTable: types::Keyword {
        using types::Keyword::operator,;

        explicit JoinTable(std::string sql, const std::string &join): Keyword{ std::move(sql) } {
            this->sql.pop_back(); this->sql.pop_back();
            this->sql.append(join);
        }

        ReturnType operator,(const types::SQLTable &table) {
            sql.append(table.name).append(" ");
            return ReturnType(sql);
        }

        ReturnType operator,(const types::Runnable &subquery) {
            sql.append("(").append(subquery.sql).append(") ");
            return ReturnType(sql);
        }
    };

    struct From: types::Runnable {
        using types::Runnable::operator,;
        explicit From(const std::string &sql): Runnable(sql, "\nFROM ") {}

        From& operator,(const types::SQLTable &table) {
            sql.append(table.name).append(", ");
            return *this;
        }

        From& operator,(const Runnable &subquery) {
            sql.append("(").append(subquery.sql).append("), ");
            return *this;
        }

        auto operator,(keywords::LeftJoin) { return JoinTable<JoinIntermediate>(sql, "\nLEFT JOIN "); }
        auto operator,(keywords::InnerJoin) { return JoinTable<JoinIntermediate>(sql, "\nINNER JOIN "); }
        auto operator,(keywords::CrossJoin) { return JoinTable<Join>(sql, "\nCROSS JOIN "); }

        Where operator,(keywords::Where) { return Where(sql); }
        GroupBy operator,(keywords::GroupBy) { return GroupBy(sql); }
        OrderBy operator,(keywords::OrderBy) { return OrderBy(sql); }
        Limit operator,(keywords::Limit) { return Limit(sql); }
    };

    struct Select: types::Keyword {
        using types::Keyword::operator,;
        explicit Select(): Keyword("SELECT ") {}

        template<typename T>
        Select& operator,(const types::SQLCol<T> &col) {
            sql.append(col.name).append(", ");
            return *this;
        }

        Select& operator,(const expr::AsExpr &expr) {
            sql.append(expr.sql).append(", ");
            return *this;
        }
        Select& operator,(const expr::AggregateExpr &expr) {
            sql.append(expr.sql).append(", ");
            return *this;
        }

        From operator,(keywords::From) const { return From(sql); }
    };
}


#endif //SQLPP_SELECT_H

//
// Created by mrspaar on 7/25/23.
//

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsubobject-linkage"

#ifndef SQLPP_BASE_H
#define SQLPP_BASE_H

#include "core/db.h"


namespace sqlpp::keywords {
    struct Cout{} cout;

    template<typename Derived, char const *str>
    struct Keyword {
        std::string *source = nullptr;

        Derived& operator()(std::string *src) {
            source = src;
            removeTrailingComma();

            source->append(str);
            return static_cast<Derived&>(*this);
        }

        template<typename T>
        void operator,(const T&) const {
            static_assert(traits::always_false<T>::value, "Invalid operation");
        }

        void operator,(Cout&) const {
            removeTrailingComma();
            std::cout << *source << ";" << std::endl;
        }

        void removeTrailingComma() const {
            if (source->find(", ", source->size()-2) != std::string::npos)
                source->erase(source->size()-2, 2);
        }
    };


    constexpr char const limitStr[] = " LIMIT ";
    struct Limit: Keyword<Limit, limitStr> {
        using Keyword::operator,;

        Limit& operator,(int x) {
            source->append(std::to_string(x)).append(", ");
            return *this;
        }
    };


    constexpr char const orderByStr[] = " ORDER BY ";
    struct OrderBy: Keyword<OrderBy, orderByStr> {
        using Keyword::operator,;

        OrderBy& operator,(const expr::OrderExpr &expr) {
            source->append(expr.sql).append(", ");
            return *this;
        }

        template<typename T>
        OrderBy& operator,(const types::SQLCol<T> &col) {
            source->append(col.name).append(", ");
            return *this;
        }

        Limit& operator,(Limit&& token) { return token(source); };
    };


    constexpr char const whereStr[] = " WHERE ";
    struct Where: Keyword<Where, whereStr> {
        using Keyword::operator,;

        virtual Where& operator,(const expr::ChainedExpr &expr) {
            source->append(expr.sql);
            return *this;
        }

        OrderBy& operator,(OrderBy&& token) { return token(source); };
        Limit& operator,(Limit&& token) { return token(source); };
    };


    constexpr char const fromStr[] = " FROM ";
    struct From: Keyword<From, fromStr> {
        using Keyword::operator,;

        virtual From& operator,(const types::SQLTable &table) {
            source->append(table.name);
            return *this;
        }

        virtual Where& operator,(Where&& token) { return token(source); };
    };

    constexpr char const abortStr[] = "OR ABORT ";
    constexpr char const failStr[] = "OR FAIL ";
    constexpr char const ignoreStr[] = "OR IGNORE ";
    constexpr char const replaceStr[] = "OR REPLACE ";
    constexpr char const rollbackStr[] = "OR ROLLBACK ";

    template<char const *str>
    struct Or: Keyword<Or<str>, str> {
        using Keyword<Or<str>, str>::operator,;
    };
}


#endif //SQLPP_BASE_H

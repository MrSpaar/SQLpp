//
// Created by mrspaar on 7/27/23.
//

#ifndef SQLPP_INSERT_H
#define SQLPP_INSERT_H

#include "queries/base.h"

namespace sqlpp::keywords::insert {
    /*constexpr char const valuesStr[] = " VALUES (";

    template<typename... ItemTypes>
    struct Values: Keyword<Values<ItemTypes...>, valuesStr> {
        using Keyword<Values<ItemTypes...>, valuesStr>::operator,;

        Values& prepend(std::string *str) {
            this->source->insert(0, str);
            return *this;
        }

        template<typename T>
        void addItem(const T& item) {
            if constexpr (std::is_convertible_v<T, TEXT>)
                this->source->append("'").append(item).append("', ");
            else
                this->source->append(std::to_string(item)).append(", ");
        }

        Values& operator()(ItemTypes... values) {
            (addItem(values), ...);
            this->source->pop_back(); this->source->pop_back();
            this->source->append(")");
            return *this;
        }
    };*/


    constexpr char const defaultStr[] = " DEFAULT VALUES";
    struct DefaultValues: Keyword<DefaultValues, defaultStr> {
        using Keyword::operator,;
    };

    
    constexpr char const intoStr[] = " INTO ";
    struct Into: Keyword<Into, intoStr> {
        using Keyword::operator,;

        template<typename... ColTypes>
        expr::TableExpr<ColTypes...>& operator,(const expr::TableExpr<ColTypes...> &expr) {
            source->append(expr.sql);
            return expr;
        };

        DefaultValues& operator,(DefaultValues&& token) { return token(source); };
    };


    template<char const *str>
    struct InsertOr: Or<str> {
        using Or<str>::operator,;
        Into& operator,(Into&& token) { return token(this->source); };
    };


    constexpr char const insertStr[] = "INSERT ";
    struct Insert: Keyword<Insert, insertStr> {
        using Keyword::operator,;

        std::string sql;
        Insert(): Keyword<Insert, insertStr>() { operator()(&sql); }

        template<char const *str>
        InsertOr<str>& operator,(Or<str>&& token) { return (InsertOr<str>&) token(source); };
        Into& operator,(Into&& token) { return token(source); };
    };
}

#endif //SQLPP_INSERT_H

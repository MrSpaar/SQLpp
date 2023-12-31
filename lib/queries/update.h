//
// Created by mrspaar on 7/25/23.
//

#ifndef SQLPP_UPDATE_H
#define SQLPP_UPDATE_H

#include "base.h"


namespace sqlpp::keywords::update {
    struct Set: Runnable {
        template<typename Item, typename... Items>
        Set& morph(const Item &item, const Items&... items) {
            static_assert(
                    (std::is_same_v<Item, expr::EqExpr> && ... && std::is_same_v<Items, expr::EqExpr>),
                    "All items must be of type EqExpr"
            );

            append(" SET ");
            append(item);
            ((append(", "), append(items)), ...);

            return *this;
        }

        From& from(const types::SQLTable &table) {
            return ((From*) this)->morph(table);
        }

        From& from(const SubQuery &subQuery) {
            return ((From*) this)->morph(subQuery);
        }

        Where& where(const expr::ConditionExpr &expr) {
            return ((Where*) this)->morph(expr);
        }
    };

    struct Or: Keyword {
        Or& morph(const std::string &token) {
            append("OR ").append(token);
            return *this;
        }

        Or& next(const types::SQLTable &table) {
            append(table);
            return *this;
        }

        template<typename... Items>
        Set& set(const Items&... items) {
            return ((Set*) this)->morph(items...);
        }
    };

    struct Update: Keyword {
        Update(): Keyword("UPDATE ") {};
        explicit Update(const types::SQLTable &table): Keyword("UPDATE ") { append(table); }

        Or& or_(const std::string &token) {
            return ((Or*) this)->morph(token);
        }

        template<typename... Exprs>
        Set& set(const Exprs&... exprs) {
            return ((Set*) this)->morph(exprs...);
        }
    };
}


#endif //SQLPP_UPDATE_H

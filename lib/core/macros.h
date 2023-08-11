//
// Created by mrspaar on 7/29/23.
//

#ifndef SQLPP_MACROS_H
#define SQLPP_MACROS_H

#include "queries/operations/select.h"
#include "queries/operations/insert.h"
#include "queries/operations/update.h"
#include "queries/operations/delete.h"


#define COL(colName, type, flags)\
    static sqlpp::types::SQLCol<type> colName(SQL.columns, #colName, flags);
#define FOREIGN_KEY(col1, table, col2)\
    static sqlp::types::SQLForeignKey fk##table##col2(TABLE.foreignKeys, col1.name, table::SQL.name, table::col2.name);

#define USING(ns)               using namespace ns
#define CREATE_TABLE(table, db)  db.rawQuery(table::TABLE.serialize())

#define TABLE(table, cols) namespace table {   \
    static sqlpp::types::SQLTable SQL(#table); \
    cols                                       \
} using namespace table;


#define SEP                               ,
#define QUERY(...)                        __VA_ARGS__
#define COUT                              SEP sqlpp::keywords::cout
#define RUN(db)                           SEP db


#define AND                              &&
#define OR                               ||
#define AS                               |=
#define LIKE                             %=
#define IN(...)                         .in({__VA_ARGS__})
#define BETWEEN(min, max)               .between(min, max)

#define COUNT(x)                        sqlpp::math::aggregateFunc("COUNT", x)
#define SUM(x)                          sqlpp::math::aggregateFunc("SUM", x)
#define AVG(x)                          sqlpp::math::aggregateFunc("AVG", x)
#define MIN(x)                          sqlpp::math::aggregateFunc("MIN", x)
#define MAX(x)                          sqlpp::math::aggregateFunc("MAX", x)
#define COS(x)                          sqlpp::math::numericFunc("COS", x)
#define COSH(x)                         sqlpp::math::numericFunc("COSH", x)
#define ACOS(x)                         sqlpp::math::numericFunc("ACOS", x)
#define ACOSH(x)                        sqlpp::math::numericFunc("ACOSH", x)
#define SIN(x)                          sqlpp::math::numericFunc("SIN", x)
#define SINH(x)                         sqlpp::math::numericFunc("SINH", x)
#define ASIN(x)                         sqlpp::math::numericFunc("ASIN", x)
#define ASINH(x)                        sqlpp::math::numericFunc("ASINH", x)
#define TAN(x)                          sqlpp::math::numericFunc("TAN", x)
#define TANH(x)                         sqlpp::math::numericFunc("TANH", x)
#define ATAN(x)                         sqlpp::math::numericFunc("ATAN", x)
#define ATANH(x)                        sqlpp::math::numericFunc("ATANH", x)
#define DEGREES(x)                      sqlpp::math::numericFunc("DEGREES", x)
#define RADIANS(x)                      sqlpp::math::numericFunc("RADIANS", x)
#define CEIL(x)                         sqlpp::math::numericFunc("CEIL", x)
#define CEILING(x)                      sqlpp::math::numericFunc("CEILING", x)
#define FLOOR(x)                        sqlpp::math::numericFunc("FLOOR", x)
#define TRUNC(x)                        sqlpp::math::numericFunc("TRUNC", x)
#define EXP(x)                          sqlpp::math::numericFunc("EXP", x)
#define SQRT(x)                         sqlpp::math::numericFunc("SQRT", x)
#define LN(x)                           sqlpp::math::numericFunc("LN", x)
#define LOG2(x)                         sqlpp::math::numericFunc("LOG2", x)
#define LOG10(x)                        sqlpp::math::numericFunc("LOG10", x)
#define LOG10_2(x)                      sqlpp::math::numericFunc("LOG", x)
#define LOGB(b, y)                      sqlpp::math::twoArgNumericFunc("LOG", b, y)
#define ATAN2(x, y)                     sqlpp::math::twoArgNumericFunc("ATAN2", x, y)
#define POW(x, y)                       sqlpp::math::twoArgNumericFunc("POW", x, y)
#define POWER(x, y)                     sqlpp::math::twoArgNumericFunc("POWER", x, y)
#define MOD(x, y)                       sqlpp::math::twoArgNumericFunc("MOD", x, y)
#define ROUND(x, y)                     sqlpp::math::twoArgNumericFunc("ROUND", x, y)

#define CHOOSE_LOG(_1, _2, NAME, ...) NAME
#define LOG(...) CHOOSE_LOG(__VA_ARGS__, LOGB, LOG10_2)(__VA_ARGS__)


#define SELECT                          sqlpp::keywords::Select::Select() SEP
#define FROM                            SEP sqlpp::keywords::from SEP
#define WHERE                           SEP sqlpp::keywords::where SEP
#define LEFT_JOIN                       SEP sqlpp::keywords::leftJoin SEP
#define INNER_JOIN                      SEP sqlpp::keywords::innerJoin SEP
#define CROSS_JOIN                      SEP sqlpp::keywords::crossJoin SEP
#define ON                              SEP sqlpp::keywords::on SEP
#define GROUP_BY                        SEP sqlpp::keywords::groupBy SEP
#define ORDER_BY                        SEP sqlpp::keywords::orderBy SEP
#define ASC                             .asc()
#define DESC                            .desc()
#define HAVING                          SEP sqlpp::keywords::having SEP
#define LIMIT                           SEP sqlpp::keywords::limit SEP


#define INSERT                          sqlpp::keywords::Insert::Insert() SEP
#define INSERT_OR                       sqlpp::keywords::Insert::InsertOr() SEP
#define INTO                            sqlpp::keywords::into SEP
#define VALUES                          SEP sqlpp::keywords::Insert::Values
#define DEFAULT_VALUES                  SEP sqlpp::keywords::defaultValues

#define ABORT                          sqlpp::keywords::abort SEP
#define FAIL                           sqlpp::keywords::fail SEP
#define IGNORE                         sqlpp::keywords::ignore SEP
#define REPLACE                        sqlpp::keywords::replace SEP
#define ROLLBACK                       sqlpp::keywords::rollback SEP

#define UPDATE                          sqlpp::keywords::Update::Update() SEP
#define UPDATE_OR                       sqlpp::keywords::Update::UpdateOr() SEP
#define SET                             SEP sqlpp::keywords::set SEP


#define DELETE_FROM                     sqlpp::keywords::Delete::DeleteFrom() SEP


#endif //SQLPP_MACROS_H

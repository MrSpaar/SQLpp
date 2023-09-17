//
// Created by mrspaar on 7/29/23.
//

#ifndef SQLPP_MACROS_H
#define SQLPP_MACROS_H

#include "queries/select.h"
#include "queries/insert.h"
#include "queries/update.h"
#include "queries/delete.h"


#define COL(colName, type)\
    static sqlpp::types::SQLCol<type> colName(#colName);

#define TABLE(table, cols) namespace table##Table {\
    static sqlpp::types::SQLTable table(#table);   \
    cols                                           \
} using namespace table##Table;


#define S(...)                     __VA_ARGS__)
#define AS                         |=
#define LIKE                       %=
#define IN(...)                    .in({__VA_ARGS__})
#define BETWEEN(x, y)              .between(x, y)

#define COUNT(x)                   sqlpp::expr::NumericExpr("COUNT", x)
#define SUM(x)                     sqlpp::expr::NumericExpr("SUM", x)
#define AVG(x)                     sqlpp::expr::NumericExpr("AVG", x)
#define MIN(x)                     sqlpp::expr::NumericExpr("MIN", x)
#define MAX(x)                     sqlpp::expr::NumericExpr("MAX", x)
#define COS(x)                     sqlpp::expr::NumericExpr("COS", x)
#define COSH(x)                    sqlpp::expr::NumericExpr("COSH", x)
#define ACOS(x)                    sqlpp::expr::NumericExpr("ACOS", x)
#define ACOSH(x)                   sqlpp::expr::NumericExpr("ACOSH", x)
#define SIN(x)                     sqlpp::expr::NumericExpr("SIN", x)
#define SINH(x)                    sqlpp::expr::NumericExpr("SINH", x)
#define ASIN(x)                    sqlpp::expr::NumericExpr("ASIN", x)
#define ASINH(x)                   sqlpp::expr::NumericExpr("ASINH", x)
#define TAN(x)                     sqlpp::expr::NumericExpr("TAN", x)
#define TANH(x)                    sqlpp::expr::NumericExpr("TANH", x)
#define ATAN(x)                    sqlpp::expr::NumericExpr("ATAN", x)
#define ATANH(x)                   sqlpp::expr::NumericExpr("ATANH", x)
#define DEGREES(x)                 sqlpp::expr::NumericExpr("DEGREES", x)
#define RADIANS(x)                 sqlpp::expr::NumericExpr("RADIANS", x)
#define CEIL(x)                    sqlpp::expr::NumericExpr("CEIL", x)
#define CEILING(x)                 sqlpp::expr::NumericExpr("CEILING", x)
#define FLOOR(x)                   sqlpp::expr::NumericExpr("FLOOR", x)
#define TRUNC(x)                   sqlpp::expr::NumericExpr("TRUNC", x)
#define EXP(x)                     sqlpp::expr::NumericExpr("EXP", x)
#define SQRT(x)                    sqlpp::expr::NumericExpr("SQRT", x)
#define LN(x)                      sqlpp::expr::NumericExpr("LN", x)
#define LOG2(x)                    sqlpp::expr::NumericExpr("LOG2", x)
#define LOG10(x)                   sqlpp::expr::NumericExpr("LOG10", x)
#define LOG10_2(x)                 sqlpp::expr::NumericExpr("LOG", x)
#define LOGB(b, y)                 sqlpp::expr::NumericExpr("LOG", b, y)
#define ATAN2(x, y)                sqlpp::expr::NumericExpr("ATAN2", x, y)
#define POW(x, y)                  sqlpp::expr::NumericExpr("POW", x, y)
#define POWER(x, y)                sqlpp::expr::NumericExpr("POWER", x, y)
#define MOD(x, y)                  sqlpp::expr::NumericExpr("MOD", x, y)
#define ROUND(x, y)                sqlpp::expr::NumericExpr("ROUND", x, y)

#define CHOOSE_LOG(_1, _2, NAME, ...) NAME
#define LOG(...) CHOOSE_LOG(__VA_ARGS__, LOGB, LOG10_2)(__VA_ARGS__)


#define AND                        ).and_(
#define OR                         ).or_(
#define FROM                       ).from(
#define WHERE                      ).where(
#define LIMIT                      ).limit(
#define OFFSET                     ).offset(
#define ORDER                      ).order(
#define BY                         ).by(
#define ASC                        .asc()
#define DESC                       .desc()

#define SELECT                     sqlpp::keywords::select::Select(
#define SUB_SELECT(...)            sqlpp::keywords::select::Select(__VA_ARGS__)
#define LEFT                       ).joinOp(" LEFT"
#define INNER                      ).joinOp(" INNER"
#define CROSS                      ).joinOp(" CROSS"
#define JOIN                       ).join(
#define ON                         ).on(
#define GROUP                      ).group(
#define HAVING                     ).having(

#define DELETE                     sqlpp::keywords::del::Delete(

#define ABORT                      "ABORT ").next(
#define FAIL                       "FAIL ").next(
#define IGNORE                     "IGNORE ").next(
#define REPLACE                    "REPLACE ").next(
#define ROLLBACK                   "ROLLBACK ").next(

#define UPDATE                     sqlpp::keywords::update::Update(
#define SET                        ).set(

#define INSERT                     sqlpp::keywords::insert::Insert(
#define INTO                       ).into(
#define VALUES(...)                ).values(__VA_ARGS__
#define DEFAULT                    ).default_
#define VALUES_                    (

#define COUT                       ).cout()


#endif //SQLPP_MACROS_H

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

#define COUNT(x)                   sqlpp::expr::NumExpr("COUNT", x)
#define SUM(x)                     sqlpp::expr::NumExpr("SUM", x)
#define AVG(x)                     sqlpp::expr::NumExpr("AVG", x)
#define MIN(x)                     sqlpp::expr::NumExpr("MIN", x)
#define MAX(x)                     sqlpp::expr::NumExpr("MAX", x)
#define COS(x)                     sqlpp::expr::NumExpr("COS", x)
#define COSH(x)                    sqlpp::expr::NumExpr("COSH", x)
#define ACOS(x)                    sqlpp::expr::NumExpr("ACOS", x)
#define ACOSH(x)                   sqlpp::expr::NumExpr("ACOSH", x)
#define SIN(x)                     sqlpp::expr::NumExpr("SIN", x)
#define SINH(x)                    sqlpp::expr::NumExpr("SINH", x)
#define ASIN(x)                    sqlpp::expr::NumExpr("ASIN", x)
#define ASINH(x)                   sqlpp::expr::NumExpr("ASINH", x)
#define TAN(x)                     sqlpp::expr::NumExpr("TAN", x)
#define TANH(x)                    sqlpp::expr::NumExpr("TANH", x)
#define ATAN(x)                    sqlpp::expr::NumExpr("ATAN", x)
#define ATANH(x)                   sqlpp::expr::NumExpr("ATANH", x)
#define DEGREES(x)                 sqlpp::expr::NumExpr("DEGREES", x)
#define RADIANS(x)                 sqlpp::expr::NumExpr("RADIANS", x)
#define CEIL(x)                    sqlpp::expr::NumExpr("CEIL", x)
#define CEILING(x)                 sqlpp::expr::NumExpr("CEILING", x)
#define FLOOR(x)                   sqlpp::expr::NumExpr("FLOOR", x)
#define TRUNC(x)                   sqlpp::expr::NumExpr("TRUNC", x)
#define EXP(x)                     sqlpp::expr::NumExpr("EXP", x)
#define SQRT(x)                    sqlpp::expr::NumExpr("SQRT", x)
#define LN(x)                      sqlpp::expr::NumExpr("LN", x)
#define LOG2(x)                    sqlpp::expr::NumExpr("LOG2", x)
#define LOG10(x)                   sqlpp::expr::NumExpr("LOG10", x)
#define LOG10_2(x)                 sqlpp::expr::NumExpr("LOG", x)
#define LOGB(b, y)                 sqlpp::expr::NumExpr("LOG", b, y)
#define ATAN2(x, y)                sqlpp::expr::NumExpr("ATAN2", x, y)
#define POW(x, y)                  sqlpp::expr::NumExpr("POW", x, y)
#define POWER(x, y)                sqlpp::expr::NumExpr("POWER", x, y)
#define MOD(x, y)                  sqlpp::expr::NumExpr("MOD", x, y)
#define ROUND(x, y)                sqlpp::expr::NumExpr("ROUND", x, y)

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

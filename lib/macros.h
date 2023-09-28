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
    static sqlpp::types::SQLCol<type> colName{#colName};

#define TABLE(table, cols) namespace table##Table {\
    static sqlpp::types::SQLTable table{#table};   \
    cols                                           \
} using namespace table##Table;

#define CHOOSE_2(_1, _2, NAME, ...) NAME
#define CHOOSE_3(_1, _2, _3, NAME, ...) NAME


#define AS                         |=
#define LIKE                       %=
#define IN(...)                    .in({__VA_ARGS__})
#define BETWEEN(x, y)              .between(x, y)

#define TRIM_1(x)                  sqlpp::expr::NumExpr("TRIM", x)
#define TRIM_2(x, y)               sqlpp::expr::NumExpr("TRIM", x, y)
#define TRIM(...)                  CHOOSE_2(__VA_ARGS__, TRIM_2, TRIM_1)(__VA_ARGS__)
#define LTRIM_1(x)                 sqlpp::expr::NumExpr("LTRIM", x)
#define LTRIM_2(x, y)              sqlpp::expr::NumExpr("LTRIM", x, y)
#define LTRIM(...)                 CHOOSE_2(__VA_ARGS__, LTRIM_2, LTRIM_1)(__VA_ARGS__)
#define RTRIM_1(x)                 sqlpp::expr::NumExpr("RTRIM", x)
#define RTRIM_2(x, y)              sqlpp::expr::NumExpr("RTRIM", x, y)
#define RTRIM(...)                 CHOOSE_2(__VA_ARGS__, RTRIM_2, RTRIM_1)(__VA_ARGS__)
#define LOWER(x)                   sqlpp::expr::NumExpr("LOWER", x)
#define UPPER(x)                   sqlpp::expr::NumExpr("UPPER", x)
#define SUBSTR_2(x, y)             sqlpp::expr::NumExpr("SUBSTR", x, y)
#define SUBSTR_3(x, y, z)          sqlpp::expr::NumExpr("SUBSTR", x, y, z)
#define SUBSTR(...)                CHOOSE_3(__VA_ARGS__, SUBSTR_3, SUBSTR_2)(__VA_ARGS__)
#define REPLACE_(x, y, z)          sqlpp::expr::NumExpr("REPLACE", x, y, z)
#define HEX(x)                     sqlpp::expr::NumExpr("HEX", x)
#define CHAR(...)                  sqlpp::expr::NumExpr("CHAR", __VA_ARGS__)
#define COALESCE(X, ...)           sqlpp::expr::NumExpr("COALESCE", X, __VA_ARGS__)
#define SOUNDEX(x)                 sqlpp::expr::NumExpr("SOUNDEX", x)
#define UNHEX_1(x)                 sqlpp::expr::NumExpr("UNHEX", x)
#define UNHEX_2(x, y)              sqlpp::expr::NumExpr("UNHEX", x, y)
#define UNHEX(...)                 CHOOSE_2(__VA_ARGS__, UNHEX_2, UNHEX_1)(__VA_ARGS__)

#define CHANGES()                  sqlpp::expr::NumExpr("CHANGES()")
#define TOTAL_CHANGES()            sqlpp::expr::NumExpr("TOTAL_CHANGES()")
#define TYPEOF(x)                  sqlpp::expr::NumExpr("TYPEOF", x)
#define LAST_INSERT_ROWID()        sqlpp::expr::NumExpr("LAST_INSERT_ROWID()")
#define UNICODE(x)                 sqlpp::expr::NumExpr("UNICODE", x)
#define LENGTH(x)                  sqlpp::expr::NumExpr("LENGTH", x)
#define OCTET_LENGTH(x)            sqlpp::expr::NumExpr("OCTET_LENGTH", x)
#define QUOTE(x)                   sqlpp::expr::NumExpr("QUOTE", x)

#define IFNULL(x, y)               sqlpp::expr::NumExpr("IFNULL", x, y)
#define IIF(x, y, z)               sqlpp::expr::NumExpr("IIF", x, y, z)
#define INSTR(x, y)                sqlpp::expr::NumExpr("INSTR", x, y)
#define NULLIF(x, y)               sqlpp::expr::NumExpr("NULLIF", x, y)

#define COUNT(x)                   sqlpp::expr::NumExpr("COUNT", x)
#define SUM(x)                     sqlpp::expr::NumExpr("SUM", x)
#define AVG(x)                     sqlpp::expr::NumExpr("AVG", x)
#define MIN(x)                     sqlpp::expr::NumExpr("MIN", x)
#define MAX(x)                     sqlpp::expr::NumExpr("MAX", x)
#define ABS(x)                     sqlpp::expr::NumExpr("ABS", x)
#define SIGN(x)                    sqlpp::expr::NumExpr("SIGN", x)
#define RANDOM()                   sqlpp::expr::NumExpr("RANDOM()")
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
#define LOG(...)                   CHOOSE_2(__VA_ARGS__, LOGB, LOG10_2)(__VA_ARGS__)
#define ATAN2(x, y)                sqlpp::expr::NumExpr("ATAN2", x, y)
#define POW(x, y)                  sqlpp::expr::NumExpr("POW", x, y)
#define POWER(x, y)                sqlpp::expr::NumExpr("POWER", x, y)
#define MOD(x, y)                  sqlpp::expr::NumExpr("MOD", x, y)
#define ROUND(x, y)                sqlpp::expr::NumExpr("ROUND", x, y)


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

#define END                        )
#define COUT                       ).cout()
#define RUN(conn)                  ).run(conn)


#endif //SQLPP_MACROS_H

//
// Created by mrspaar on 7/29/23.
//

#ifndef SQLPP_MACROS_H
#define SQLPP_MACROS_H

#include <any>
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


#define TRIM_1(x)                  sqlpp::expr::MathExpr<std::string, std::string>("TRIM", x)
#define TRIM_2(x, y)               sqlpp::expr::MathExpr<std::string, std::string>("TRIM", x, y)
#define LTRIM_1(x)                 sqlpp::expr::MathExpr<std::string, std::string>("LTRIM", x)
#define LTRIM_2(x, y)              sqlpp::expr::MathExpr<std::string, std::string>("LTRIM", x, y)
#define RTRIM_1(x)                 sqlpp::expr::MathExpr<std::string, std::string>("RTRIM", x)
#define RTRIM_2(x, y)              sqlpp::expr::MathExpr<std::string, std::string>("RTRIM", x, y)
#define LOWER(x)                   sqlpp::expr::MathExpr<std::string, std::string>("LOWER", x)
#define UPPER(x)                   sqlpp::expr::MathExpr<std::string, std::string>("UPPER", x)
#define SUBSTR_2(x, y)             sqlpp::expr::MathExpr<std::string, std::string>("SUBSTR", x, y)
#define SUBSTR_3(x, y, z)          sqlpp::expr::MathExpr<std::string, std::string>("SUBSTR", x, y, z)
#define REPLACE_(x, y, z)          sqlpp::expr::MathExpr<std::string, std::string>("REPLACE", x, y, z)
#define CHAR(...)                  sqlpp::expr::MathExpr<std::string, std::string>("CHAR", __VA_ARGS__)
#define SOUNDEX(x)                 sqlpp::expr::MathExpr<std::string, std::string>("SOUNDEX", x)
#define HEX(x)                     sqlpp::expr::MathExpr<std::string, std::string>("HEX", x)
#define UNHEX_1(x)                 sqlpp::expr::MathExpr<std::string, std::string>("UNHEX", x)
#define UNHEX_2(x, y)              sqlpp::expr::MathExpr<std::string, std::string>("UNHEX", x, y)
#define TRIM(...)                  CHOOSE_2(__VA_ARGS__, TRIM_2, TRIM_1)(__VA_ARGS__)
#define LTRIM(...)                 CHOOSE_2(__VA_ARGS__, LTRIM_2, LTRIM_1)(__VA_ARGS__)
#define RTRIM(...)                 CHOOSE_2(__VA_ARGS__, RTRIM_2, RTRIM_1)(__VA_ARGS__)
#define UNHEX(...)                 CHOOSE_2(__VA_ARGS__, UNHEX_2, UNHEX_1)(__VA_ARGS__)
#define SUBSTR(...)                CHOOSE_3(__VA_ARGS__, SUBSTR_3, SUBSTR_2)(__VA_ARGS__)

#define IFNULL(x, y)               sqlpp::expr::MathExpr<std::any, std::any>("IFNULL", x, y)
#define IIF(x, y, z)               sqlpp::expr::MathExpr<std::any, std::any>("IIF", x, y, z)
#define INSTR(x, y)                sqlpp::expr::MathExpr<std::any, std::any>("INSTR", x, y)
#define NULLIF(x, y)               sqlpp::expr::MathExpr<std::any, std::any>("NULLIF", x, y)
#define COALESCE(X, ...)           sqlpp::expr::MathExpr<std::any, std::any>("COALESCE", X, __VA_ARGS__)

#define UNICODE(x)                 sqlpp::expr::MathExpr<std::any, int>("UNICODE", x)
#define COUNT(x)                   sqlpp::expr::MathExpr<std::any, int>("COUNT", x)
#define LENGTH(x)                  sqlpp::expr::MathExpr<std::any, int>("LENGTH", x)
#define OCTET_LENGTH(x)            sqlpp::expr::MathExpr<std::any, int>("OCTET_LENGTH", x)
#define QUOTE(x)                   sqlpp::expr::MathExpr<std::any, std::string>("QUOTE", x)
#define TYPEOF(x)                  sqlpp::expr::MathExpr<std::any, std::string>("TYPEOF", x)

#define SUM(x)                     sqlpp::expr::MathExpr<int, int>("SUM", x)
#define AVG(x)                     sqlpp::expr::MathExpr<int, int>("AVG", x)
#define MIN(x)                     sqlpp::expr::MathExpr<int, int>("MIN", x)
#define MAX(x)                     sqlpp::expr::MathExpr<int, int>("MAX", x)
#define ABS(x)                     sqlpp::expr::MathExpr<int, int>("ABS", x)
#define SIGN(x)                    sqlpp::expr::MathExpr<int, int>("SIGN", x)
#define RANDOM()                   sqlpp::expr::MathExpr<int, int>("RANDOM()")
#define COS(x)                     sqlpp::expr::MathExpr<int, int>("COS", x)
#define COSH(x)                    sqlpp::expr::MathExpr<int, int>("COSH", x)
#define ACOS(x)                    sqlpp::expr::MathExpr<int, int>("ACOS", x)
#define ACOSH(x)                   sqlpp::expr::MathExpr<int, int>("ACOSH", x)
#define SIN(x)                     sqlpp::expr::MathExpr<int, int>("SIN", x)
#define SINH(x)                    sqlpp::expr::MathExpr<int, int>("SINH", x)
#define ASIN(x)                    sqlpp::expr::MathExpr<int, int>("ASIN", x)
#define ASINH(x)                   sqlpp::expr::MathExpr<int, int>("ASINH", x)
#define TAN(x)                     sqlpp::expr::MathExpr<int, int>("TAN", x)
#define TANH(x)                    sqlpp::expr::MathExpr<int, int>("TANH", x)
#define ATAN(x)                    sqlpp::expr::MathExpr<int, int>("ATAN", x)
#define ATANH(x)                   sqlpp::expr::MathExpr<int, int>("ATANH", x)
#define DEGREES(x)                 sqlpp::expr::MathExpr<int, int>("DEGREES", x)
#define RADIANS(x)                 sqlpp::expr::MathExpr<int, int>("RADIANS", x)
#define CEIL(x)                    sqlpp::expr::MathExpr<int, int>("CEIL", x)
#define CEILING(x)                 sqlpp::expr::MathExpr<int, int>("CEILING", x)
#define FLOOR(x)                   sqlpp::expr::MathExpr<int, int>("FLOOR", x)
#define TRUNC(x)                   sqlpp::expr::MathExpr<int, int>("TRUNC", x)
#define EXP(x)                     sqlpp::expr::MathExpr<int, int>("EXP", x)
#define SQRT(x)                    sqlpp::expr::MathExpr<int, int>("SQRT", x)
#define LN(x)                      sqlpp::expr::MathExpr<int, int>("LN", x)
#define LOG2(x)                    sqlpp::expr::MathExpr<int, int>("LOG2", x)
#define LOG10(x)                   sqlpp::expr::MathExpr<int, int>("LOG10", x)
#define LOG10_2(x)                 sqlpp::expr::MathExpr<int, int>("LOG", x)
#define LOGB(b, y)                 sqlpp::expr::MathExpr<int, int>("LOG", b, y)
#define ATAN2(x, y)                sqlpp::expr::MathExpr<int, int>("ATAN2", x, y)
#define POW(x, y)                  sqlpp::expr::MathExpr<int, int>("POW", x, y)
#define POWER(x, y)                sqlpp::expr::MathExpr<int, int>("POWER", x, y)
#define MOD(x, y)                  sqlpp::expr::MathExpr<int, int>("MOD", x, y)
#define ROUND(x, y)                sqlpp::expr::MathExpr<int, int>("ROUND", x, y)
#define LOG(...)                   CHOOSE_2(__VA_ARGS__, LOGB, LOG10_2)(__VA_ARGS__)

#define CHANGES()                  sqlpp::expr::MathExpr<std::any, std::any>("CHANGES()")
#define TOTAL_CHANGES()            sqlpp::expr::MathExpr<std::any, std::any>("TOTAL_CHANGES()")
#define LAST_INSERT_ROWID()        sqlpp::expr::MathExpr<std::any, std::any>("LAST_INSERT_ROWID()")


#define AS                         |=
#define LIKE                       %=
#define IN(...)                    .in(__VA_ARGS__)
#define BETWEEN(x, y)              .between(x, y)
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

#define DELETE                     sqlpp::keywords::delete_::Delete(

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

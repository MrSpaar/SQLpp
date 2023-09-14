//
// Created by mrspaar on 7/13/23.
//

#include "core/macros.h"
using namespace sqlpp;


TABLE(User,
    COL(uid, INTEGER, PRIMARY_KEY | AUTOINCREMENT)
    COL(name, TEXT, NOT_NULL)
    COL(email, TEXT, UNIQUE)
)

TABLE(Test,
    COL(tid, INTEGER, PRIMARY_KEY | AUTOINCREMENT)
    COL(avatar_url, TEXT, NOT_NULL)

    FOREIGN_KEY(tid, User, uid)
)


int main() {
    SELECT COUNT(uid) AS "count", name, uid, SUB(
        SELECT COUNT(tid)
        FROM Test::SQL
        WHERE uid == tid
    ) AS "test_count"
    FROM User::SQL
    LEFT JOIN Test::SQL ON uid == tid
    WHERE name LIKE "John%"
    GROUP BY name, email
    HAVING uid > 10
    ORDER BY uid, name DESC
    LIMIT 10, 20 COUT;

    DELETE FROM User::SQL
    WHERE uid > 5 COUT;

    UPDATE OR IGNORE User::SQL
    SET name = "John", email = "john.doe@mail.com"
    WHERE uid > 5 COUT;

    INSERT INTO User::SQL(uid, name, email)
    DEFAULT VALUES COUT;

    INSERT OR IGNORE INTO User::SQL(name, email)
    VALUES "John", "john.doe@mail.com" COUT;

    return 0;
}

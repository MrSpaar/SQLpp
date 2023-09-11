//
// Created by mrspaar on 7/13/23.
//

#include "core/macros.h"
using namespace sqlpp;


TABLE(User,
    COL(id, INTEGER, PRIMARY_KEY | AUTOINCREMENT)
    COL(name, TEXT, NOT_NULL)
    COL(email, TEXT, UNIQUE)
)


int main() {
    SELECT name, id, email
    FROM User::SQL
    WHERE id > 5 AND name LIKE "John"
    GROUP BY name, email
    HAVING id > 10
    ORDER BY id ASC, name DESC
    LIMIT 10, 20 COUT;

    DELETE FROM User::SQL
    WHERE id > 5 COUT;

    UPDATE OR IGNORE User::SQL
    SET name = "John", email = "john.doe@mail.com"
    WHERE id > 5 COUT;

    INSERT INTO User::SQL(id, name, email)
    DEFAULT_VALUES COUT;

    INSERT OR IGNORE INTO User::SQL(name, email)
    VALUES("John", "john.doe@mail.com") COUT;

    return 0;
}

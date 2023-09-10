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
    SELECT id, name, email FROM User::SQL LEFT_JOIN User::SQL ON name == email COUT;
    DELETE FROM User::SQL WHERE id > 0 ORDER_BY id ASC LIMIT 10 COUT;
    UPDATE OR_IGNORE User::SQL SET name = "John" WHERE id > 0 COUT;

    return 0;
}

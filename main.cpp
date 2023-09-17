//
// Created by mrspaar on 7/13/23.
//

#include "core/macros.h"
using namespace sqlpp;

TABLE(user,
    COL(id, INTEGER)
    COL(name, TEXT)
)

int main() {
    SELECT COUNT(id) AS "count", name FROM user WHERE id > 10 COUT;
    INSERT INTO user (id, name) VALUES(1, "Hello") COUT;

    return 0;
}

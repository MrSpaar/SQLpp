//
// Created by mrspaar on 7/13/23.
//

#include "macros.h"
using namespace sqlpp;

TABLE(user,
    COL(id, INTEGER)
    COL(name, TEXT)
)


int main() {
    Connection conn("test.db");

    SQLResult res = SELECT COUNT(id) AS "count", name FROM user WHERE id*2+1 > 10 EXEC(conn);

    if (res.bad())
        std::cerr << res.errMsg << std::endl;

    UPDATE OR IGNORE user SET id = 1, name = "Hello" WHERE id == 1 COUT;
    INSERT OR IGNORE INTO user(id, name) VALUES(1, "Hello") COUT;

    return 0;
}

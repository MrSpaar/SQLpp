//
// Created by mrspaar on 7/13/23.
//

#include "macros.h"
using namespace sqlpp;

TABLE(user,
    COL(id, INTEGER)
    COL(name, TEXT)
    COL(email, TEXT)
)


int main() {
    Connection conn("../data/test.db");
    SQLResult res = SELECT LOG(id, 2) AS "id", name, email, id+1 AS "next" FROM user EXEC(conn);

    if (res.bad())
        std::cerr << res.errMsg << std::endl;
    else if (res.empty())
        std::cout << "Empty result" << std::endl;
    else
        for (SQLRow &row : res)
            std::cout << row[id] << " " << row["next"].as<int>() << " " << row[name] << " " << row[email] << std::endl;

    return 0;
}

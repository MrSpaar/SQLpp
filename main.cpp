//
// Created by mrspaar on 7/13/23.
//

#include "lib/macros.h"
using namespace sqlpp;

TABLE(user,
    COL(id, INTEGER)
    COL(name, TEXT)
    COL(email, TEXT)
)


int main() {
    Connection conn("../data/test.db");
    SQLResult res = SELECT id, name, email FROM user WHERE id > 0 RUN(conn);

    if (res.bad())
        std::cerr << res.errMsg << std::endl;
    else if (res.empty())
        std::cout << "Empty result" << std::endl;
    else
        for (auto &row: res)
            std::cout << row[id] << " " << row[name] << " " << row[email] << std::endl;

    return 0;
}

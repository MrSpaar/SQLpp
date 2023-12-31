# SQLpp

This library is more of a proof of concept that went a little too far.
SQLpp allows a seamless integration of SQLite queries in C++ code, allowing it to be checked at compile time :
```cpp
#include "core/macros.h"
using namespace sqlpp;

TABLE(users,
    COL(id, INTEGER)
    COL(nickname, TEXT)
    COL(avatar, BLOB)
)

int main() {
    SELECT id, nickname FROM users
    WHERE nickname LIKE "%foo%"
    ORDER BY id DESC LIMIT 20 COUT;
    
    return 0;
}
```

> [!WARNING]
> This library isn't meant to be used in any project, it's just an experiment.

## Coverage

Some syntax differ from standard SQLite due to the use of C++ macros (all uppercase):

| SQLite                | SQLpp                     | Reason                                                    |
|:----------------------|:--------------------------|:----------------------------------------------------------|
| `DEFAULT VALUES`      | `DEFAULT_VALUES`          | `VALUES(..)` is already used and incompatible             |
| `BETWEEN x AND y`     | `BETWEEN(x, y)`           | Can't use `AND` because of operator precedence            |
| `=` and `<>`          | `==` and `!=`             | `<>` is not a valid operator and `=` is used for `UPDATE` |
| `foo AS bar`          | `foo AS "bar"`            | Technically possible if the alias is a variable           |
| `<query or subquery>` | `<query or subquery> END` | Hidden open parenthesis at the end of the query           |
| `REPLACE INTO`        | `INSERT OR REPLACE INTO`  | Too much overhead for an alias                            |

As this is a proof of concept, some features are not implemented yet :
- [ ] [Upsert](https://www.sqlite.org/syntax/upsert-clause.html), [returning](https://www.sqlite.org/syntax/returning-clause.html) and [conflict](https://www.sqlite.org/syntax/conflict-clause.html) clauses
- [ ] `CREATE`[`INDEX`](https://www.sqlite.org/lang_createindex.html)`|`[`TABLE`](https://www.sqlite.org/lang_createtable.html)`|`[`TRIGGER`](https://www.sqlite.org/lang_createtrigger.html)`|`[`VIEW`](https://www.sqlite.org/lang_createview.html) and [`ALTER TABLE`](https://www.sqlite.org/lang_altertable.html)
- [ ] `DROP`[`INDEX`](https://www.sqlite.org/lang_dropindex.html)`|`[`TABLE`](https://www.sqlite.org/lang_droptable.html)`|`[`TRIGGER`](https://www.sqlite.org/lang_droptrigger.html)`|`[`VIEW`](https://www.sqlite.org/lang_dropview.html)
- [ ] [Transactions](https://www.sqlite.org/lang_transaction.html) and [`PRAGMA`](https://www.sqlite.org/pragma.html)
- [ ] [`ANALYZE`](https://www.sqlite.org/lang_analyze.html) and [`EXPLAIN`](https://www.sqlite.org/eqp.html)
- [ ] [`ATTACH`](https://www.sqlite.org/lang_attach.html) and [`DETACH`](https://www.sqlite.org/lang_detach.html)

## Runtime

The library also provides a runtime API, which is a thin wrapper around the SQLite C API:
```cpp
#include "macros.h"
using namespace sqlpp;

TABLE(user,
    COL(id, INTEGER)
    COL(name, TEXT)
    COL(email, TEXT)
)


int main() {
    Connection conn("../data/test.db");
    SQLResult res = SELECT id, name, email FROM user RUN(conn);

    if (res.bad()) {
        std::cerr << res.errMsg << std::endl;
        return 1;
    } else if (res.empty()) {
        std::cout << "Empty result" << std::endl;
        return 0;
    }
    
    // row[id] returns an int
    // for aliases, use row["alias"].as<T>()
    
    for (SQLRow &row : res)
        std::cout << row[id] << " " << row[name] << " " << row[email] << std::endl;

    return 0;
}
```

There is also the possibility to store queries in a variable and execute them later :
```cpp
SQLQuery query = SELECT id, name, email FROM user END;
SQLResult res = conn.run(query.sql);
```

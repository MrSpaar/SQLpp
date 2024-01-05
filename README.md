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
    ORDER BY id DESC LIMIT 20
    COUT;
    
    INSERT INTO users (nickname, avatar)
    VALUES ("bar", BLOB("baz")) COUT;
    
    UPDATE OR IGNORE users
    SET nickname = "bar"
    WHERE SQRT(id)+20 == 42 COUT;
    
    DELETE FROM users
    WHERE id == 42 COUT;
    
    return 0;
}
```

## Coverage

Some syntax differ from standard SQLite due to the use of C++ macros :

| SQLite                | SQLpp                     | Reason                                                    |
|:----------------------|:--------------------------|:----------------------------------------------------------|
| `DEFAULT VALUES`      | `DEFAULT VALUES_`         | `VALUES` is used for `INSERT`                             |
| `BETWEEN x AND y`     | `BETWEEN(x, y)`           | `AND` is used to chain conditions                         |
| `=` and `<>`          | `==` and `!=`             | `<>` is not a valid operator and `=` is used for `UPDATE` |
| `foo AS bar`          | `foo AS "bar"`            | Technically possible if the alias is a variable           |
| `<query or subquery>` | `<query or subquery> END` | Hidden open parenthesis at the end of the query           |
| `aNy_KeyWOrD`         | `ANY_KEYWORD`             | C++ macros are case sensitive                             |
| `REPLACE INTO`        | `INSERT OR REPLACE INTO`  | Too much overhead for an alias                            |

As this is a proof of concept, some features are not implemented yet :
- [ ] [Upsert](https://www.sqlite.org/syntax/upsert-clause.html), [returning](https://www.sqlite.org/syntax/returning-clause.html) and [conflict](https://www.sqlite.org/syntax/conflict-clause.html) clauses
- [ ] `CREATE`[`INDEX`](https://www.sqlite.org/lang_createindex.html)`|`[`TABLE`](https://www.sqlite.org/lang_createtable.html)`|`[`TRIGGER`](https://www.sqlite.org/lang_createtrigger.html)`|`[`VIEW`](https://www.sqlite.org/lang_createview.html) and [`ALTER TABLE`](https://www.sqlite.org/lang_altertable.html)
- [ ] `DROP`[`INDEX`](https://www.sqlite.org/lang_dropindex.html)`|`[`TABLE`](https://www.sqlite.org/lang_droptable.html)`|`[`TRIGGER`](https://www.sqlite.org/lang_droptrigger.html)`|`[`VIEW`](https://www.sqlite.org/lang_dropview.html)
- [ ] [Transactions](https://www.sqlite.org/lang_transaction.html) and [`PRAGMA`](https://www.sqlite.org/pragma.html)
- [ ] [`ANALYZE`](https://www.sqlite.org/lang_analyze.html) and [`EXPLAIN`](https://www.sqlite.org/eqp.html)
- [ ] [`ATTACH`](https://www.sqlite.org/lang_attach.html) and [`DETACH`](https://www.sqlite.org/lang_detach.html)

## Runtime

The library also provides a runtime API, which is a thin wrapper around the SQLite C API.
It is not checked at compile time and is not type safe.

Here is an example of how to use it :
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

## Installation

To install the library, you can use the following commands :
```bash
git clone http://github.com/MrSpaar/SQLpp

mkdir build && cd build
cmake ..
sudo make install
```

Then, because the library is header-only, you can directly include it in your project :
```cpp
#include <sqlpp/macros.h>
```

> [!WARNING]
> The library is a wrapper around the SQLite3 C API, so you need to link it to your project.

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

As this is a proof of concept, the library is incomplete and partially untested :

| Feature                                                                                                              | Supported   | Notes                                              |
|----------------------------------------------------------------------------------------------------------------------|-------------|----------------------------------------------------|
| [SELECT](https://www.sqlite.org/lang_select.html)                                                                    | ✔️          | `ALL` and `DISTINCT` not yet implemented           |
| [INSERT](https://www.sqlite.org/lang_insert.html)                                                                    | ✔️          | `INSERT INTO table SELECT ...` not yet implemented |
| [UPDATE](https://www.sqlite.org/lang_update.html)                                                                    | ✔️          | `(foo, bar, baz) = ...` not yet implemented        |
| [DELETE](https://www.sqlite.org/lang_delete.html)                                                                    | ✔️          | Full support                                       |
| [Math](https://www.sqlite.org/lang_mathfunc.html)                                                                    | ✔️          | Full support                                       |
| [Core functions](https://www.sqlite.org/lang_corefunc.html)                                                          | ✔️          | Full support                                       |
| [Date/Time](https://www.sqlite.org/lang_datefunc.html)                                                               | ✔️          | Full support                                       |
| [WITH](https://www.sqlite.org/lang_with.html) and [Windows](https://www.sqlite.org/windowfunctions.html)             | ❌         | Not planned                                        |
| [Views](https://www.sqlite.org/lang_createview.html) and [Triggers](https://www.sqlite.org/lang_createtrigger.html)  | ❌         | Not planned                                        |

Some syntax differ from standard SQLite due to the use of C++ macros :

| SQLite             | SQLpp             | Reason                                                    |
|--------------------|-------------------|-----------------------------------------------------------|
| `DEFAULT VALUES`   | `DEFAULT VALUES_` | `VALUES` is used for `INSERT`                             |
| `BETWEEN x AND y`  | `BETWEEN(x, y)`   | `AND` is used to chain conditions                         |
| `=` and `<>`       | `==` and `!=`     | `<>` is not a valid operator and `=` is used for `UPDATE` |
| `foo AS bar`       | `foo AS "bar"`    | Technically possible if the alias is a variable           |
| `(... END)`        | `(...)`           | `END` is used to end subqueries                           |
| `aNy_KeyWOrD`      | `ANY_KEYWORD`     | C++ macros are case sensitive                             |

I might add an equivalent for `*` (eg. `SELECT * FROM ...`) in the future.

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

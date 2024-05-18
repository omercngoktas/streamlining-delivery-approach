#include <iostream>
#include <mysql/mysql.h>

int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    const char *server = "localhost";
    const char *user = "omercngoktas";
    const char *password = "mwanamboka"; // Set these values to your MySQL configuration
    const char *database = "mydatabase";

    conn = mysql_init(NULL);

    // Connect to database
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        std::cerr << "MySQL connection failed: " << mysql_error(conn) << std::endl;
        return 1;
    }

    // Send SQL query
    if (mysql_query(conn, "SELECT VERSION()")) {
        std::cerr << "MySQL query failed: " << mysql_error(conn) << std::endl;
        return 1;
    }

    res = mysql_use_result(conn);

    // Output table name
    std::cout << "MySQL Version: ";
    while ((row = mysql_fetch_row(res)) != NULL)
        std::cout << row[0] << std::endl;

    // Close connection
    mysql_free_result(res);
    mysql_close(conn);

    return 0;
}

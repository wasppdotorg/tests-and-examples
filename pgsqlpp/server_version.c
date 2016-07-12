#include <stdio.h>
#include <stdlib.h>
#include <postgresql/libpq-fe.h>

void do_exit(PGconn *conn) {
    
    PQfinish(conn);
    exit(1);
}

int main() {
    
    PGconn *conn = PQconnectdb("host=localhost user=test1 password=test1 dbname=test1");

    if (PQstatus(conn) == CONNECTION_BAD) {
        
        fprintf(stderr, "Connection to database failed: %s\n",
            PQerrorMessage(conn));
        do_exit(conn);
    }

    int ver = PQserverVersion(conn);

    printf("Server version: %d\n", ver);
    
    PQfinish(conn);

    return 0;
}

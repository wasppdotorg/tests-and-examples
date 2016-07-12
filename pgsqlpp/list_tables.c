#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>

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
    
    PGresult *res = PQexec(conn, "SELECT table_name FROM information_schema.tables "
                    "WHERE table_schema = 'public'");    
    
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {

        printf("No data retrieved\n");        
        PQclear(res);
        do_exit(conn);
    }       
    
    int rows = PQntuples(res);
    
    for(int i=0; i<rows; i++) {
        
        printf("%s\n", PQgetvalue(res, i, 0));
    }        
    
    PQclear(res);
    PQfinish(conn);

    return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"


/*
 * example 4 with a queries build on-the-fly, the good way
 */

int main(int argc, char *argv[]) {
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLHSTMT stmt2;
    SQLRETURN ret; /* ODBC API return status */
    SQLINTEGER rental_id;


  if(argc < 1){
    fprintf(stderr, "Error en los parametros de entrada:\n\n");
  }

   rental_id = atoi(argv[1]);

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt2);


    SQLPrepare(stmt, (SQLCHAR*) "DELETE FROM payment \
    WHERE rental_id = ?", SQL_NTS);

    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &rental_id, 0, NULL);
    
    ret = SQLExecute(stmt);
    if (ret < 0) {
        odbc_extract_error("Error", stmt, SQL_HANDLE_STMT);
        printf ("Error eliminando los pagos");
        return EXIT_FAILURE;
    }



    SQLPrepare(stmt2, (SQLCHAR*) "DELETE FROM rental \
    WHERE rental_id = ?", SQL_NTS);
    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &rental_id, 0, NULL);

    ret = SQLExecute(stmt2);
    if (ret < 0) {
        odbc_extract_error("Error", stmt2, SQL_HANDLE_STMT);
        printf("Error eliminando datos en rental\n");
        return EXIT_FAILURE;
    }





    SQLCloseCursor(stmt);
    SQLCloseCursor(stmt2);

    printf("\n");
    printf("Eliminado correctamente\n");



    /* free up statement handle */
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt2);

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
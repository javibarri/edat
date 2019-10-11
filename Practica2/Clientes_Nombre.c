#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"


/*
 * example 4 with a queries build on-the-fly, the good way
 */

int main(void) {
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret; /* ODBC API return status */
    SQLCHAR nombre[512];
    SQLCHAR customer_id[512];
    SQLCHAR first_name[512];
    SQLCHAR last_name[512];
    SQLCHAR create_date[512];
    SQLCHAR address_id[512];

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    SQLPrepare(stmt, (SQLCHAR*) "select customer.customer_id, customer.first_name, customer.last_name, customer.create_date, address.address from customer, address where address.address_id = customer.address_id and customer.first_name = ? ", SQL_NTS);

    printf("Nombre del cliente  = ");
    fflush(stdout);
    while (scanf("%s", nombre) != EOF) {
        SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_PARAM_INPUT, SQL_C_CHAR, 0, 0, nombre, 0, NULL);
        
        SQLExecute(stmt);
        
        SQLBindCol(stmt, 1, SQL_C_CHAR, customer_id, sizeof(customer_id), NULL);
        SQLBindCol(stmt, 2, SQL_C_CHAR, first_name, sizeof(first_name), NULL);
        SQLBindCol(stmt, 3, SQL_C_CHAR, last_name, sizeof(last_name), NULL);
        SQLBindCol(stmt, 4, SQL_C_CHAR, create_date, sizeof(create_date), NULL);
        SQLBindCol(stmt, 5, SQL_C_CHAR, address_id, sizeof(address_id), NULL);
    

        /* Loop through the rows in the result-set */
        while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
            printf("Id del cliente = %s\n", customer_id);
            printf("Nombre = %s\n", first_name);
            printf("Apellido = %s\n", last_name);
            printf("Fecha de registro = %s\n", create_date);
            printf("Direccion completa = %s\n", address_id);
        }

        SQLCloseCursor(stmt);

        printf("Nombre del cliente = ");
        fflush(stdout);
    }
    printf("\n");

    /* free up statement handle */
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


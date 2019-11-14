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
    SQLHSTMT stmt3;
    SQLRETURN ret; /* ODBC API return status */
    SQLINTEGER film_id;
    SQLINTEGER inventory_id;
    SQLINTEGER customer_id;
    SQLINTEGER staff_id;
    SQLINTEGER rental_id;
    SQLINTEGER store_id;
    SQLINTEGER amount;
    time_t t;
    struct tm *tm;
    char fechayhora[100];


  if(argc < 5){
    fprintf(stderr, "Error en los parametros de entrada:\n\n");
  }
  t = time(NULL);
  tm = localtime(&t);
  sprintf(fechayhora, "%d-%d-%d", tm->tm_year+1900, tm->tm_mon, tm->tm_mday);
  printf ("Hoy es %s\n", fechayhora);
   customer_id = atoi(argv[1]);
   staff_id = atoi(argv[3]);
   store_id = atoi(argv[4]);
   amount = atoi(argv[5]);
   film_id = atoi(argv[2]);

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt2);
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt3);


    SQLPrepare(stmt, (SQLCHAR*) "select inventory_id \
    from inventory \
    where film_id = ? and store_id = ? ", SQL_NTS);

    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &film_id, 0, NULL);
    SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &store_id, 0, NULL);
    
    ret = SQLExecute(stmt);
    if (ret < 0) {
        odbc_extract_error("Error", stmt, SQL_HANDLE_STMT);
        printf ("Error seleccionando el inventory");
    }

    SQLBindCol(stmt, 1, SQL_C_SLONG, &inventory_id, sizeof(inventory_id), NULL);
    ret = SQLFetch(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        printf ("No hay pelicula para insertar");
        return EXIT_FAILURE;
    }



    rental_id =SQLPrepare(stmt2, (SQLCHAR*) "INSERT INTO rental (customer_id, rental_date, inventory_id, staff_id) \
    VALUES \
    (?, ?, ?, ?)\
    returning rental_id; ", SQL_NTS);
    SQLBindParameter(stmt2, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &customer_id, 0, NULL); 
    SQLBindParameter(stmt2, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, sizeof(fechayhora), 0, fechayhora, 0, NULL);
    SQLBindParameter(stmt2, 3, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &inventory_id, 0, NULL);
    SQLBindParameter(stmt2, 4, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &staff_id, 0, NULL);
    ret = SQLExecute(stmt2);
    if (ret < 0) {
        odbc_extract_error("Error", stmt2, SQL_HANDLE_STMT);
        printf("Error metiendo en rental\n");
    }

    SQLBindCol(stmt2, 1, SQL_C_SLONG, &rental_id, sizeof(inventory_id), NULL);
    ret = SQLFetch(stmt2);
    if (!SQL_SUCCEEDED(ret)) {
        printf ("No ha sido posible insertar\n");
        return EXIT_FAILURE;
    }


    SQLPrepare(stmt, (SQLCHAR*) "INSERT INTO payment(rental_id, amount) \
    VALUES\
    (?, ?) ", SQL_NTS);
    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &rental_id, 0, NULL);
    SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &amount, 0, NULL);
    ret = SQLExecute(stmt3);
    if (ret < 0) {
        odbc_extract_error("Error", stmt3, SQL_HANDLE_STMT);
        printf("Error metiendo en payment");
    }




    SQLCloseCursor(stmt);
    SQLCloseCursor(stmt2);
    SQLCloseCursor(stmt3);

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
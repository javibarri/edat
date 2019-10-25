#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
    SQLRETURN ret; /* ODBC API return status */
    SQLCHAR nombre[512];
    SQLCHAR customer_id[512];
    SQLCHAR first_name[512];
    SQLCHAR last_name[512];
    SQLCHAR create_date[512];
    SQLCHAR address_id[512];
    SQLCHAR n[512];
    SQLCHAR a[512];
    int i;


    if (argc != 5) {
    fprintf(stderr, "Error en los parametros de entrada:\n\n");
    fprintf(stderr, "%s -n <First_name> -a <Last_name>\n", argv[0]);
    fprintf(stderr, "Donde:\n");
    fprintf(stderr, "-n: nombre del cliente\n");
    fprintf(stderr, "-a: apellido del cliente\n");
    exit(-1);
  }

  printf("Practica numero 2, apartado 1\n");
  printf("Realizada por: Javier Barriuso y Miguel Irastorza\n");
  printf("Grupo: 1261\n");

  /* comprueba la linea de comandos */
  for(i = 1; i < argc ; i++) {
    if (strcmp(argv[i], "-n") == 0) {
      strcpy(n, argv[++i]);
    } else if (strcmp(argv[i], "-a") == 0) {
      strcpy(a, argv[++i]);
    } else {
      fprintf(stderr, "Parametro %s es incorrecto\n", argv[i]);
    }
  }

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    SQLPrepare(stmt, (SQLCHAR*) "select customer.customer_id, customer.first_name, customer.last_name, customer.create_date, address.address from customer, address where address.address_id = customer.address_id and (customer.first_name = ? or customer.last_name = ?) ", SQL_NTS);

        SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_PARAM_INPUT, SQL_C_CHAR, 0, 0, n, 0, NULL);

        SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_PARAM_INPUT, SQL_C_CHAR, 0, 0, a, 0, NULL);
        
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


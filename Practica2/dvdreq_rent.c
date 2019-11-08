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
    SQLCHAR fecha[512];
    SQLINTEGER alquiler_id;
    SQLINTEGER film_id;
    SQLINTEGER empleado_id;
    SQLINTEGER client_id;
    SQLINTEGER cliente_id;
    SQLINTEGER tienda_id;
    SQLDOUBLE precio;
    SQLCHAR nombre[512];
    SQLCHAR title[512];
    SQLCHAR fechapequena[512];
    SQLCHAR fechamayor[512];


  if(argc < 3){
    fprintf(stderr, "Error en los parametros de entrada:\n\n");
  }

  client_id = atoi(argv[1]);
  strcpy((char*)fechapequena, argv[2]);
  strcpy((char*)fechamayor, argv[3]);

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    /* SQLPrepare(stmt, (SQLCHAR*) "select f.film_id, f.title, f.release_year, f.length, l.name, f.description, a.first_name, a.last_name \
    from film f, language l, actor a, film_actor fa \
    where f.language_id = l.language_id and f.film_id = fa.film_id and fa.actor_id = a.actor_id and f.title = ? ", SQL_NTS);
*/

    SQLPrepare(stmt, (SQLCHAR*) "select r.rental_id, r.rental_date, r.inventory_id, f.title, r.staff_id, s.first_name, sto.store_id, p.amount, c.customer_id    \
from rental r, inventory i, film f, staff s, store sto, payment p, customer c \
where c.customer_id = ? and c.customer_id = r.customer_id and r.inventory_id = i.inventory_id and i.film_id = f.film_id and r.staff_id = s.staff_id and i.store_id = sto.store_id and \
r.rental_id = p.rental_id \
and r.rental_date > ? and r.rental_date < ? \
order by r.rental_date", SQL_NTS);

        SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &client_id, 0, NULL);
        SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_PARAM_INPUT, SQL_C_CHAR, 0, 0, fechapequena, 0, NULL);
        SQLBindParameter(stmt, 3, SQL_PARAM_INPUT, SQL_PARAM_INPUT, SQL_C_CHAR, 0, 0, fechamayor, 0, NULL);

        ret = SQLExecute(stmt);
        if (ret < 0) {
            odbc_extract_error("Error", stmt, SQL_HANDLE_STMT);
        }
 
        SQLBindCol(stmt, 1, SQL_C_SLONG, &alquiler_id, sizeof(alquiler_id), NULL);
        SQLBindCol(stmt, 2, SQL_C_CHAR, fecha, sizeof(fecha), NULL);
        SQLBindCol(stmt, 3, SQL_C_SLONG, &film_id, sizeof(film_id), NULL);
        SQLBindCol(stmt, 4, SQL_C_CHAR, title, sizeof(title), NULL);
        SQLBindCol(stmt, 5, SQL_C_SLONG, &empleado_id, sizeof(empleado_id), NULL);
        SQLBindCol(stmt, 6, SQL_C_CHAR, nombre, sizeof(nombre), NULL);
        SQLBindCol(stmt, 7, SQL_C_SLONG, &tienda_id, sizeof(tienda_id), NULL);
        SQLBindCol(stmt, 8, SQL_C_DOUBLE, &precio, sizeof(precio), NULL);
        SQLBindCol(stmt, 9, SQL_C_SLONG, &cliente_id, sizeof(cliente_id), NULL);
    
        /* Loop through the rows in the result-set */
        while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
            printf("Id del alquiler = %d\n", alquiler_id);
            printf("Fecha del alquiler = %s\n", fecha);
            printf("Id de la pelicula = %d\n", film_id);
            printf("Titulo de la película = %s\n", title);
            printf("Id del empleado = %d\n", empleado_id);
            printf("Id de la tienda = %d\n", tienda_id);
            printf("Precio pagado por el alquiler = %.2f\n", precio);
            printf("Id del cliente = %d\n", cliente_id);
            printf("\n");
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

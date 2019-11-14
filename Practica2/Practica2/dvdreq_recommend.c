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
    SQLINTEGER client_id;
    SQLCHAR title1[512];


  if(argc < 1){
    fprintf(stderr, "Error en los parametros de entrada:\n\n");
  }

  client_id = atoi(argv[1]);

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);


    SQLPrepare(stmt, (SQLCHAR*) "select title \
from inventory, film, \
	(select inventory_id, count(*) as num \
		from \
			(select inventory_id \
			from \
				(select r.inventory_id, customer_id \
					from rental r, inventory i,film_category f, \
					(select s1.category_id as categoryfav \
					       from \
							(select f.category_id, count (r.inventory_id) as nums \
							 from rental r, film_category f, Inventory i \
							 where r.inventory_id = i.inventory_id and i.film_id = f.film_id and r.customer_id = ? \
							 group by f.category_id) as s1, \
							(select f.category_id, count (r.inventory_id) as nums \
							 from rental r, film_category f, Inventory i \
							 where r.inventory_id = i.inventory_id and i.film_id = f.film_id and r.customer_id = ? \
							 group by f.category_id \
							 order by nums desc limit 1) as s2 \
						where s1.nums = s2.nums) as s3 \
					where 	r.inventory_id = i.inventory_id and i.film_id = f.film_id and \
						f.category_id = s3.categoryfav) as s4 \
				where s4.customer_id <> ?) as s5 \
		group by inventory_id \
		order by num desc \
		limit 3) as s6	\
where s6.inventory_id = inventory.inventory_id and inventory.film_id = film.film_id", SQL_NTS);

        SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &client_id, 0, NULL);
        SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &client_id, 0, NULL);
        SQLBindParameter(stmt, 3, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &client_id, 0, NULL);


        ret = SQLExecute(stmt);
        if (ret < 0) {
            odbc_extract_error("Error", stmt, SQL_HANDLE_STMT);
        }
 

        SQLBindCol(stmt, 1, SQL_C_CHAR, title1, sizeof(title1), NULL);


        /* Loop through the rows in the result-set */
        while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
            printf("Película recomendada = %s\n", title1);
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


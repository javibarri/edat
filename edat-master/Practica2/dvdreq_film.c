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
    SQLINTEGER film_id;
    SQLCHAR titulo[512];
    SQLINTEGER ano;
    SQLSMALLINT longitud;
    SQLCHAR idioma[512];
    SQLCHAR descripcion[512];
    SQLCHAR actor[512];
    SQLCHAR title[512];
    char *err[512];
    int i=0;


  if(argc < 1){
    fprintf(stderr, "Error en los parametros de entrada:\n\n");
  }

  strcpy((char *)title,argv[1]);

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

    SQLPrepare(stmt, (SQLCHAR*) "select f.film_id, f.title, f.release_year, f.length, l.name, f.description, a.first_name, a.last_name \
    from film f, language l, actor a, film_actor fa \
    where f.language_id = l.language_id and f.film_id = fa.film_id and fa.actor_id = a.actor_id and f.title = ? ", SQL_NTS);

        SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_PARAM_INPUT, SQL_C_CHAR, 0, 0, title, 0, NULL);
        
        ret = SQLExecute(stmt);
        if (ret < 0) {
            odbc_extract_error("Error", stmt, SQL_HANDLE_STMT);
        }
 
        SQLBindCol(stmt, 1, SQL_C_SLONG, &film_id, sizeof(film_id), NULL);
        SQLBindCol(stmt, 2, SQL_C_CHAR, titulo, sizeof(titulo), NULL);
        SQLBindCol(stmt, 3, SQL_C_SLONG, &ano, sizeof(ano), NULL);
        SQLBindCol(stmt, 4, SQL_C_SHORT, &longitud, sizeof(longitud), NULL);
        SQLBindCol(stmt, 5, SQL_C_CHAR, idioma, sizeof(idioma), NULL);
        SQLBindCol(stmt, 6, SQL_C_CHAR, descripcion, sizeof(descripcion), NULL);
        SQLBindCol(stmt, 7, SQL_C_CHAR, actor, sizeof(actor), NULL);
    
        /* Loop through the rows in the result-set */
        while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
            if(i==0){
            printf("Id de la pelicula = %d\n", film_id);
            printf("titulo = %s\n", titulo);
            printf("Año = %d\n", ano);
            printf("Longitud = %d\n", longitud);
            printf("Idioma = %s\n", idioma);
            printf("Descripcion = %s\n", descripcion);
            printf("Actor = %s\n", actor);
            i++;
            }
            printf("Actor = %s\n", actor);
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


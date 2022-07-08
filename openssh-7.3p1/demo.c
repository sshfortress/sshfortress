/*
 *
 * gcc c_demo.c -o demo -lsqlcipher
 *
 * */

#include "sqlite3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR(X) 
SQLITE_API int sqlite3_key(
		  sqlite3 *db,                   /* Database to be rekeyed */
		    const void *pKey, int nKey     /* The key */
		);


static int callback(void *NotUsed, int argc, char **argv, char **col_name)
{  
	int i;  
	for(i=0; i<argc; i++)
	{  
		printf("%s\n", argv[i] ? argv[i] : "NULL");  
	}  
	printf("\n");  
	return 0;  
} 


int main(int argc, char *argv[])
{
	sqlite3 *db;
	const char *file= "test.db";
	const char *key = "03000200-0400-0500-0006-000700080009";
	char *s;
	if (sqlite3_open(file, &db) == SQLITE_OK) 
	{
		int  rc;

		if(db == NULL) 
		{
			ERROR(("sqlite3_open reported OK, but db is null, retrying open %s\n", sqlite3_errmsg(db)))
		}
		if(sqlite3_key(db, key, strlen(key)) != SQLITE_OK) 
		{
			ERROR(("error setting key %s\n", sqlite3_errmsg(db)))
				exit(-1);
		}
		char* sql1 = "CREATE TABLE t(id INTEGER NOT NULL, license text NOT NULL, state text NOT NULL, PRIMARY KEY (id));";  
		char* sql2 = "INSERT INTO t(license, state) VALUES ('twPwrK5eAezrmePeturcqK1vvtfoEKzhuxPSqK4WvKjsve16utbnEu56wMfsmwXouLvwq1uWEe1LBG', 'on');";
		char* sql3 = "select * from t where id='1';";  
		char* err_msg = NULL;
		rc = sqlite3_exec(db, sql1, callback, 0, &err_msg);  
		if( rc!=SQLITE_OK )
		{  

			fprintf(stderr, "SQL error: %s\n", err_msg);  
			sqlite3_free(err_msg);  
			err_msg = NULL;
		} 
		rc = sqlite3_exec(db, sql2, callback, 0, &err_msg);  
		if( rc!=SQLITE_OK )
		{  
			fprintf(stderr, "SQL error: %s\n", err_msg);  
			sqlite3_free(err_msg);  
			err_msg = NULL;
		}  

		int nrow,ncolumn;
		char ** db_result;
		rc = sqlite3_get_table(db,sql3,&db_result,&nrow,&ncolumn,&s);
		if(rc != SQLITE_OK)
		{
			printf("select error\n");
			sqlite3_close(db);
			return 1;
		}
				printf("%s  ",db_result[0]);
				printf("%s  ",db_result[1]);
				printf("%s  ",db_result[2]);
				printf("%s  ",db_result[3]);
				printf("%s  ",db_result[4]);
				printf("%s  ",db_result[5]);
				/*
		int i,j;
		for(i=0;i<(nrow+1)*ncolumn;i+=ncolumn)
		{
			for(j=0;j<ncolumn;j++)
			{
				printf("%s  ",db_result[i+j]);
			}
			printf("\n");
		}
		*/
		sqlite3_close(db);  

	}
}


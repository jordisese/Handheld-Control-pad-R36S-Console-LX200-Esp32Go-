#include "query.h"
sqlite3 *db;
sqlite3_stmt *stmt,*stmt1,*stmt2,*stmt3;

void exit_with_error(sqlite3 *db, const char * msg)
{
    fprintf(stderr, "%s: %s\n", msg, sqlite3_errmsg(db));
    sqlite3_close(db);
    exit(1);
}

int squery(char* param,char* out,struct ln_equ_posn *pos,char table)

{
    int result=0;
    char temp[60]="%";
    printf("%s\n",temp);
    switch (table)
    {
    case ('s'):
    {
        param[0]=toupper(param[0]) ;
        strcpy(temp,param);
       // strcat(temp,param);
        strcat(temp,"%");
        printf("%s\n",temp);
        sqlite3_bind_text(stmt2, 1,temp, -1, SQLITE_STATIC);

        while(SQLITE_ROW == sqlite3_step(stmt2))
        {
            result=1;
            sprintf(out,"%s,%f,%f", sqlite3_column_text(stmt2, 2),sqlite3_column_double(stmt2, 0),sqlite3_column_double(stmt2,1));
            printf("%s\n",out);
            pos->ra=sqlite3_column_double(stmt2, 0)*15.0;
            pos->dec=sqlite3_column_double(stmt2, 1);
            strcpy( param,sqlite3_column_text(stmt2, 2));
            //printf("%s\n",param);
        }
        sqlite3_reset(stmt2) ;
    };
    break;

    case ('f'):
    {
        param[0]=toupper(param[0]) ;
        //strcpy(temp,param);
        strcat(temp,param);
        strcat(temp,"%");
        printf("%s\n",temp);
        sqlite3_bind_text(stmt3, 1,temp, -1, SQLITE_STATIC);

        while(SQLITE_ROW == sqlite3_step(stmt3))
        {
            result=1;
            sprintf(out,"%s,%f,%f", sqlite3_column_text(stmt3, 2),sqlite3_column_double(stmt3, 0),sqlite3_column_double(stmt3,1));
            printf("%s\n",out);
            pos->ra=sqlite3_column_double(stmt3, 0)*15.0;
            pos->dec=sqlite3_column_double(stmt3, 1);
            strcpy( param,sqlite3_column_text(stmt3, 2));
            //printf("%s\n",param);
        }
        sqlite3_reset(stmt3) ;
    };
    break;
    case 'I':
    case 'N':
    {


        sqlite3_bind_text(stmt, 1,(char*)param, -1, SQLITE_STATIC);

        while(SQLITE_ROW == sqlite3_step(stmt))
        {
            result=1;
            sprintf(out,"%s,%f,%f,%s,%s", sqlite3_column_text(stmt, 0),sqlite3_column_double(stmt, 1),sqlite3_column_double(stmt, 2),
                    sqlite3_column_text(stmt, 4),sqlite3_column_text(stmt, 5));
            printf("%s\n",out);
            pos->ra=sqlite3_column_double(stmt, 1)*15.0;
            pos->dec=sqlite3_column_double(stmt, 2);
        }
        sqlite3_reset(stmt) ;

    }
    break;
    case ('M'):

    {
        sqlite3_bind_text(stmt1, 1,(char*)param, -1, SQLITE_STATIC);

        while(SQLITE_ROW == sqlite3_step(stmt1))
        {
            result=1;
            sprintf(out,"%s,%f,%f,%s,%s", sqlite3_column_text(stmt1, 5),sqlite3_column_double(stmt1, 1),sqlite3_column_double(stmt1,2),
                    sqlite3_column_text(stmt1, 4),sqlite3_column_text(stmt1, 0));
            printf("%s\n",out);
            pos->ra=sqlite3_column_double(stmt1, 1)*15.0;
            pos->dec=sqlite3_column_double(stmt1, 2);
        }
        sqlite3_reset(stmt1) ;
    }
    break;

    default :

        break;
    }
    if (!result)
    {
        pos->ra=0;
        pos->dec=0;
        sprintf(out,"Object %s not found in DB",param);
        printf("%s\n",out);
    }
    return result;

}

int initDB()
{
    //  sqlite3 *db;
    //  sqlite3_stmt *stmt;

    int rc = sqlite3_open(OPEN_DB, &db);


    if (rc != SQLITE_OK)
        exit_with_error(db, "can't open db: ");

    rc = sqlite3_prepare_v2(db, "SELECT ngc,AR,dec,common_names,type,messier FROM OPENNGC WHERE ngc=?1 ;", -1, &stmt, 0);
    if (rc != SQLITE_OK)
        exit_with_error(db, "failure fetching data: ");
    rc = sqlite3_prepare_v2(db, "SELECT ngc,AR,dec,common_names,type,messier FROM OPENNGC WHERE messier=?1;", -1, &stmt1, 0);

    if (rc != SQLITE_OK)
        exit_with_error(db, "failure fetching data: ");

    rc = sqlite3_prepare_v2(db, "SELECT ra,declination,propername FROM stars WHERE propername like ?1;", -1, &stmt2, 0);


    if (rc != SQLITE_OK)
        exit_with_error(db, "failure fetching data: ");


    //rc = sqlite3_prepare_v2(db, "SELECT ra,declination,BayerFlam FROM stars WHERE BayerFlam like ?1;", -1, &stmt2, 0);
    if (rc != SQLITE_OK)
        exit_with_error(db, "failure fetching data: ");

    rc = sqlite3_prepare_v2(db, "SELECT ra,declination,BayerFlam FROM stars WHERE BayerFlam like ?1;", -1, &stmt3, 0);


    if (rc != SQLITE_OK)
        exit_with_error(db, "failure fetching data: ");

    return 0;
}

void closeDB()
{
    sqlite3_finalize(stmt);
    sqlite3_finalize(stmt1);
    sqlite3_close(db);
}


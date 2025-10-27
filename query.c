#include "query.h"
#include <math.h>

sqlite3 *db;
sqlite3_stmt *stmt,*stmt1,*stmt2,*stmt3,*stmt4;

double tpCal(char * tp)
{
    char temp[20];
    struct ln_date epoch_date;
     printf("tp %s\n",tp);
    strncpy(temp, tp, 4);
    temp[4]=0;
    printf("-%s",temp);
    epoch_date.years = atoi(temp);
    strncpy(temp, tp+4, 2);
    temp[2]=0;
    printf("-%s",temp);
    epoch_date.months = atoi(temp);
    strncpy(temp, tp+6, 2);
    temp[2]=0;
    printf("-%s",temp);
    epoch_date.days = atoi(temp);
    strncpy(temp, tp+8, strlen(tp)-8);
    printf("-%s\n",temp);
    double day =atof(temp)*24.0;
      printf("-%f\n",day);
    epoch_date.hours = round(day);
    epoch_date.minutes = 0;
    epoch_date.seconds = 0;
    day=ln_get_julian_day(&epoch_date);
    printf("JD %f\n",day);
    return day;

}

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
            strcpy( ( char * restrict) param,(const char *) sqlite3_column_text(stmt2, 2));
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
            strcpy( ( char * restrict) param,(const char *) sqlite3_column_text(stmt3, 2));
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
    case ('c'):
    {
        struct ln_ell_orbit orbit;
        param[0]=toupper(param[0]) ;
        //strcpy(temp,param);
        strcat(temp,param);
        strcat(temp,"%");
        printf("%s\n",temp);
        sqlite3_bind_text(stmt4, 1,temp, -1, SQLITE_STATIC);
        //"SELECT full_name,epoch_mjd,q,e,i,w,om FROM comet WHERE full_name like ?1;"
        double JD = ln_get_julian_from_sys();
        while(SQLITE_ROW == sqlite3_step(stmt4))
        {
            result=1;
            sprintf(out,"%s",sqlite3_column_text(stmt4, 7));
            double  eJD= tpCal(out);
            sprintf(out,"%s,%f,%f,%f,%f,%f", sqlite3_column_text(stmt4, 0),sqlite3_column_double(stmt4, 2),sqlite3_column_double(stmt4,3),
                    sqlite3_column_double(stmt4, 4),sqlite3_column_double(stmt4,5),sqlite3_column_double(stmt4,6));
            printf("%s\n",out);
            orbit.JD =  eJD;//sqlite3_column_double(stmt4, 1)+25.0;//+2451544.5;//+2400000.5;
            double q= sqlite3_column_double(stmt4, 2);
            orbit.e = sqlite3_column_double(stmt4, 3);
            orbit.a = ln_get_ell_smajor_diam(orbit.e,q);
            orbit.i = sqlite3_column_double(stmt4, 4);
            orbit.omega = sqlite3_column_double(stmt4, 6);
            orbit.w = sqlite3_column_double(stmt4, 5);
            orbit.n = 0.0;//0.2990330;
            if (orbit.n == 0.0)		orbit.n = ln_get_ell_mean_motion(orbit.a);
            ln_get_ell_body_equ_coords(JD, &orbit, pos);
            fprintf(stdout, "(RA) for comet   %f\n", pos->ra);
            fprintf(stdout, "(Dec) for comet   %f\n", pos->dec);
            strcpy( ( char * restrict) param,(const char *) sqlite3_column_text(stmt4, 0));

            //printf("%s\n",param);
        }
        sqlite3_reset(stmt4) ;
    };
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

    rc = sqlite3_prepare_v2(db, "SELECT full_name,epoch_mjd,q,e,i,w,om,tp_cal FROM comet WHERE full_name like ?1;", -1, &stmt4, 0);


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


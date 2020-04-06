/* Processed by ecpg (12.2) */
/* These include files are added by the preprocessor */
#include <ecpglib.h>
#include <ecpgerrno.h>
#include <sqlca.h>
/* End of automatic include section */

#line 1 "sg0130Company.x"
/* Source code for sg0130Company.x
By Srishti Gajare
March 25,2020
*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


#line 1 "/usr/local/include/sqlca.h"
#ifndef POSTGRES_SQLCA_H
#define POSTGRES_SQLCA_H

#ifndef PGDLLIMPORT
#if  defined(WIN32) || defined(__CYGWIN__)
#define PGDLLIMPORT __declspec (dllimport)
#else
#define PGDLLIMPORT
#endif							/* __CYGWIN__ */
#endif							/* PGDLLIMPORT */

#define SQLERRMC_LEN	150

#ifdef __cplusplus
extern "C"
{
#endif

struct sqlca_t
{
	char		sqlcaid[8];
	long		sqlabc;
	long		sqlcode;
	struct
	{
		int			sqlerrml;
		char		sqlerrmc[SQLERRMC_LEN];
	}			sqlerrm;
	char		sqlerrp[8];
	long		sqlerrd[6];
	/* Element 0: empty						*/
	/* 1: OID of processed tuple if applicable			*/
	/* 2: number of rows processed				*/
	/* after an INSERT, UPDATE or				*/
	/* DELETE statement					*/
	/* 3: empty						*/
	/* 4: empty						*/
	/* 5: empty						*/
	char		sqlwarn[8];
	/* Element 0: set to 'W' if at least one other is 'W'	*/
	/* 1: if 'W' at least one character string		*/
	/* value was truncated when it was			*/
	/* stored into a host variable.             */

	/*
	 * 2: if 'W' a (hopefully) non-fatal notice occurred
	 */	/* 3: empty */
	/* 4: empty						*/
	/* 5: empty						*/
	/* 6: empty						*/
	/* 7: empty						*/

	char		sqlstate[5];
};

struct sqlca_t *ECPGget_sqlca(void);

#ifndef POSTGRES_ECPG_INTERNAL
#define sqlca (*ECPGget_sqlca())
#endif

#ifdef __cplusplus
}
#endif

#endif

#line 9 "sg0130Company.x"

/* exec sql whenever sqlerror  sqlprint ; */
#line 10 "sg0130Company.x"


/*Declare Global variables*/
/* exec sql begin declare section */
     
     
     
     
     
     
     
     
      
     
     

#line 14 "sg0130Company.x"
 char * fname = NULL ;
 
#line 15 "sg0130Company.x"
 char * lname = NULL ;
 
#line 16 "sg0130Company.x"
 char * pname = NULL ;
 
#line 17 "sg0130Company.x"
 char * dname = NULL ;
 
#line 18 "sg0130Company.x"
 char * pno ;
 
#line 19 "sg0130Company.x"
 char * cli_ssn ;
 
#line 20 "sg0130Company.x"
 float work_hours , cli_hours , added_hours , sub_hours , total_hours ;
 
#line 21 "sg0130Company.x"
 float emp_salary , dept_avg_salary ;
 
#line 22 "sg0130Company.x"
 int pnumber , dnum ;
 
#line 23 "sg0130Company.x"
 int no_of_deps ;
 
#line 24 "sg0130Company.x"
 int flag ;
/* exec sql end declare section */
#line 25 "sg0130Company.x"


int main(int argc,char *argv[])
{
    int add;
    int subtract;
    /*Function to perform add operation*/
    int add_func();
    /*Function to perform Subtract operation*/
    int sub_func();
    /*Function to provide other information related to employee*/
    int EmpDetails();
    add=strcmp(argv[1],"-add");
    subtract=strcmp(argv[1],"-subtract");
    if(argc==8)
    {
        if(add==0)
        {
            if (strcmp(argv[2],"-ssn")==0)
                cli_ssn=argv[3];
            else
            {   
                printf("Please enter a valid SSN argument\n");
                exit(0);
            }
            if (strcmp(argv[4],"-pno")==0)
                pnumber=atoi(argv[5]);
            else
            {
                printf("Please enter a valid Project number argument\n");
                exit(0);
            }
            if(strcmp(argv[6],"-hours")==0)
                sscanf(argv[7],"%f",&cli_hours);
            else
            {
                printf("Please enter valid Hours argument\n");
                exit(0);
            }
            /*Call the add function if correct parameters are passed from CLI*/
            add_func();
        }
        else if(subtract==0)
        {
            if (strcmp(argv[2],"-ssn")==0)
                cli_ssn=argv[3];
            else
            {
                printf("Please enter a valid SSN argument\n");
                exit(0);
            }
            if (strcmp(argv[4],"-pno")==0)
                pnumber=atoi(argv[5]);
            else
            {
                printf("Please enter a valid Project number argument\n");
                exit(0);
            }
            if(strcmp(argv[6],"-hours")==0)
                sscanf(argv[7],"%f",&cli_hours);
            else
            {
                printf("Please enter valid Hours argument\n");
                exit(0);
            }
            /*Call the Subtract function if correct parameters are passed from CLI*/
            sub_func();
        }
        else
        {
            printf("Please pass right arguement names\n");
            exit(0);
        }
    }
    else
    {
        printf("The CLI arguments entered is incorrect, Please enter the right arguments\n");
        exit(0);
    }    
    { ECPGtrans(__LINE__, NULL, "commit");
#line 104 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 104 "sg0130Company.x"

    { ECPGdisconnect(__LINE__, "CURRENT");
#line 105 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 105 "sg0130Company.x"

    return 0;   
}

/*Add funtion implementation*/
int add_func()
{
    { ECPGconnect(__LINE__, 0, "unix:postgresql://localhost/cs687" , "sg0130" , "687studentspring2020" , NULL, 0); 
#line 112 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 112 "sg0130Company.x"

    if(SQLCODE==0)
    {
        printf("\nConnected to Database\n\n");
    }
    else
    {
        printf("Connection to database failed\n\n");
        exit(0);
    }    
    /*CURSOR defined to retrieve the details of the employee,the details 
    of the project that he/she is working on along with number of hours spent on that project*/
    /* declare emp_proj_details cursor for select e . fname , e . lname , p . pname , coalesce ( w . hours , 0 ) from sg0130 . WORKS_ON w , sg0130 . EMPLOYEE e , sg0130 . PROJECT p where w . essn = $1  and w . essn = e . ssn and p . pnumber = w . Pno and pno = $2  */
#line 130 "sg0130Company.x"

    /*Opening the CURSOR*/
    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "declare emp_proj_details cursor for select e . fname , e . lname , p . pname , coalesce ( w . hours , 0 ) from sg0130 . WORKS_ON w , sg0130 . EMPLOYEE e , sg0130 . PROJECT p where w . essn = $1  and w . essn = e . ssn and p . pnumber = w . Pno and pno = $2 ", 
	ECPGt_char,&(cli_ssn),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(pnumber),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);
#line 132 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 132 "sg0130Company.x"

    /*Fetching the CURSOR*/
    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "fetch in emp_proj_details", ECPGt_EOIT, 
	ECPGt_char,&(fname),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,&(lname),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,&(pname),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_float,&(work_hours),(long)1,(long)1,sizeof(float), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);
#line 134 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 134 "sg0130Company.x"

    /*If the above operation returns true for the given Employee/SSN, add the specified hours to existing hours*/
    if(SQLCODE==0)
    { 
        printf("The number of hours worked on project %s by %s %s is %.2f hours\n\n",pname,fname,lname,work_hours); 
        /*Variable that holds addition of existing hours and hours passed as parameter from CLI*/
        added_hours=work_hours+cli_hours;

        { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "update sg0130 . WORKS_ON set hours = $1  where essn = $2  and pno = $3 ", 
	ECPGt_float,&(added_hours),(long)1,(long)1,sizeof(float), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,&(cli_ssn),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(pnumber),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);
#line 145 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 145 "sg0130Company.x"

        printf("The number of hours worked on project %s by employee %s %s increased from %.2f to %.2f hours\n\n",pname,fname,lname,work_hours,added_hours);
        fname=NULL;
        lname=NULL;
        pname=NULL;
        free(fname);
        free(lname);
        free(pname);
        { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "close emp_proj_details", ECPGt_EOIT, ECPGt_EORT);
#line 153 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 153 "sg0130Company.x"

    }
    /*Insert the Employee details in works_on for the given pno from CLI if he's not working on that pno*/
    else 
	{
        { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "insert into sg0130 . works_on values ( $1  , $2  , $3  )", 
	ECPGt_char,&(cli_ssn),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(pnumber),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_float,&(cli_hours),(long)1,(long)1,sizeof(float), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);
#line 158 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 158 "sg0130Company.x"

        /*If insert is success provide new project and working hours details*/
        if(SQLCODE==0)
        {
            /* declare emp_insert cursor for select e . fname , e . lname , p . pname , w . hours from sg0130 . employee e , sg0130 . project p , sg0130 . works_on w where w . pno = p . pnumber and w . essn = e . ssn and ssn = $1  and pnumber = $2  */
#line 168 "sg0130Company.x"

            { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "declare emp_insert cursor for select e . fname , e . lname , p . pname , w . hours from sg0130 . employee e , sg0130 . project p , sg0130 . works_on w where w . pno = p . pnumber and w . essn = e . ssn and ssn = $1  and pnumber = $2 ", 
	ECPGt_char,&(cli_ssn),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(pnumber),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);
#line 169 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 169 "sg0130Company.x"

            { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "fetch in emp_insert", ECPGt_EOIT, 
	ECPGt_char,&(fname),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,&(lname),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,&(pname),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_float,&(cli_hours),(long)1,(long)1,sizeof(float), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);
#line 170 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 170 "sg0130Company.x"

            printf("%s %s doesnt work on project %s\n\n",fname,lname,pname);
            printf("Employee %s %s started to work on project %s for %.2f hours\n\n",fname,lname,pname,cli_hours);
            fname=NULL;
            lname=NULL;
            pname=NULL;
            free(fname);
            free(lname);
            free(pname);
            { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "close emp_insert", ECPGt_EOIT, ECPGt_EORT);
#line 179 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 179 "sg0130Company.x"

        }
    }
    /*Call the function to display other information related to employee*/
    EmpDetails();
    return 0;    
}

/*Subtract function implementation*/
int sub_func()
{
    { ECPGconnect(__LINE__, 0, "unix:postgresql://localhost/cs687" , "sg0130" , "687studentspring2020" , NULL, 0); 
#line 190 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 190 "sg0130Company.x"

    if (SQLCODE == 0)
    {
        printf("\nConnected to Database\n\n");
    }
    else
    {
        printf("Connection to Database failed\n");
        return 0;
    }    
   /*CURSOR defined to retrieve the details of the employee,the details 
    of the project that he/she is working on along with number of hours spent on that project*/
    /* declare emp_proj_details_sub cursor for select e . fname , e . lname , p . pname , coalesce ( w . hours , 0 ) from sg0130 . WORKS_ON w , sg0130 . EMPLOYEE e , sg0130 . PROJECT p where w . essn = $1  and w . essn = e . ssn and p . pnumber = w . Pno and pno = $2  */
#line 208 "sg0130Company.x"

    /*Opening the CURSOR*/
    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "declare emp_proj_details_sub cursor for select e . fname , e . lname , p . pname , coalesce ( w . hours , 0 ) from sg0130 . WORKS_ON w , sg0130 . EMPLOYEE e , sg0130 . PROJECT p where w . essn = $1  and w . essn = e . ssn and p . pnumber = w . Pno and pno = $2 ", 
	ECPGt_char,&(cli_ssn),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(pnumber),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);
#line 210 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 210 "sg0130Company.x"

    /*Fetching the CURSOR*/
    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "fetch in emp_proj_details_sub", ECPGt_EOIT, 
	ECPGt_char,&(fname),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,&(lname),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,&(pname),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_float,&(work_hours),(long)1,(long)1,sizeof(float), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);
#line 212 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 212 "sg0130Company.x"
 
    /*If the above operation returns true for the given Employee/SSN, subtract the specified hours to existing hours*/
    if(SQLCODE==0)
    {
        printf("The number of hours worked on project %s by %s %s is %.2f\n\n",pname,fname,lname,work_hours);
        sub_hours=work_hours-cli_hours;
        /*If final hours <=0 delete the entry of employee from works on table*/
        if(sub_hours<=0)
		{
			{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "delete from sg0130 . WORKS_ON where essn = $1  and pno = $2 ", 
	ECPGt_char,&(cli_ssn),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(pnumber),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);
#line 223 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 223 "sg0130Company.x"

			printf("Employee %s %s used to work on project %s for %.2f hours.The employee stopped working on this project\n\n",fname,lname,pname,work_hours);
            fname=NULL;
            lname=NULL;
            pname=NULL;
        }
        else
        {   
            { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "update sg0130 . WORKS_ON set hours = $1  where essn = $2  and pno = $3 ", 
	ECPGt_float,&(sub_hours),(long)1,(long)1,sizeof(float), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,&(cli_ssn),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(pnumber),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);
#line 234 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 234 "sg0130Company.x"

            printf("The number of hours worked on project %s by employee %s %s decreased from %.2f to %.2f\n\n",pname,fname,lname,work_hours,sub_hours);
            fname=NULL;
            lname=NULL;
            pname=NULL;        
        }
    }
    /*Perform this operation if the employee doesnt work on the project specified in CLI*/
    else
    {
        /* declare emp_no_proj cursor for select e . fname , e . lname , p . pname from sg0130 . employee e , sg0130 . project p where pnumber = $1  and ssn = $2  */
#line 248 "sg0130Company.x"

        { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "declare emp_no_proj cursor for select e . fname , e . lname , p . pname from sg0130 . employee e , sg0130 . project p where pnumber = $1  and ssn = $2 ", 
	ECPGt_int,&(pnumber),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,&(cli_ssn),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);
#line 249 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 249 "sg0130Company.x"

        { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "fetch in emp_no_proj", ECPGt_EOIT, 
	ECPGt_char,&(fname),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,&(lname),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,&(pname),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);
#line 250 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 250 "sg0130Company.x"

        printf("%s %s doesnt work on project %s\n\n",fname,lname,pname);
        { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "close emp_no_proj", ECPGt_EOIT, ECPGt_EORT);
#line 252 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 252 "sg0130Company.x"

        fname=NULL;
        lname=NULL;
        pname=NULL;
    }
    free(fname);
    free(lname);
    free(pname);
    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "close emp_proj_details_sub", ECPGt_EOIT, ECPGt_EORT);
#line 260 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 260 "sg0130Company.x"

    /*Call the function to display other information related to employee*/
    EmpDetails();
    return 0;
}

/*Employee other Details implementation function*/
int EmpDetails()
{
    flag=0;
    float diff_salary;

    /*Define a cursor to retrieve all the projects and corresponding working hours of the employee*/
    /* declare emp_proj cursor for select e . fname , e . lname , p . pname , w . pno , w . hours from sg0130 . works_on w , sg0130 . project p , sg0130 . employee e where ssn = $1  and pno = pnumber and essn = ssn */
#line 278 "sg0130Company.x"

    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "declare emp_proj cursor for select e . fname , e . lname , p . pname , w . pno , w . hours from sg0130 . works_on w , sg0130 . project p , sg0130 . employee e where ssn = $1  and pno = pnumber and essn = ssn", 
	ECPGt_char,&(cli_ssn),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);
#line 279 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 279 "sg0130Company.x"

    /* exec sql whenever not found  break ; */
#line 280 "sg0130Company.x"


    while (SQLCODE==0)
    {
       { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "fetch in emp_proj", ECPGt_EOIT, 
	ECPGt_char,&(fname),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,&(lname),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,&(pname),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(pnumber),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_float,&(work_hours),(long)1,(long)1,sizeof(float), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);
#line 284 "sg0130Company.x"

if (sqlca.sqlcode == ECPG_NOT_FOUND) break;
#line 284 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 284 "sg0130Company.x"

       printf("1.*)The Employee %s %s works on Project Number: %d Project Name: %s for %.2f hours\n\n",fname,lname,pnumber,pname,work_hours);
       fname=NULL;
       lname=NULL;
       pname=NULL; 
    }
    free(fname);
    free(lname);
    free(pname);
    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "close emp_proj", ECPGt_EOIT, ECPGt_EORT);
#line 293 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 293 "sg0130Company.x"

    
    /*Define a cursor to retrieve the total hours worked by the employee on all the project*/
    /* declare emp_total_hours cursor for select e . fname , e . lname , sum ( w . hours ) as total_hours from sg0130 . employee e , sg0130 . project p , sg0130 . works_on w where essn = ssn and pno = pnumber and ssn = $1  group by fname , lname */
#line 302 "sg0130Company.x"

	{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "declare emp_total_hours cursor for select e . fname , e . lname , sum ( w . hours ) as total_hours from sg0130 . employee e , sg0130 . project p , sg0130 . works_on w where essn = ssn and pno = pnumber and ssn = $1  group by fname , lname", 
	ECPGt_char,&(cli_ssn),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);
#line 303 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 303 "sg0130Company.x"

	/* exec sql whenever not found  break ; */
#line 304 "sg0130Company.x"


	while (SQLCODE==0)
	{
        if(flag==1)
		    break;
        { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "fetch in emp_total_hours", ECPGt_EOIT, 
	ECPGt_char,&(fname),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,&(lname),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_float,&(total_hours),(long)1,(long)1,sizeof(float), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);
#line 310 "sg0130Company.x"

if (sqlca.sqlcode == ECPG_NOT_FOUND) break;
#line 310 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 310 "sg0130Company.x"
  
        printf("2.The Employee %s %s works total of %.2f hours on all projects\n\n",fname,lname,total_hours);
        flag=1;
	}
    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "close emp_total_hours", ECPGt_EOIT, ECPGt_EORT);
#line 314 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 314 "sg0130Company.x"


    /*Define a cursor to retrieve the salary the department the employee belongs too*/
    /* declare emp_dept_details cursor for select e . fname , e . lname , d . dname , e . salary , e . dno from sg0130 . employee e , sg0130 . department d where e . dno = d . dnumber and ssn = $1  */
#line 321 "sg0130Company.x"

    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "declare emp_dept_details cursor for select e . fname , e . lname , d . dname , e . salary , e . dno from sg0130 . employee e , sg0130 . department d where e . dno = d . dnumber and ssn = $1 ", 
	ECPGt_char,&(cli_ssn),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);
#line 322 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 322 "sg0130Company.x"

    /* exec sql whenever not found  break ; */
#line 323 "sg0130Company.x"

    
    while(SQLCODE==0)
    {
        { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "fetch in emp_dept_details", ECPGt_EOIT, 
	ECPGt_char,&(fname),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,&(lname),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,&(dname),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_float,&(emp_salary),(long)1,(long)1,sizeof(float), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(dnum),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);
#line 327 "sg0130Company.x"

if (sqlca.sqlcode == ECPG_NOT_FOUND) break;
#line 327 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 327 "sg0130Company.x"

        printf("3.The EMPLOYEE %s %s works in Department: '%s' and his/her salary is $%.2f\n\n",fname,lname,dname,emp_salary);
    }
    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "close emp_dept_details", ECPGt_EOIT, ECPGt_EORT);
#line 330 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 330 "sg0130Company.x"


    /*Define a cursor to retrieve the number of dependents if any of an employee*/
	/* declare emp_deps cursor for select e . fname , e . lname , count ( t . dependent_name ) no_of_dependents from sg0130 . employee e , sg0130 . dependent t where e . ssn = t . essn and e . ssn = $1  group by t . essn , e . fname , e . lname */
#line 338 "sg0130Company.x"

	{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "declare emp_deps cursor for select e . fname , e . lname , count ( t . dependent_name ) no_of_dependents from sg0130 . employee e , sg0130 . dependent t where e . ssn = t . essn and e . ssn = $1  group by t . essn , e . fname , e . lname", 
	ECPGt_char,&(cli_ssn),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);
#line 339 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 339 "sg0130Company.x"

    /* exec sql whenever not found  break ; */
#line 340 "sg0130Company.x"


	while (SQLCODE==0)
	{	
		{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "fetch in emp_deps", ECPGt_EOIT, 
	ECPGt_char,&(fname),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,&(lname),(long)0,(long)1,(1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(no_of_deps),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);
#line 344 "sg0130Company.x"

if (sqlca.sqlcode == ECPG_NOT_FOUND) break;
#line 344 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 344 "sg0130Company.x"

		printf("4.The no of dependents of Employee %s %s are %d\n\n",fname,lname,no_of_deps);   
	}
    /*If the employee has no dependents then enter this loop*/
	if(no_of_deps==0)
	{   
        flag=0;
		while (SQLCODE==100)
		{
			if (flag==1)
				break;
			printf("4.The employee %s %s has no dependents\n\n",fname,lname);
			flag = 1;
		}
	}
	{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "close emp_deps", ECPGt_EOIT, ECPGt_EORT);
#line 359 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 359 "sg0130Company.x"


    /*Define cursor to retrieve average salary of each department and hence find the difference 
    between this and employee salary*/
    /* declare emp_dept_sal cursor for select avg ( e . salary ) from sg0130 . employee e where dno = $1  group by dno having count ( e . ssn ) >= 1 */
#line 368 "sg0130Company.x"

    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "declare emp_dept_sal cursor for select avg ( e . salary ) from sg0130 . employee e where dno = $1  group by dno having count ( e . ssn ) >= 1", 
	ECPGt_int,&(dnum),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);
#line 369 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 369 "sg0130Company.x"

    /* exec sql whenever not found  break ; */
#line 370 "sg0130Company.x"

    
    while(SQLCODE==0)
    {
        { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "fetch in emp_dept_sal", ECPGt_EOIT, 
	ECPGt_float,&(dept_avg_salary),(long)1,(long)1,sizeof(float), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);
#line 374 "sg0130Company.x"

if (sqlca.sqlcode == ECPG_NOT_FOUND) break;
#line 374 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 374 "sg0130Company.x"

        diff_salary=abs(emp_salary-dept_avg_salary);
        printf("5.Department %d Avg Salary:$%.2f, The employee %s %s has difference salary of $%.2f\n\n",dnum,dept_avg_salary,
        fname,lname,diff_salary);
    }
    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "close emp_dept_sal", ECPGt_EOIT, ECPGt_EORT);
#line 379 "sg0130Company.x"

if (sqlca.sqlcode < 0) sqlprint();}
#line 379 "sg0130Company.x"

}
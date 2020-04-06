/* Source code for sample.x
By Srishti Gajare
March 25,2020
*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

EXEC SQL INCLUDE sqlca;
EXEC SQL WHENEVER SQLERROR sqlprint;

/*Declare Global variables*/
EXEC SQL BEGIN DECLARE SECTION;
    char *fname=NULL;
    char *lname=NULL;
    char *pname=NULL;
    char *dname=NULL;
    char *pno;
    char *cli_ssn;
    float work_hours,cli_hours,added_hours,sub_hours,total_hours;
    float emp_salary,dept_avg_salary;
    int pnumber, dnum;
    int no_of_deps;
    int flag;
EXEC SQL END DECLARE SECTION;

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
    EXEC SQL COMMIT;
    EXEC SQL DISCONNECT;
    return 0;   
}

/*Add funtion implementation*/
int add_func()
{
    EXEC SQL CONNECT TO unix:postgresql://localhost/cs687 USER sg0130 USING "687studentspring2020";
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
    EXEC SQL DECLARE emp_proj_details CURSOR FOR
    SELECT e.fname, e.lname, p.pname, COALESCE(w.hours,0)
	FROM sg0130.WORKS_ON w, sg0130.EMPLOYEE e, sg0130.PROJECT p
	WHERE w.essn = :cli_ssn AND
	w.essn = e.ssn AND
	p.pnumber = w.Pno AND
	pno= :pnumber;
    /*Opening the CURSOR*/
    EXEC SQL OPEN emp_proj_details;
    /*Fetching the CURSOR*/
    EXEC SQL FETCH IN emp_proj_details INTO :fname, :lname, :pname,:work_hours;
    /*If the above operation returns true for the given Employee/SSN, add the specified hours to existing hours*/
    if(SQLCODE==0)
    { 
        printf("The number of hours worked on project %s by %s %s is %.2f hours\n\n",pname,fname,lname,work_hours); 
        /*Variable that holds addition of existing hours and hours passed as parameter from CLI*/
        added_hours=work_hours+cli_hours;

        EXEC SQL UPDATE sg0130.WORKS_ON
		SET hours = :added_hours
		WHERE essn = :cli_ssn AND
		pno= :pnumber;
        printf("The number of hours worked on project %s by employee %s %s increased from %.2f to %.2f hours\n\n",pname,fname,lname,work_hours,added_hours);
        fname=NULL;
        lname=NULL;
        pname=NULL;
        free(fname);
        free(lname);
        free(pname);
        EXEC SQL CLOSE emp_proj_details;
    }
    /*Insert the Employee details in works_on for the given pno from CLI if he's not working on that pno*/
    else 
	{
        EXEC SQL INSERT INTO sg0130.works_on values(:cli_ssn,:pnumber,:cli_hours);
        /*If insert is success provide new project and working hours details*/
        if(SQLCODE==0)
        {
            EXEC SQL DECLARE emp_insert CURSOR FOR
            SELECT e.fname,e.lname,p.pname,w.hours from
            sg0130.employee e,sg0130.project p,sg0130.works_on w
            WHERE w.pno=p.pnumber and
            w.essn=e.ssn and
            ssn=:cli_ssn and
            pnumber=:pnumber;
            EXEC SQL OPEN emp_insert;
            EXEC SQL FETCH IN emp_insert INTO :fname,:lname,:pname,:cli_hours;
            printf("%s %s doesnt work on project %s\n\n",fname,lname,pname);
            printf("Employee %s %s started to work on project %s for %.2f hours\n\n",fname,lname,pname,cli_hours);
            fname=NULL;
            lname=NULL;
            pname=NULL;
            free(fname);
            free(lname);
            free(pname);
            EXEC SQL CLOSE emp_insert;
        }
    }
    /*Call the function to display other information related to employee*/
    EmpDetails();
    return 0;    
}

/*Subtract function implementation*/
int sub_func()
{
    EXEC SQL CONNECT TO unix:postgresql://localhost/cs687 USER sg0130 USING "687studentspring2020";
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
    EXEC SQL DECLARE emp_proj_details_sub CURSOR FOR
    SELECT e.fname, e.lname, p.pname, COALESCE(w.hours,0)
	FROM sg0130.WORKS_ON w, sg0130.EMPLOYEE e, sg0130.PROJECT p
	WHERE w.essn = :cli_ssn AND
	w.essn = e.ssn AND
	p.pnumber = w.Pno AND
	pno= :pnumber;
    /*Opening the CURSOR*/
    EXEC SQL OPEN emp_proj_details_sub;
    /*Fetching the CURSOR*/
    EXEC SQL FETCH IN emp_proj_details_sub INTO :fname, :lname, :pname,:work_hours; 
    /*If the above operation returns true for the given Employee/SSN, subtract the specified hours to existing hours*/
    if(SQLCODE==0)
    {
        printf("The number of hours worked on project %s by %s %s is %.2f\n\n",pname,fname,lname,work_hours);
        sub_hours=work_hours-cli_hours;
        /*If final hours <=0 delete the entry of employee from works on table*/
        if(sub_hours<=0)
	{
		EXEC SQL DELETE FROM sg0130.WORKS_ON
		WHERE essn = :cli_ssn AND
		pno= :pnumber;
		printf("Employee %s %s used to work on project %s for %.2f hours.The employee stopped working on this project\n\n",fname,lname,pname,work_hours);
            	fname=NULL;
            	lname=NULL;
            	pname=NULL;
        }
        else
        {   
            EXEC SQL UPDATE sg0130.WORKS_ON
	    SET hours = :sub_hours
	    WHERE essn = :cli_ssn AND
	    pno= :pnumber;
            printf("The number of hours worked on project %s by employee %s %s decreased from %.2f to %.2f\n\n",pname,fname,lname,work_hours,sub_hours);
            fname=NULL;
            lname=NULL;
            pname=NULL;        
        }
    }
    /*Perform this operation if the employee doesnt work on the project specified in CLI*/
    else
    {
        EXEC SQL DECLARE emp_no_proj CURSOR FOR
        SELECT e.fname,e.lname,p.pname 
        from sg0130.employee e,sg0130.project p
        WHERE pnumber=:pnumber and
        ssn=:cli_ssn;
        EXEC SQL OPEN emp_no_proj;
        EXEC SQL FETCH IN emp_no_proj INTO :fname,:lname,:pname;
        printf("%s %s doesnt work on project %s\n\n",fname,lname,pname);
        EXEC SQL CLOSE emp_no_proj;
        fname=NULL;
        lname=NULL;
        pname=NULL;
    }
    free(fname);
    free(lname);
    free(pname);
    EXEC SQL CLOSE emp_proj_details_sub;
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
    EXEC SQL DECLARE emp_proj CURSOR FOR
    SELECT  e.fname,e.lname,p.pname, w.pno,w.hours
    FROM sg0130.works_on w ,sg0130.project p,sg0130.employee e
    WHERE ssn=:cli_ssn and 
    pno=pnumber and 
    essn=ssn;
    EXEC SQL OPEN emp_proj;
    EXEC SQL WHENEVER NOT FOUND DO BREAK;

    while (SQLCODE==0)
    {
       EXEC SQL FETCH IN emp_proj INTO :fname, :lname,:pname,:pnumber,:work_hours;
       printf("1.*)The Employee %s %s works on Project Number: %d Project Name: %s for %.2f hours\n\n",fname,lname,pnumber,pname,work_hours);
       fname=NULL;
       lname=NULL;
       pname=NULL; 
    }
    free(fname);
    free(lname);
    free(pname);
    EXEC SQL CLOSE emp_proj;
    
    /*Define a cursor to retrieve the total hours worked by the employee on all the project*/
    EXEC SQL DECLARE emp_total_hours CURSOR FOR  
    SELECT e.fname,e.lname,sum(w.hours) as total_hours 
    FROM sg0130.employee e,sg0130.project p,sg0130.works_on w
    WHERE essn=ssn and 
    pno=pnumber and
    ssn=:cli_ssn 
    group by fname,lname;
    EXEC SQL OPEN emp_total_hours;
    EXEC SQL WHENEVER NOT FOUND DO BREAK;

    while (SQLCODE==0)
    {
        if(flag==1)
		break;
        EXEC SQL FETCH IN emp_total_hours INTO :fname, :lname,:total_hours;  
        printf("2.The Employee %s %s works total of %.2f hours on all projects\n\n",fname,lname,total_hours);
        flag=1;
    }
    EXEC SQL CLOSE emp_total_hours;

    /*Define a cursor to retrieve the salary the department the employee belongs too*/
    EXEC SQL DECLARE emp_dept_details CURSOR FOR
    SELECT e.fname,e.lname,d.dname,e.salary,e.dno from
    sg0130.employee e,sg0130.department d WHERE
    e.dno=d.dnumber and
    ssn=:cli_ssn;
    EXEC SQL OPEN emp_dept_details;
    EXEC SQL WHENEVER NOT FOUND DO BREAK;
    
    while(SQLCODE==0)
    {
        EXEC SQL FETCH in emp_dept_details INTO :fname,:lname,:dname,:emp_salary,:dnum;
        printf("3.The EMPLOYEE %s %s works in Department: '%s' and his/her salary is $%.2f\n\n",fname,lname,dname,emp_salary);
    }
    EXEC SQL CLOSE emp_dept_details;

    /*Define a cursor to retrieve the number of dependents if any of an employee*/
	EXEC SQL DECLARE emp_deps CURSOR FOR 
	SELECT e.fname,e.lname, count(t.dependent_name) no_of_dependents 
	FROM sg0130.employee e, sg0130.dependent t 
	WHERE e.ssn = t.essn
	AND e.ssn = :cli_ssn		 
	GROUP BY t.essn,e.fname,e.lname;
	EXEC SQL OPEN emp_deps;
    EXEC SQL WHENEVER NOT FOUND DO BREAK;

	while (SQLCODE==0)
	{	
		EXEC SQL FETCH IN emp_deps INTO :fname,:lname, :no_of_deps;
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
	EXEC SQL CLOSE emp_deps;

    /*Define cursor to retrieve average salary of each department and hence find the difference 
    between this and employee salary*/
    EXEC SQL DECLARE emp_dept_sal CURSOR FOR
    SELECT avg(e.salary) 
    from sg0130.employee e
    WHERE dno=:dnum
    GROUP BY dno 
    HAVING count(e.ssn)>=1;
    EXEC SQL OPEN emp_dept_sal;
    EXEC SQL WHENEVER NOT FOUND DO BREAK;
    
    while(SQLCODE==0)
    {
        EXEC SQL FETCH in emp_dept_sal INTO :dept_avg_salary;
        diff_salary=abs(emp_salary-dept_avg_salary);
        printf("5.Department %d Avg Salary:$%.2f, The employee %s %s has difference salary of $%.2f\n\n",dnum,dept_avg_salary,
        fname,lname,diff_salary);
    }
    EXEC SQL CLOSE emp_dept_sal;
}

# embedded-sql-in-c

A program written in C which interacts with the postgresql DB and provides the required output. 
The code assumes the [Employee schema](https://images.app.goo.gl/QMkDT5EQ7mYVBRvP). The code either adds or subtracts employee working hours from the CLI without actually running the sql queries.  
Command to run the code:

```
ecpg filename.x
cc filename.c –o outputfilename –I/usr/include/postgresql –L/usr/lib –lecpg
./outputfilename
```
The arguements that need to be passed are:
```
./outputfilename –add –ssn 123456789 –pno 1 –hours 5
./outputfilename –subtract –ssn 123456789 –pno 1 –hours 5
```
Additinally the code also displays information about the project numbers and hours the employee is working on. Provides the name of his/her department, total hours worked by the employee, the number of his/her dependents, his/her salary, and the difference between his/her salary and average salary in his/her department.

If the employee is not working on the project, a new tuple for this employee will be added to Works_On relation. Employee cannot work for 0 or negative hours, so the corresponding tuple will be deleted. While subtracting the hours if the employee does not work on the corresponding project appropriate output will be displayed. 

Valid CLI arguements need to be passed such as SSN, pno.

#include<stdio.h>
#include"sqlite3/sqlite3.h"


typedef struct Student
{
	int id;
	char name[30];
	char dept[30];
	double Russian;
	double math;
	double english;
}Student;


const char* student_header()
{
	return "id¡¢name¡¢Course¡¢Russian  score¡¢Math scores¡¢English results";
}

const char* student_toSql(Student* stu)
{
	static char buf[BUFSIZ] = { 0 };
	sqlite3_snprintf(BUFSIZ, buf, "INSERT INTO students(id,name,dept,Russian ,math,english)\
		 VALUES(%d,'%s','%s',%lf,%lf,%lf)", stu->id, stu->name, stu->dept, stu->Russian, stu->math, stu->english);
	return buf;
}


//int (*callback)(void*,int,char**,char**)

int printTable(void* data, int columns, char** values, char** fileds)
{

	printf("data£º%d\n", *(int*)data);

	for (size_t i = 0; i < columns; i++)
	{
		printf("%s  ", fileds[i]);
	}
	printf("\n");

	for (size_t i = 0; i < columns; i++)
	{
		printf("%s  ", values[i]);
	}
	printf("\n");
	return SQLITE_OK;
}

int main()
{
	int rc = SQLITE_ERROR;
	sqlite3* db = NULL;
	rc = sqlite3_open("student.db", &db);
	if (rc == SQLITE_ERROR) 
	{
		sqlite3_log(sqlite3_errcode(db), "open failed\n");
		return -1;
	}
	//Create data table
	rc = sqlite3_exec(db, "CREATE TABLE students(id INT PRIMARY KEY,name VARCHAR(30),dept VARCHAR(30), Russian  REAL,\
 math REAL,english REAL)", NULL, NULL, NULL);
	if (rc == SQLITE_OK) {
		printf("The students table was created successfully~\n");
	}
	else
	{
		printf("Failed to create the students table£º%s\n", sqlite3_errmsg(db));
	}
	//Insert data
	rc = sqlite3_exec(db, "INSERT INTO students(id,name,dept,Russian ,math,english)\
		VALUES(001, 'Anna' ,'math', '3', '4', '5'),(002, 'Jerry' ,'Russian ', '4', '4', '3')", NULL, NULL, NULL);

	if (rc != SQLITE_OK)
	{
		printf("insert into failed:%s\n", sqlite3_errmsg(db));

	}

	//Insert data after user input
	printf("Please enter student information(%s)\n", student_header());
	Student stu;
	scanf("%d %s %s %lf %lf %lf", &stu.id, stu.name, stu.dept, &stu.Russian, &stu.math, &stu.english);
	rc = sqlite3_exec(db, student_toSql(&stu), NULL, NULL, NULL);
	if (rc != SQLITE_OK)
	{
		printf("insert into failed:%s\n", sqlite3_errmsg(db));
	}


	//Query data
	int number = 1;
	rc = sqlite3_exec(db, "SELECT * FROM students", printTable, &number, NULL);
	if (rc == SQLITE_OK)
	{
		printf("Query Successful£¡\n");
	}
	else
	{
		printf("Failed query£º%s\n", sqlite3_errmsg(db));
	}

	//change the data
	rc = sqlite3_exec(db, "UPDATE students SET name='Coco' WHERE id=001", NULL, NULL, NULL);
	if (rc != SQLITE_OK)
	{
		printf("Modification Failure£º%s\n", sqlite3_errmsg(db));
	}

	//delete data table
	rc = sqlite3_exec(db, "DELETE FROM students  WHERE id=100", NULL, NULL, NULL);
	if (rc != SQLITE_OK)
	{
		printf("Delete Failed£º%s\n", sqlite3_errmsg(db));
	}

	//Output data again
	char** result = NULL;
	int rowCount, columnCount;
	char* errMsg;
	rc = sqlite3_get_table(db,			//The database on which the SQL executes
		"SELECT * FROM students",		//The SQL to be executed
		&result,		                //Write the result table here
		&rowCount,                      //Write the number of rows in the result here
		&columnCount,                   //Write the number of columns of result here
		&errMsg                         //Write error messages here
	);
	if (rc != SQLITE_OK)
	{
		printf("%s\n", sqlite3_errmsg(db));
	}
	else
	{
		for (size_t r = 0; r <= rowCount; r++)
		{
			for (size_t c = 0; c < columnCount; c++)
			{
				printf("%s  ", result[r * columnCount + c]);
			}
			printf("\n");
		}
	}

	sqlite3_close(db);
	return 0;
}
#include "string.h"
class Employee {
	private:
		int id; // ID of the employee
		string Name; //Name of the employee
		int Department; //Department of the employee
		int position; //position of the employee
		int hourlyRate; //hourly rate of the employee
		int hoursWorked; // number of hours worked by the employee
	public:
		Employee(); //Employee Constructor
		~Employee(){} //Employee Destructor

		int getId(); //Return Employee ID
		void setId(int Id); //Set employee ID

		string getName(); //return employee name
		void setName(string n); //set employee name

		int getDept(); //return employee department
		void setDept(int dpt); //set employee department

		int getpos(); //return employee position
		void setpos(int pos); //set employee position

		int gethrate(); //return employee hourly rate
		void sethrate(int hr); // set employee hourly rate

		int getpayCheck(); // return individual employee pay check
		int getHoursWorked(); // retrun number of hours worked by the employee

};
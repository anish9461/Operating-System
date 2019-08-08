#include "kernel.h"
#include "main.h"
#include "thread.h"
#include "list.h"
#include "string.h"
#include "employee.h"
#include <fstream>
#include <cstdlib>

//----------< Initialize global Employee ID >-----------
int idMax = 8001;
int order;
int compare(Employee *a, Employee *b);
void readFromFile();
void enterNewRecord();
void displayEmployees(Employee *e);
void SearchEmployeebyID(int id);
void printDeptEmployees(int dept);
void UpdateDetails();
void DeleteEmployee();
int comparePay(Employee *a, Employee *b);
void printShowCheck(Employee *e);
void ShowCheck();
void saveToFile();
void selectMenu();
void company(int i);
void myFunction();


//----------< Compare function to sort Employees by ascending order of their departments and then by position >------------------
int compare(Employee *a, Employee *b)
{
	//comparing the departments of two employee objects and sorting them in increasing order
	if (a->getDept() > b->getDept())
		return 1;
	else if (a->getDept() < b->getDept())
		return -1;
	else
	{
		//If departments are same, sorting in increasing oreder of employee postions
		if (a->getpos() > b->getpos())
			return 1;
		else if (a->getpos() < b->getpos())
		{
			return -1;
		}
		else
			return 0;
	}
}

//------------< List of Employee Records sorted according to compare function >----------------
List<Employee*> *eList = new SortedList<Employee*>(compare);

//-------------< Reading Employee records from the existing file >-------------
void readFromFile()
{
	//flushing the output stream
	cout<<std::flush;
	//reading the employee.bat file from threads folder
	ifstream inFile("../threads/employee.dat");
	char str[500];
	//text gives the substrings in string datatype
	string text;
	const char *test;
	int i;
	int count;
while (inFile.getline(str, 255, '\n')) {
	//creating pointer to the object
		Employee *e = new Employee();
		i = 0;
		//count gives the substrings positions
		count = 0;
		//run this loop until last character is null
		while (!(str[i-1] == '\0'))
		{
			//get the Substrings and concatenate to the string
			while (!((str[i] == '\0') || (isspace(str[i]))))
			{
				if (count == 0)
					text = text + str[i];
				else if(count == 1)
					text = text + str[i];
				else if (count == 2)
					text = text + str[i];
				else if (count == 3)
					text = text + str[i];
				else if (count == 4)
				{
					text = text + str[i];
				}
				i++;
			}
			//Set the Employee ID 
			if (count == 0)
			{
				test = text.c_str();
				int tempid = atoi(test);
				e->setId(tempid);
				if(idMax <= tempid)
					idMax = tempid + 1;	
			}
			//Set the name 
			else if (count == 1)
			{
				e->setName(text);
			}
			//Set the department
			else if (count == 2)
			{
				test = text.c_str();
				e->setDept(atoi(test));
			}
			//Set the position 
			else if (count == 3)
			{
				test = text.c_str();
				e->setpos(atoi(test));
			}
			//Set the hourly rate
			else if (count == 4)
			{
				test = text.c_str();
				e->sethrate(atoi(test));
			}
			i++;
			count++;
			text = "";
		}
		//Append the employee object pointer to the list
		eList->Append(e);
	}
}

//-------------< Function to enter new Employee Record : Select option 1 in Menu >------------
void enterNewRecord()
{
	//create employee object pointer
	Employee *e = new Employee();
	e->setId(idMax);
	idMax++;
	//Set the employee name
	cout << "Enter Employee Name" << endl;
	string name;
	cin >> name;
	e->setName(name);
	//Set the employee position
	cout << "Enter Position" << endl;
	int position;
	cin >> position;
	e->setpos(position);
	//set the employee department
	cout << "Enter Department" << endl;
	int dept;
	cin >> dept;
	e->setDept(dept);
	//set the employee hourly rate
	cout << "Enter hourly rate" << endl;
	int hr;
	cin >> hr;
	e->sethrate(hr);
	//Append the employee object pointer to the list
	eList->Append(e);
}

//-----------< Function to display All Employee Records >-----------------
void displayEmployees(Employee *e)
{
		cout << endl;
		cout << ">---------< Employee Record >----------<" << endl << endl;
		cout << "Employee ID: "<< e->getId() << endl;
		cout << "Employee Name: "<< e->getName() << endl;
		cout << "Employee Department: "<< e->getDept() << endl;
		cout << "Employee Position: "<< e->getpos() << endl;
		cout << "Employee hourly rate: "<< e->gethrate() << endl <<endl << endl;
}

//------------< Function to print employee record searched by ID >----------------------
void SearchEmployeebyID(int id)
{
Employee *e = new Employee();
//create List Iterator to iterate through the employee list
ListIterator<Employee*> *Ilist = new ListIterator<Employee*>(eList);
List<Employee*> *eListID = new SortedList<Employee*>(compare);
//Iterate through the list
while(!Ilist->IsDone())
	{
		//get the current item in the list
		e = Ilist->Item();
		//check if entered ID is equal to the employee ID from the list
		if(id == e->getId())
		{
			//append the employee object pointer to the list
			eListID->Append(e);
			//display the employee record using Apply function
			eListID->Apply(displayEmployees);
		return;
		}
		//point to next object in the list
		Ilist->Next();
	}
	
	cout << "Employee entry does not exist" << endl << endl;
}

//-----------------< Function to print employees in particular Department >-------------------
void printDeptEmployees(int dept)
{
//create employee object pointer
Employee *e = new Employee();
//create list iterator to iterate through the list
ListIterator<Employee*> *Ilist = new ListIterator<Employee*>(eList);
//create list for storing employees in the department
List<Employee*> *eListDept = new SortedList<Employee*>(compare);
//iterate through the list
while(!Ilist->IsDone())
	{
		//assign employee to current item in the list
		e = Ilist->Item();
		//Check if the entered department is present in the list
		if(dept == e->getDept())
		{
			//append the employee object pointer to the list
			eListDept->Append(e);
		}
		//point to next item in the list
		Ilist->Next();
	}
	//print the employees n the department using apply function
	eListDept->Apply(displayEmployees);
}

//-----------------< Function to search Employee by ID and Department >-------------- 
void SearchEmployee()
{
	int select;
	cout << "To Search:\n  Select 1 to search by ID \n  Select 2 to search by Department" << endl;
	cin >> select;
	cout << endl;
	//switch statement to search employee by ID or department
	switch(select)
	{
		case 1:
			{
				int id;
				cout << "Enter Employee ID" << endl;
				cin >> id;
				//function to search employee by ID 
				SearchEmployeebyID(id);
				break;
			}				
		case 2:
			{
				int dept;
				cout << "Enter Department number" << endl;
				cin >> dept;
				//function to search employee by department
				printDeptEmployees(dept);	
			break;
			}
		default:
			cout << "Invalid Entry" << endl << endl;
	}	
}

//-----------------< Function to update the existing Employee Record >------------------
void UpdateDetails()
{
	//create employee object pointer 
	Employee *e = new Employee();
	//create list iterator to iterate through the list
	ListIterator<Employee*> *Ilist = new ListIterator<Employee*>(eList);
	int id = 0;
	cout << "Enter Employee ID" << endl;
	cin >> id;
	cout << endl;
	//iterate through the list to get the employee ID
	while(!Ilist->IsDone())
	{
		e = Ilist->Item();
		if(id == e->getId())
			break;
		Ilist->Next();
	}
	//Check if ID exists
	if(id != 0)
	{
		cout << "Employee Entry does not exist" << endl << endl;
		return;
	}
	int select;
	//update the employee details
	while(select != 5)
	{
		cout << "1. Update Name\n2. Update Department\n3. Update Position\n4. Update hourly rate\n5. Exit\n Select Option" << endl;
		cin >> select;
		cout << endl;
		switch(select){
			case 1: {
					cout << "Enter New Name" << endl;
					string name;
					cin >> name;
					//set employee new name
					e->setName(name);
					break;
			}
			case 2:{
				cout << "Enter New Department" << endl;
				int dept;
				cin >> dept;
				//set employee new department
				e->setDept(dept);
				break;
			}
			case 3:{
				cout << "Enter New position" << endl;
				int pos;
				cin >> pos;
				//set employee new position
				e->setpos(pos);
				break;
			}
			case 4:{
				cout << "Enter New hourly rate" << endl;
				int hrate;
				cin >> hrate;
				//set employee new hour rate
				e->sethrate(hrate);
				break;
			}
			case 5:{
				break;
			}
			default:
				cout << "Invalid Selection" << endl;			
		}

	}
	//create employee temporary list
	List<Employee*> *eListtemp = new SortedList<Employee*>(compare);
	//create employee list iterator to iterate through the list
	ListIterator<Employee*> *Ilisttemp = new ListIterator<Employee*>(eList);
	//iterate through the list to sort the list after updating
	while(!Ilisttemp->IsDone())
	{
		//get the current item from the list
		e = Ilisttemp->Item();
		//append the employee object pointer to the list
		eListtemp->Append(e);
		//get the next item from the list
		Ilisttemp->Next();
	}
	eList = eListtemp;
}

//-----------------< Function to Delete existing employee record from the list >------------------
void DeleteEmployee()
{
	//Employee ID to be deleted
	int id;
	string check;
	cout << "Enter the employee ID" <<endl;
	cin >> id;
	//Ask for confirmation for deletion
	cout << "Confirm Y/N" << endl;
	cin >> check;
	//create Employee object pointer
	Employee *e = new Employee();
//create list iterator to iterate through the list
ListIterator<Employee*> *Ilist = new ListIterator<Employee*>(eList);
//Delete the employee object if "Y"
if(check == "Y")
{
	//Iterate through the list
while(!Ilist->IsDone())
	{
		//current item in the list
		e = Ilist->Item();
		//check if entered ID is equal to the ID in the list
		if(id == e->getId())
		{   
			//Remove employee object pointer from the list
		   eList->Remove(e);
		   //delete the employee object pointer
		   delete e;
		   idMax--;
		   break;
		}
		//point to next item in the list
		Ilist->Next();
	}
}
else{
	cout << endl;
	cout << "Employee Record Not Deleted" << endl;
}
}

//-------------< Function to compare pay of employees for sorting employees according to individual paycheck >--------------
int comparePay(Employee *a, Employee *b)
{
	//sort in descending order of the individual employee paycheck
	if (a->getpayCheck() < b->getpayCheck())
		return 1;
	else if (a->getpayCheck() > b->getpayCheck())
		return -1;
	else
		return 0;
}

//----------------< Display the Order#, employee ID, working hours and paycheck amount >-------------------
void printShowCheck(Employee *e)
{
		cout << ">----------< Employee >-----------<" << endl << endl;
		cout << "Order #"<< order << endl;
		cout << "Employee ID: "<< e->getId() << " || Employee Name: " << e->getName() << endl;
		cout << "Working hours: " << e->getHoursWorked() << endl;
		cout << "PayCheck Amount: " << e->getpayCheck() << endl << endl;
		order++;
}
//---------------< Function to find and display Individual Employee PayCheck and TotalPayAmount >--------------
void ShowCheck()
{
	order = 1;
	int totalPayCheck = 0;
	//create new employee object pointer
	Employee *e = new Employee();
	//create sorted list of paycheck amount
	List<Employee*> *ePayList = new SortedList<Employee*>(comparePay);
	//create list iterator to iterate through the list
	ListIterator<Employee*> *Ilist = new ListIterator<Employee*>(eList);
	//iterate through the list
	while(!Ilist->IsDone())
	{
		//current item in the list
		e = Ilist->Item();
		//append employee object pointer to the list
		ePayList->Append(e);
		//point to net item in the list
		Ilist->Next();
	}
	//create list iterator for paycheck list
	ListIterator<Employee*> *IlistPay = new ListIterator<Employee*>(ePayList);
	//iterate through the list
	while(!IlistPay->IsDone())
	{
		//current item in the list
		e = IlistPay->Item();
		//append employee object pointer to the list
		totalPayCheck = totalPayCheck + e->getpayCheck();
		//point to net item in the list
		IlistPay->Next();
	}
	//display the paycheck amount in decreasing order of paycheck
	ePayList->Apply(printShowCheck);
	cout << endl;
	cout << "Total Amount of PayCheck is: " << totalPayCheck << endl << endl;

}

//--------------< Function to save all the Employee Records back to employee.bat file >------------
void saveToFile()
{
	cout << "Saving Employee Data to employee.dat file" << endl;
	//create ofstream 
	ofstream myfile;
	//open the employee file
	myfile.open("../threads/employee.dat");
	//create the employee object pointer
	Employee *e = new Employee();
	//create list iterator to iterate through the list
	ListIterator<Employee*> *Ilist = new ListIterator<Employee*>(eList);
	//iterate through the list
	while(!Ilist->IsDone())
	{
		//current item in the list
		e = Ilist->Item();
		//output the employee record to the employee.dat file
		myfile << e->getId() << " " << e->getName() << " " << e->getDept() << " " << e->getpos() << " " << e->gethrate() << endl;
		//point to next item in the list
		Ilist->Next();
	}
	//close the file
	myfile.close();
}

//-----------------< Function to select different Menu options for Employee record >-----------
void selectMenu()
{
	//option to select the different employee functions
	int option=0;
	while (option != 7)
	{
		cout << "Select 1 to enter new record" << endl;
		cout << "Select 2 to display all the Employees" << endl;
		cout << "Select 3 to search an Employee" << endl;
		cout << "Select 4 to Update employee details" << endl;
		cout << "Select 5 to Delete an Employee information" << endl;
		cout << "Select 6 to get the weekly personal paychecks and total paycheck amount" << endl;
		cout << "Select 7 to quit the program" << endl << endl;
		cout << "Select any one option" << endl;
		cin >> option;
		//switch statement select menu
		switch (option) {
			//function to enter new record
		case 1: enterNewRecord();
			break;
			//function to display all the employee records
		case 2: eList->Apply(displayEmployees);
			break;
			//function to search employee record
		case 3: SearchEmployee();	
			break;
			//function to update employe details
		case 4: UpdateDetails();
			break;
			//function to delete employee record
		case 5: DeleteEmployee();
			break;
			//function to display individual paychecks and total paycheck amount
		case 6: ShowCheck();
			break;
			//function to save to the file
		case 7: saveToFile();
			exit(0);
		}	
	}
}

//--------< Company function that reads from file first and then let user choose any menu option from selectMenu() function >-----------------
void company(int i)
{
	//function to read from the file
     readFromFile();
	 //fucntion to select menu
     selectMenu();
}

//------------< Defined function that creates child thread for executing program >--------------
void myFunction()
{
	//create a new thread and fork it to company
Thread *t = new Thread("forked thread");
t -> Fork((VoidFunctionPtr) company, (void *) 1);
}






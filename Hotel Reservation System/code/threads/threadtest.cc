#include "kernel.h"
#include "main.h"
#include "thread.h"
#include "list.h"
#include "bitmap.h"
#include "string.h"
#include "guest.h"
#include <fstream>
#include <cstdlib>

int compareID(guest *a, guest *b);
void guestRequest();
void printDiscarded(guest *g);
void printCheckInGuestInfo(guest *g);
void printCheckOutGuest(guest *g);
void grantedRequests();
void discardedRequests();
void totalRequestsGenerated();
void displayOccupancy(int stime);
void worker();
bool getAvailableRooms(guest *g);
void clearBitMap(guest *g);
void selectRooms(guest *g);
void setBitMap(guest *g);
void clearBitMapHeap();
void guestRequest();
void myFunction();
void clearCheckOutList();
void clearDiscardedList();


//----------<Initialize simulated Time  >-----------
int simulatedTime = 1;
int uID = 8001; //unique ID for the guest
//initializing bit map
Bitmap*  *rooms = new Bitmap*[11]; 

int compareID(guest *a, guest *b)
{
	if(a->getId() > b->getId())
		return 1;
	else if(a->getId() < b->getId())
		return -1;
	else 
		return 0;
}

//creating list of threads
List<Thread*> *stayingList = new List<Thread*>(); // list of threads for staying guests
List<Thread*> *confirmedList = new List<Thread*>(); // list of threads for confirmed guests
List<guest*> *discardedList = new SortedList<guest*>(compareID); //list of guest requests discarded
List<guest*> *checkOutList = new SortedList<guest*>(compareID); //list of guest requests checked out

//displaying discarded list of guests
void printDiscarded(guest *g)
{
	cout << "Guest ID : " << g->getId() << endl;
	cout << endl << endl;
}

//displaying list of guests checked In
void printCheckInGuestInfo(guest *g)
{
	cout << "Guest : " << g->getId() << endl;
	cout << "Check In Date : " << g->getCheckInDate() << endl;
	cout << "Check Out Date : " << g->getCheckOutDate() << endl;
	cout << "Number of Rooms : " << g->getNumberRooms() << endl;
	cout << "Rooms Occupied : ";
	ListIterator<int> roomListIterator = g->getRooms(); //get the list of room numbers
	while(!roomListIterator.IsDone())
	{
		cout << (roomListIterator.Item()) << " ";
		roomListIterator.Next();
	}
	cout << endl << endl;
}

//displaying list of guests that checked out
void printCheckOutGuest(guest *g)
{
	cout << " >------------------< Guest Checked Out >-----------------<" << endl;
	cout << "Guest ID : " << g->getId() << endl;
	cout << "Check Out Date : " << g->getCheckOutDate() << endl;
	cout << "Rooms Vacated : ";
	ListIterator<int> roomListIterator = g->getRooms();
	while(!roomListIterator.IsDone())
	{
		cout << (roomListIterator.Item()) << " ";
		roomListIterator.Next();
	}
	cout << endl << endl;
	if(g->getCheckOutDate() < 11)
		cout << "Number of available rooms : " << rooms[g->getCheckOutDate()]->NumClear();
	else
		cout << "Number of available rooms : " << rooms[11]->NumClear();
	cout << endl << endl;
}

//displaying list of granted requests
void grantedRequests()
{
	//checkout list iterator
	ListIterator<guest*> *checkoutListIterator = new ListIterator<guest*>(checkOutList);
	cout << " >--------------------------< Granted Requests >----------------------------<" << endl << endl;
	guest *g = new guest();
	int x = 1;
	while(!checkoutListIterator->IsDone())
	{
		g = checkoutListIterator->Item();
		cout << ">------------------< Granted Request " << x << " >--------------------< " << endl << endl;
		cout << "Guest : " << g->getId() << endl;
		cout << "Check In Date : " << g->getCheckInDate() << endl;
		cout << "Check Out Date : " << g->getCheckOutDate() << endl;
		cout << "Number of Rooms : " << g->getNumberRooms() << endl;
		cout << "Rooms stayed : ";
		ListIterator<int> roomListIterator = g->getRooms(); //get the list of rooms
		//display room numbers
		while(!roomListIterator.IsDone())
		{
			cout << (roomListIterator.Item()) << " ";
			roomListIterator.Next();
		}
		cout << endl << endl;
		x++;
		checkoutListIterator->Next();
	}
	
}

//displaying list of discarded requests
void discardedRequests()
{
	//discarded list iterator
	ListIterator<guest*> *discardedListIterator = new ListIterator<guest*>(discardedList);
	guest *g = new guest();
	int x = 1;
	while(!discardedListIterator->IsDone())
	{
		g = discardedListIterator->Item();
		cout << ">------------------< Discarded Request " << x << " >--------------------< " << endl << endl;
		cout << "Guest : " << g->getId() << endl;
		cout << "Check In Date : " << g->getCheckInDate() << endl;
		cout << "Check Out Date : " << g->getCheckOutDate() << endl;
		cout << "Number of Rooms Needed : " << g->getNumberRooms() << endl;
		cout << endl;
		x++;
		discardedListIterator->Next();
	}
	cout << endl << endl;

}


//displaying total requests generated in simulation
void totalRequestsGenerated()
{
	//list iterators for checkout and discarded list
	ListIterator<guest*> *checkoutListIterator = new ListIterator<guest*>(checkOutList);
	ListIterator<guest*> *discardedListIterator = new ListIterator<guest*>(discardedList);
	cout << " >--------------------------< Total requests Generated >----------------------------<" << endl << endl;
	guest *g = new guest();
	int x = 1;
	while(!checkoutListIterator->IsDone())
	{
		g = checkoutListIterator->Item();
		cout << ">------------------< Request " << x << " >--------------------< " << endl << endl;
		cout << "Guest : " << g->getId() << endl;
		cout << "Check In Date : " << g->getCheckInDate() << endl;
		cout << "Check Out Date : " << g->getCheckOutDate() << endl;
		cout << "Number of Rooms : " << g->getNumberRooms() << endl;
		cout << "Rooms stayed : ";
		ListIterator<int> roomListIterator = g->getRooms();
		//display room numbers
		while(!roomListIterator.IsDone())
		{
			cout << (roomListIterator.Item()) << " ";
			roomListIterator.Next();
		}
		cout << endl << endl;
		x++;
		checkoutListIterator->Next();
	}
	//display discarded guest requests
	while(!discardedListIterator->IsDone())
	{
		g = discardedListIterator->Item();
		cout << ">------------------< Request " << x << " >--------------------< " << endl << endl;
		cout << "Guest : " << g->getId() << endl;
		cout << "Check In Date : " << g->getCheckInDate() << endl;
		cout << "Check Out Date : " << g->getCheckOutDate() << endl;
		cout << "Number of Rooms : " << g->getNumberRooms() << endl;
		cout << endl << endl;
		x++;
		discardedListIterator->Next();
	}
	cout << endl << endl;
}

//clearing Checkout list
void clearCheckOutList()
{
	ListIterator<guest*> *checkoutListIterator = new ListIterator<guest*>(checkOutList);
	guest *g = new guest();
	int count = 0;
	//deep deleting guest from destructor
	while(!checkoutListIterator->IsDone())
	{
		g = checkoutListIterator->Item();
		delete g;
		count++;
		checkoutListIterator->Next();
	}
	//removing guest object pointer from the checkout list
	while(count != 0)
	{
		checkOutList->RemoveFront();
		count--;
	}

}

//clearing discarded list
void clearDiscardedList()
{
	//list iterator for discarded list
	ListIterator<guest*> *discardedListIterator = new ListIterator<guest*>(discardedList);
	guest *g = new guest();
	int count = 0;
	while(!discardedListIterator->IsDone())
	{
		g = discardedListIterator->Item();
		delete g;
		count++;
		discardedListIterator->Next();
	}

	while(count != 0)
	{
		discardedList->RemoveFront();
		count--;
	}
}

//display occupancy rate of rooms each day at the end of the day
void displayOccupancy(int stime)
{
	cout << endl;
	//number of available rooms on current date at the end of the day
	float x = rooms[stime]->NumClear();
	cout << "====================================================================="<<endl;
	cout << "Occupancy rate on Day: " << stime << " is " << ((30 - x)/30)*100 << "%" << endl;
	cout << "====================================================================="<<endl << endl;
}



//returns "true" if there are available rooms otherwise "false"
bool getAvailableRooms(guest *g)
{
	ListIterator<int> roomListIteratorempty = g->getRooms(); //To check if the assigned rooms are empty
	if(roomListIteratorempty.IsDone())
		return false;
	else
		return true;
}

//function to clear the bitmap
void clearBitMap(guest *g)
{
	//iterating through the number of days
	
	for(int j = 0; j <= g->getNumNights();j++)
	{
			//list iterator for the number of rooms
			ListIterator<int> roomListIteratorclear = g->getRooms();
			//clear all rooms on Day 11
			if(g->getCheckOutDate() > 11 && (simulatedTime == 11))
			{
				while(!roomListIteratorclear.IsDone())
				{
					rooms[11]->Clear(roomListIteratorclear.Item() - 1);	
					roomListIteratorclear.Next();	
				}
			}
				//clearing rooms on other days
				while(!roomListIteratorclear.IsDone())
				{
					if((g->getCheckInDate() + j) <= 11)
						rooms[g->getCheckInDate() + j]->Clear(roomListIteratorclear.Item() - 1);	
					roomListIteratorclear.Next();	
				}
			
	}
	
}

//function to select the availabe rooms
void selectRooms(guest *g)
{
	//temporary variables 
	int count = 0; 
	int countRoom = 0;
	//check if there are available rooms
	if(rooms[g->getCheckInDate()]->NumClear() > g->getNumberRooms())
	{
			if((g->getCheckInDate()) < 11)
			{
				int i = 0;
				//iterate through all the rooms and check which are empty
				while(i < 30)
				{
					count = 0;
					//iterate through the number of days and check if that particular room is empty
					for(int j = 0; j < g->getNumNights(); j++)
					{
						if((g->getCheckInDate() + j) < 12)
						{
							if(rooms[g->getCheckInDate() + j]->Test(i) == TRUE)
								continue;
							count++;
						}
					}
					//mark the room in the bitmap if its empty for all the days
					if(count == g->getNumNights())
					{
						rooms[g->getCheckInDate()]->Mark(i);
						g->assignedRooms->Append(i + 1);
						countRoom++;
					}
					//logic to mark the rooms in the bitmap when checkout date exceeds 11
					if(count == (12 - g->getCheckInDate()))
					{
						if(rooms[g->getCheckInDate()]->Test(i) == FALSE)
						{
							
							rooms[g->getCheckInDate()]->Mark(i);
							g->assignedRooms->Append(i + 1);
							countRoom++;
						}
					}

					if(countRoom == g->getNumberRooms())
						break;
					i++;
				}
				//if rooms not availabe, remove them from the list
				if(countRoom < g->getNumberRooms() && countRoom > 0)
				{
					clearBitMap(g);
					while(countRoom != 0)
					{
						g->assignedRooms->RemoveFront();
						countRoom--;					
					}
				}
			}
	}
}


//setting the bitmap
void setBitMap(guest *g)
{
	for(int j = 1; j <= g->getNumNights();j++)
	{
		//iterate through the rooms to mark the particular index in the bitmap
		ListIterator<int> roomListIteratorset = g->getRooms();
		while(!roomListIteratorset.IsDone())
		{
			if((g->getCheckInDate() + j) < 12)
			{
				rooms[g->getCheckInDate() + j]->Mark(roomListIteratorset.Item() - 1);
			}		
			roomListIteratorset.Next();
		}		
	}
}

//deleting bitmap from heap
void clearBitMapHeap()
{
	for(int i=1; i <= 11;i++)
	{
		delete rooms[i];
	}
}

//starting concierge thread
void worker()
{

//initializing bitmap
for(int i=1; i <= 11; i++)
	{
		rooms[i] = new Bitmap(30);
	}
cout << endl;
cout << ">===========================================< Simulating Hotel Reservation System >=============================================<" << endl << endl;
//simulating each day
	while(simulatedTime < 11)
	{

		//confirm list iteration
        ListIterator<Thread*> *confirmedListIterator = new ListIterator<Thread*>(confirmedList);
		while(!confirmedListIterator->IsDone())
		{
			Thread *t = confirmedListIterator->Item(); //"t" pointing to current thread
			kernel->scheduler->ReadyToRun(t); // putting the thread 't' to ReadyToRun state
			confirmedListIterator->Next();
		}

		//iterating staying list
		ListIterator<Thread*> *stayingListIterator = new ListIterator<Thread*>(stayingList);
		while(!stayingListIterator->IsDone())
		{
			Thread *t = stayingListIterator->Item(); //"t" pointing to current thread
			 kernel->scheduler->ReadyToRun(t); // putting the thread 't' to ReadyToRun state
			 stayingListIterator->Next(); 
		}

		cout << ">=====================================================< Day : " << simulatedTime << " >===================================================<"<<endl << endl;
		for(int i = 0; i<5;i++)
		{
			Thread *t = new Thread("guest request");
			t->Fork((VoidFunctionPtr) guestRequest, (void *) 1);
		}
		

		kernel->currentThread->Yield(); //yielding concierge thread

		displayOccupancy(simulatedTime); //displaying occupancy rate

	 	simulatedTime++; //incrementing the days
	 }
	 cout << endl << endl;
	 cout << ">======================================================= Day : " << simulatedTime << " >==============================================================<"<<endl;
	//checking out remaining guests on Day 11
	ListIterator<Thread*> *stayingListIteratorRemaining = new ListIterator<Thread*>(stayingList);
		while(!stayingListIteratorRemaining->IsDone())
		{
			Thread *t = stayingListIteratorRemaining->Item();
			kernel->scheduler->ReadyToRun(t);
			stayingListIteratorRemaining->Next();
		}
	kernel->currentThread->Yield();
	float x = rooms[simulatedTime]->NumClear(); //occupancy rate on day 11
	cout << "====================================================================="<<endl;
	cout << "Occupancy rate on Day: " << simulatedTime << " is " << ((30 - x)/30)*100 << "%" << endl;
	cout << "====================================================================="<<endl;
	cout << endl;


	cout << endl << endl;
	cout << ">===================================================< Summary of people who stayed >==================================================< " << endl << endl;
	

	//total requests generated
	totalRequestsGenerated();
	
	//total granted requests
	grantedRequests();

	//discarded requests
	discardedRequests();

cout << ">==============================================< End Simulation >====================================================< " <<endl;

	//clearing check out list
	clearCheckOutList();

	//clearing discarded list
	clearDiscardedList();

	//deleting bitmap
	clearBitMapHeap();

	//finishing the concierge thread
	kernel->currentThread->Finish();
}


void guestRequest()
{
	guest *g = new guest(); //creating guest object pointer
	g->setId(uID); //setting ID to the guest 
	uID++;
	g->setCheckInDate(simulatedTime); //setting checkin date
	g->setNumberRooms(); //setting number of rooms
	g->setNights(); //setting number of nights
	g->setCheckOutDate(g->getCheckInDate() + g->getNumNights()); //setting checkout date
	selectRooms(g); //selecting available rooms
	bool isRoomAvailable = getAvailableRooms(g); //check if rooms are available

//put the guest to discarded list when rooms are not available
if(!isRoomAvailable)
{
	cout << ">--------------------------------------------------< Request Discarded >---------------------------------------------------<" << endl << endl;
	cout << "Rooms Not available" << endl;
	printDiscarded(g);
	discardedList->Append(g);
	kernel->currentThread->Finish();
}

//else check if guest wants to check in or confirm for future date
else
{
	//check if current date is equal to check in date
	if(g->getCheckInDate() == simulatedTime)
	{
		//dispaly check in information of the guest		
		cout << ">-------------------------------------------------< Reservation made!!! >-------------------------------------------------------<" << endl << endl;
		cout << "Guest Checking In " << endl;
		printCheckInGuestInfo(g);
		cout << endl;
		//set the bitmap
		setBitMap(g);
		//display available rooms
		cout << "Number of availabe rooms : " << rooms[g->getCheckInDate()]->NumClear() << endl;
		//append the current thread to staying list
		stayingList->Append(kernel->currentThread);
		//sleep the thread
		kernel->currentThread->Sleep(FALSE);
		//put the thread to sleep again if check out date is not equal to current date
		while(g->getCheckOutDate() != simulatedTime)
		{
			if(g->getCheckOutDate() > 11 && (simulatedTime == 11))
				break;
			kernel->currentThread->Sleep(FALSE);
		}

		//adding guest to check out list
		checkOutList->Append(g);
		
		
	//clear the bit map
	clearBitMap(g);

		//display checkout list
		printCheckOutGuest(g);
	//remove the thread from the staying list
	stayingList->Remove(kernel->currentThread);
	//finish the thread
	kernel->currentThread->Finish();

	}
	//put guest to confirmed list
	else
	{
		//display confirm guest information
		cout << ">-----------------------------------< Reservation made for future date : " << g->getCheckInDate() << " >--------------------------------------<" << endl << endl;
	
		printCheckInGuestInfo(g);
		setBitMap(g);
		//available rooms
		cout << "Number of availabe rooms : " << rooms[g->getCheckInDate()]->NumClear() << endl;
		
		//append the thread to confirmed list and put the thread to sleep
		confirmedList->Append(kernel->currentThread);
	
		
		kernel->currentThread->Sleep(FALSE);
		
		//until checkin date is not equal to current date, put back the thread to sleep
		while(g->getCheckInDate() != simulatedTime)
		{
			kernel->currentThread->Sleep(FALSE);
		}

		//transfer this thread to staying list thread
		stayingList->Append(kernel->currentThread);
		confirmedList->Remove(kernel->currentThread);

		//put the thread to sleep again if check in date is not equal to current date
		while(g->getCheckInDate() != simulatedTime)
		{
			kernel->currentThread->Sleep(FALSE);
		}
		//display check in guest information
		cout << endl;
		cout << ">------------------------------------------------------< Check In >--------------------------------------------------<"<< endl << endl;
		cout << "Guest Checking In " << endl;
		printCheckInGuestInfo(g);

		//put the thread to sleep again if check out date is not equal to current date
		while(g->getCheckOutDate() != simulatedTime)
		{
			if(g->getCheckOutDate() > 11 && (simulatedTime == 11))
			 	break;
			kernel->currentThread->Sleep(FALSE);
		}

		//adding guest to check out list
		checkOutList->Append(g);
		
		//clear the bitmap
		clearBitMap(g);

		//display checkout list
		printCheckOutGuest(g);

		//remove the thread from the staying list
		stayingList->Remove(kernel->currentThread);
		//finish the thread
		kernel->currentThread->Finish();
	}

}

}


//------------< Defined function that creates child thread for executing program >--------------
void myFunction()
{
	//create a new thread and fork it to worker
	Thread *t = new Thread("Conceirge thread");
	t -> Fork((VoidFunctionPtr) worker, (void *) 1);
}

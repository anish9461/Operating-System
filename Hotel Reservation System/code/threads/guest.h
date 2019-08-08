#include "string.h"
#include "list.h"
class guest {
	private:
		int id; // ID of the guest
		int rooms; //number of rooms occupied by the guest
		int NumNights; //number of nights stayed by the guest
		int checkInDate; //check in date of the guest
		int checkOutDate; //check out date of the guest
		int numberOfNights; //number of nights stayed by the guest
	public:
		guest(); //Guest Constructor
		~guest(){
			delete assignedRooms;
		} //Guest Destructor

		void setId(int Id); //Set guest ID
		int getId(); //Return guest ID
		
		List<int> *getRooms(); //return number of rooms
		List<int> *assignedRooms = new List<int>(); // list of room numbers
		
		void setCheckInDate(int cInDate); //set checkin date of the guest
		int getCheckInDate(); //get check in date of the guest
		
		void setCheckOutDate(int cOutDate); //set check out date of the guest
		int getCheckOutDate(); //get checkout date of the guest
		
		void setNumberRooms(); //set number of rooms for the guest
		int getNumberRooms(); //get number of rooms of the guest

		void setNights(); //set number of nights for the guest
		int getNumNights(); //get number of nights for the guest
};
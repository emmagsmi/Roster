//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Program:  This program manages a roster for a CS class. It allows the user
//           to look up a student in the roster by last name or student ID,
//           add a student record to the roster, show the class roster in
//           alphabetic order by last name or by student ID, and finally
//           update the roster with any changes made.
// Process:  This program reads the roster from a file, processes user commands
//           in a loop with multiple functions, and updates the file when 
//           the user quits the program. All input is case insensitive and
//           validated.
// Results:  If the user selects 'L', they can look up a student by either their
//           last name or ID number. If the user selects 'A', they can add a
//           student to the roster by inputting their last name, first name,
//           ID, classification, and GPA. The user cannot add more than 35 
//           students. If the user selects 'S', the roster will be displayed on 
//           the console in alphabetical order by last name. Finally, if the user
//           selects 'Q', the program will terminate and the file will be updated
//           with any changes.
//
// Class:    CS2010
// Section:  1005
// Term:     Fall 2025
// Author:   Emma Smith
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;

// global constants
const int CLASS_CAP = 35;
const string INPUT = "pgm7.txt";
const int COL_WIDTH = 20;

// File I/O function previews
int readRoster(string name[CLASS_CAP][2], int id[CLASS_CAP], string classification[], double gpa[CLASS_CAP]);
int updateRoster(int cnt, string name[CLASS_CAP][2], int id[CLASS_CAP], string classification[], double gpa[CLASS_CAP]);

// Getter function previews
char getUserChoiceLASQ();
char getUserChoiceLIB();
int getId();
string getClassification();
double getGpa();

// Utility function previews
int checkForDuplicateName(string newFirst, int pos, int cnt, string name[CLASS_CAP][2]);
string formatName(string n);
void printHeaderRow();
void printOneRec(int index, string name[CLASS_CAP][2], int id[CLASS_CAP], string classification[], double gpa[CLASS_CAP]);
void swapRecords(int index, int min, string name[CLASS_CAP][2], int id[CLASS_CAP], string classification[], double gpa[CLASS_CAP]);

// Searching and sorting function previews
void sortByLastName(int cnt, string name[CLASS_CAP][2], int id[CLASS_CAP], string classification[], double gpa[CLASS_CAP]);
void sortById(int cnt, string name[CLASS_CAP][2], int id[CLASS_CAP], string classification[], double gpa[CLASS_CAP]);
int search(string key, int cnt, string name[CLASS_CAP][2], int id[CLASS_CAP], string classification[], double gpa[CLASS_CAP]);
int search(int key, int cnt, string name[CLASS_CAP][2], int id[CLASS_CAP], string classification[], double gpa[CLASS_CAP]);

// Processing function previews
int doAction(char choice, int cnt, string name[CLASS_CAP][2], int id[CLASS_CAP], string classification[], double gpa[CLASS_CAP]);
void lookUp(int cnt, string name[CLASS_CAP][2], int id[CLASS_CAP], string classification[], double gpa[CLASS_CAP]);
int addStudent(int cnt, string name[CLASS_CAP][2], int id[CLASS_CAP], string classification[], double gpa[CLASS_CAP]);
void showRoster(int cnt, string name[CLASS_CAP][2], int id[CLASS_CAP], string classification[], double gpa[CLASS_CAP]);

int main() {
	string name[CLASS_CAP][2];
	int id[CLASS_CAP];
	string classification[CLASS_CAP];
	double gpa[CLASS_CAP];
	int classSize;
	char choice;

	classSize = readRoster(name, id, classification, gpa);  // reads roster from input file and gets the size of roster
	if (classSize == -1) { // if error or more than 35 records, terminate program
		return 0;
	}

	do {  // while user choice is not equal to 'Q', display menu to get user choice and redirect to appropriate action
		choice = getUserChoiceLASQ();
		if (choice != 'Q') {
			classSize = doAction(choice, classSize, name, id, classification, gpa);
		}
	} while (choice != 'Q');

	updateRoster(classSize, name, id, classification, gpa);  // updates the class roster

	return 0;
}

int readRoster(string name[CLASS_CAP][2], int id[CLASS_CAP], string classification[], double gpa[CLASS_CAP]) {
	// reads student records into parallel arrays and returns the total number of students in the roster
	ifstream roster;
	string line;
	int comma;      // position of comma
	int firstPos;   
	int secondPos; 
	int thirdPos;  
	int fourthPos;
	int cnt = 0;    // total # of students in roster

	roster.open(INPUT);  // opens input file

	if (!roster.is_open()) {  // checks if input file is open; outputs error message if file fails to open
		cout << "Error: File open failed." << endl;
		return -1;
	}

	getline(roster, line);  // gets the header line to skip

	while (!roster.eof() && cnt < CLASS_CAP) {  // gets each line of the file as long as it is under the class cap
		getline(roster, line);                  // gets line of data from file

		comma = line.find(",");                 // finds the position of the first comma
		name[cnt][0] = line.substr(0, comma);   // gets the student's last name and stores in array

		firstPos = comma + 1;
		while (firstPos < line.length() && isspace(line.at(firstPos))) {
			firstPos++;                         // skips spaces after comma
		}

		secondPos = line.find(" ", firstPos);   // finds the first space after the first name
		name[cnt][1] = line.substr(firstPos, secondPos - firstPos); // gets the student's first name and stores in array

		while (secondPos < line.length() && isspace(line.at(secondPos))) {
			secondPos++;                        // skips spaces after first name
		}

		thirdPos = line.find(" ", secondPos);   // finds position of space after id number

		id[cnt] = stoi(line.substr(secondPos, thirdPos - secondPos)); // gets the student's id number and stores in array

		while (thirdPos < line.length() && isspace(line.at(thirdPos))) {
			thirdPos++;                         // skips spaces after id number
		}

		fourthPos = line.find(" ", thirdPos);   // finds position of space after classification

		classification[cnt] = line.substr(thirdPos, fourthPos - thirdPos); // gets the student's classification and stores in array

		while (fourthPos < line.length() && isspace(line.at(fourthPos))) {
			fourthPos++;                        // skips spaces after classification
		}

		gpa[cnt] = stod(line.substr(fourthPos)); // gets the student's gpa and stores in array

		cnt++;                                  // increments the total number of students in the roster
	}
	if (cnt == CLASS_CAP) {  // if the total number of students in the roster hits the class cap, check if end of file
		if (getline(roster, line) && !line.empty()) { // if not end of file, close file and terminate program
			roster.close();
			return -1;
		}
		else {
			cout << "Alert: Class enrollment has reached the cap. You are limited to use L(ook up), S(how) and Q(uit) functions of the system." << endl;
		}
	}
	
	roster.close();  // closes the input file
	return cnt;      // returns the total number of students in the roster to main
}

int updateRoster(int cnt, string name[CLASS_CAP][2], int id[CLASS_CAP], string classification[], double gpa[CLASS_CAP]) {  
	// triggered when the user inputs 'Q' - sorts the student records by last name, then first name
	ofstream updatedRoster;
	int i;

	sortByLastName(cnt, name, id, classification, gpa);  // calls function to sort the roster by last name, then first name

	updatedRoster.open(INPUT);  // opens the file

	if (!updatedRoster.is_open()) {   // checks if file is open; outputs error message if file fails to open
		cout << "Error: File open failed." << endl;
		return -1;
	}

	updatedRoster << left << setw(COL_WIDTH) << "Last, First";
	updatedRoster << left << setw(COL_WIDTH) << "ID";
	updatedRoster << left << setw(COL_WIDTH) << "Classification";
	updatedRoster << left << setw(COL_WIDTH) << "GPA";
	updatedRoster << endl;

	for (i = 0; i < cnt; i++) {  // inputs the updated roster, sorted by last name, into the file
		updatedRoster << left << setw(COL_WIDTH) << (name[i][0] + ", " + name[i][1]);
		updatedRoster << left << setw(COL_WIDTH) << id[i];
		updatedRoster << left << setw(COL_WIDTH) << classification[i];
		updatedRoster << left << setw(COL_WIDTH) << fixed << setprecision(2) << gpa[i];

		if (i < cnt - 1) {  // makes sure there is no empty line at the end of the file
			updatedRoster << endl;
		}
	}
	updatedRoster.close();  // Closes the file
	return cnt;  // returns the total number of students in the roster to main
}

char getUserChoiceLASQ() {
	// displays the main menu and gets the user's choice
	char choice;

	cout << "What to do [ L(ook up), A(dd), S(how), Q(uit)]? ";  // displays main menu
	cin >> choice;                                               // gets user choice from input
	choice = toupper(choice);                                    // change user choice to uppercase if lowercase

	while (choice != 'L' && choice != 'A' && choice != 'S' && choice != 'Q') {
		// wile user input is invalid, output error message and prompt user to choose again
		cout << endl;
		cout << "Error: Invalid Input. Try again." << endl;
		cout << endl;
		cout << "What to do [ L(ook up), A(dd), S(how), Q(uit)]? ";
		cin >> choice;
		choice = toupper(choice);
	}
	return choice;  // returns user choice to main
}

char getUserChoiceLIB() {
	// displays a menu for users to select a lookup criterion and gets the user's choice
	char choice;

	cout << endl;
	cout << "Select lookup criterion." << endl;  // displays a menu for users to select a lookup criterion
	cout << "Type L to look up by last name, type I to look up by ID, type B to go back: ";
	cin >> choice;                               // gets user choice from input
	choice = toupper(choice);                    // change user choice to uppercase if lowercase

	while (choice != 'L' && choice != 'I' && choice != 'B') {
		// while user input is invalid, output error message and prompt user to choose again
		cout << endl;
		cout << "Error: Invalid Lookup Criterion. Try again." << endl;
		cout << endl;
		cout << "Select lookup criterion." << endl;
		cout << "Type L to look up by last name, type I to look up by ID, type B to go back: ";
		cin >> choice;
		choice = toupper(choice);
	}
	return choice;  // returns choice
}

int getId() {
	// gets the ID of student to be added
	int idToAdd;

	cout << "Enter the student's 6-digit ID or enter -1 to return to the main menu: ";  // prompts user to type the ID for the student to be added
	cin >> idToAdd;  // gets ID of student to be added from input

	if (idToAdd == -1) {  // if user input -1, then return to main menu
		cout << endl;
		return -1;
	}

	while (idToAdd < 100000 || idToAdd > 999999) {
		// while input is not a 6-digit number, output error message and prompt user to re-enter the ID # of the student to be added
		cout << "Error: Invalid Student ID. Try again." << endl;
		cout << endl;
		cout << "Enter the student's 6-digit ID or enter -1 to return to the main menu: ";
		cin >> idToAdd;
		if (idToAdd == -1) {
			return -1;
		}
	}
	return idToAdd;  // returns ID of student to be added
}

string getClassification() {
	// gets classification of the student to be added
	string classificationToAdd;
	int choice;

	cout << "Enter the student's classification. Type 1 for freshman, 2 for sophomore, 3 for junior, 4 for senior, or -1 to return to the main menu: ";  // prompts user to enter the corresponding classification of the student to be added
	cin >> choice;  // gets user choice from input

	if (choice == -1) {  // if user choice is -1, return to main menu
		return "-1";
	}

	while (choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != -1) {
		// while user choice is invalid, output error message and prompt user to try again
		cout << "Error: Invalid Classification. Try again." << endl;
		cout << endl;
		cout << "Enter the student's classification. Type 1 for freshman, 2 for sophomore, 3 for junior, 4 for senior, or -1 to return to the main menu: ";
		cin >> choice;
		if (choice == -1) {
			return "-1";
		}
	}

	if (choice == 1) {  // if user choice is 1, classification is freshman
		classificationToAdd = "Freshman";
	}

	if (choice == 2) {  // if user choice is 2, classification is sophomore
		classificationToAdd = "Sophomore";
	}

	if (choice == 3) {  // if user choice is 3, classification is junior
		classificationToAdd = "Junior";
	}

	if (choice == 4) {  // if user choice is 4, classification is senior
		classificationToAdd = "Senior";
	}
	return classificationToAdd;  // returns the classification of the student to be added
}

double getGpa() {
	// gets the gpa of the student to be added
	double gpaToAdd;

	cout << "Enter the student's GPA or -1 to return to the main menu: ";  // prompts user to enter the gpa of the student to be added
	cin >> gpaToAdd;  // gets gpa of student to be added from input

	if (gpaToAdd == -1) {  // if user input -1, return to the main menu
		return -1;
	}

	while (gpaToAdd < 0 || gpaToAdd > 4) {
		// while gpa of student to be added is not between 0 and 4, output error message and prompt user to try again
		cout << "Error: Invalid GPA. Try again." << endl;
		cout << endl;
		cout << "Enter the student's GPA or -1 to return to the main menu: ";
		cin >> gpaToAdd;
		if (gpaToAdd == -1) {
			return -1;
		}
	}
	return gpaToAdd;  // returns the gpa of the student to be added
}

int checkForDuplicateName(string newFirst, int pos, int cnt, string name[CLASS_CAP][2]) {
	// checks for duplicate names when user adds a new student to the roster
	string newLast = name[pos][0];
	int i;

	for (i = 0; i < cnt; i++) {  // goes through roster to see if the new last name and new first name match an existing student
		if (name[i][0] == newLast && name[i][1] == newFirst) {
			return i;  // if match is found, return index of duplicate student
		}
	}
	return -1;         // if match is not found, return -1
}

string formatName(string n) {
	// format any input so that the first letter of any name is capitalized
	n[0] = toupper(n[0]);

	return n;  // returns name with first letter capitalized
}

void printHeaderRow() {
	// displays header row for a console output
	cout << left << setw(COL_WIDTH) << "Last, First";  
	cout << setw(COL_WIDTH) << "ID";
	cout << setw(COL_WIDTH) << "Classification";
	cout << setw(COL_WIDTH) << "GPA";
}

void printOneRec(int index, string name[CLASS_CAP][2], int id[CLASS_CAP], string classification[], double gpa[CLASS_CAP]) {
	// prints exactly one student record from the roster based on the parameter index
	cout << left << setw(COL_WIDTH) << (name[index][0] + ", " + name[index][1]);
	cout << left << setw(COL_WIDTH) << id[index];
	cout << left << setw(COL_WIDTH) << classification[index];
	cout << left << setw(COL_WIDTH) << fixed << setprecision(2) << gpa[index];
	cout << endl;
}

void swapRecords(int index, int min, string name[CLASS_CAP][2], int id[CLASS_CAP], string classification[], double gpa[CLASS_CAP]) {
	// swaps the two student records indexed by index and index
	string tempLast = name[index][0];                  // temporarily stores last name at index
	string tempFirst = name[index][1];                 // temorarily stores first name at index
	int tempId = id[index];                            // temporarily stores id at index
	string tempClassification = classification[index]; // temporarily stores classification at index
	double tempGpa = gpa[index];                       // temporarily stores gpa at index

	name[index][0] = name[min][0];               // last name at index set equal to last name at min
	name[index][1] = name[min][1];               // first name at index set equal to first name at min
	id[index] = id[min];                         // id at index set equal to id at min
	classification[index] = classification[min]; // classification at index set equal to classification at min
	gpa[index] = gpa[min];                       // gpa at index set equal to gpa at min

	name[min][0] = tempLast;                  // last name at min set equal to last name at index
	name[min][1] = tempFirst;                 // first name at min set equal to first name at index
	id[min] = tempId;                         // id at min set equal to id at index
	classification[min] = tempClassification; // classification at min set equal to classification at index
	gpa[min] = tempGpa;                       // gpa at min set equal to gpa at index
}

void sortByLastName(int cnt, string name[CLASS_CAP][2], int id[CLASS_CAP], string classification[], double gpa[CLASS_CAP]) {
	// sorts the student records by their last name using a modified selection sort algorithm
	// if two students have the same last name, sort them by their first name
	int i;
	int j;
	int min;

	for (i = 0; i < cnt - 1; i++) {
		min = i;
		for (j = i + 1; j < cnt; j++) {
			if (name[j][0] < name[min][0] || (name[j][0] == name[min][0] && name[j][1] < name[min][1])) {
				min = j;
			}
		}
		swapRecords(i, min, name, id, classification, gpa);
	}
}

void sortById(int cnt, string name[CLASS_CAP][2], int id[CLASS_CAP], string classification[], double gpa[CLASS_CAP]) {
	// sorts the student records by their id number using insertion sort algorithm
	int i;
	int j;

	for (i = 1; i < cnt; i++) {
		j = i;
		while (j > 0 && id[j] < id[j - 1]) {
			swapRecords(j, j - 1, name, id, classification, gpa);
			j--;
		}
	}
}

int search(string key, int cnt, string name[CLASS_CAP][2], int id[CLASS_CAP], string classification[], double gpa[CLASS_CAP]) {
	// searches a student by their last name using a modified binary search algorithm
	// returns the index of the found record
	// if two students have the same last name, return the index of the first occurence
	int mid;
	int low;
	int high;
	int result = -1; // initialized to -1 so if result is not found, returns to main menu

	sortByLastName(cnt, name, id, classification, gpa); // makes sure roster is sorted by last name

	key = formatName(key);

	low = 0;
	high = cnt - 1;

	while (high >= low) {
		mid = (high + low) / 2;
		if (name[mid][0] < key) {
			low = mid + 1;
		}
		else if (name[mid][0] > key) {
			high = mid - 1;
		}
		else {
			result = mid;
			high = mid - 1; // continues to search left for the first occurrence
		}
	}
	return result; // returns result
}

int search(int key, int cnt, string name[CLASS_CAP][2], int id[CLASS_CAP], string classification[], double gpa[CLASS_CAP]) {
	// searches a student record by their id value using a modified binary search algorithm
	int mid;
	int low;
	int high;

	sortById(cnt, name, id, classification, gpa);  // calls function to sort students by their id #s

	low = 0;
	high = cnt - 1;

	while (high >= low) {
		mid = (high + low) / 2;
		if (id[mid] < key) {
			low = mid + 1;
		}
		else if (id[mid] > key) {
			high = mid - 1;
		}
		else {
			return mid;
		}
	}
	sortByLastName(cnt, name, id, classification, gpa);  // calls function to sort back to by last name
	return -1;  // if student is not found, return -1
}

int doAction(char choice, int cnt, string name[CLASS_CAP][2], int id[CLASS_CAP], string classification[], double gpa[CLASS_CAP]) {
	// redirects to the appropriate function based on choice
	if (choice == 'L') {         // if user choice is 'L' then call lookUp function
		lookUp(cnt, name, id, classification, gpa);
	}
	else if (choice == 'A') {    // if user choice is 'A' then call addStudent function
		if (cnt >= CLASS_CAP) {  // if class cap is reached, do not addStudent and output error message
			cout << "Error: Class enrollment has reached the cap." << endl;
		}
		else {
			cnt = addStudent(cnt, name, id, classification, gpa);
		}
	}
	else if (choice == 'S') {    // if user choice is 'S' then call showRoster function
		showRoster(cnt, name, id, classification, gpa);
	}
	return cnt;  // return total number of students in roster 
}

void lookUp(int cnt, string name[CLASS_CAP][2], int id[CLASS_CAP], string classification[], double gpa[CLASS_CAP]) {
	// prompts the user to enter a search criteria by calling the getUserChoiceLIB function
	char choice;
	string lastName;
	int index;
	int numFound;
	int i;
	int idNum;

	sortByLastName(cnt, name, id, classification, gpa);  // sorts roster by last name

	choice = getUserChoiceLIB();  // calls function to get user choice

	if (choice == 'B') {  // if user choice is 'B' then return to main menu
		cout << endl;
		return;
	}

	if (choice == 'L') { // if user choice is 'L' then search by student's last name
		cout << "Enter the last name: ";  // outputs prompt for user to enter the student's last name
		cin >> lastName;           // gets student's last name from input
		lastName = formatName(lastName);  // makes sure first letter of last name is capitalized
		index = search(lastName, cnt, name, id, classification, gpa);  // searches for the index of the student with the last name input by user
		if (index == -1) {  // if index is equal to -1, then student was not found, outputs message, and returns to main menu
			cout << endl;
			cout << "No record has been found." << endl;
			cout << endl;
			return;
		}
		cout << endl;
		printHeaderRow();  // prints header
		cout << endl;
		numFound = 0;
		for (i = index; i < cnt && name[i][0] == lastName; i++) {  // checks if any others students have the same last name
			printOneRec(i, name, id, classification, gpa);         // prints student record with last name
			numFound++;                                            // increments number of students found with last name
		}
		cout << numFound << " student record(s) were found." << endl;  // outputs the number of student records found with last name
		cout << endl;
	}

	if (choice == 'I') {  // if user choice is 'I' then search by student's id #
		idNum = getId();  // gets student's id #
		if (idNum == -1) {  // if student id # is -1, then return to main menu
			return;
		}

		index = search(idNum, cnt, name, id, classification, gpa);  // searches for the index of the student with the id input by user
		if (index == -1) {  // if the index is -1, then student was not found and outputs message
			cout << endl;
			cout << "No record has been found." << endl;
			cout << endl;
		}
		else {  // student is found
			cout << endl;
			printHeaderRow();                                   // outputs header row
			cout << endl;
			printOneRec(index, name, id, classification, gpa);  // outputs records of the student with the id input by user
			cout << "1 student record(s) were found." << endl;  // outputs number of student records found with the id input by user
			cout << endl;
			sortByLastName(cnt, name, id, classification, gpa); // sorts back to last name order
		}
	}
}

int addStudent(int cnt, string name[CLASS_CAP][2], int id[CLASS_CAP], string classification[], double gpa[CLASS_CAP]) {
	// prompts the user to add new student by last name, first name, student id, classification and gpa
	string newLast;
	string newFirst;
	int newId;
	string newClassification;
	double newGpa;
	int duplicate;

	cout << endl;
	cout << "Enter the student's last name or type ! to return to the main menu: ";  // prompts user to enter the last name of the student they wish to add
	cin >> newLast;  // gets last name of student user wishes to add
	if (newLast == "!") {   // if the last name is "!" then return to main menu
		cout << endl;
		return cnt;
	}
	newLast = formatName(newLast);  // makes sure first letter of last name is capitalized

	cout << "Enter the student's first name or type ! to return to the main menu: ";  // prompts user to enter the first name of the student they wish to add
	cin >> newFirst;  // gets first name of student user wishes to add
	if (newFirst == "!") {   // if the first name is "!" then return to main menu
		cout << endl;
		return cnt;
	}
	newFirst = formatName(newFirst);  // meakes sure first letter of first name is capitalized

	name[cnt][0] = newLast;  // temporarilty stores new last name in array
	name[cnt][1] = newFirst; // temporarily stores new first name in array
	duplicate = checkForDuplicateName(newFirst, cnt, cnt, name);  // checks to see if student already exists
	if (duplicate != -1) {  // if index of duplicate student is returned, then output message and record of student
		cout << endl;
		cout << newLast << ", " << newFirst << " is already enrolled." << endl;
		printOneRec(duplicate, name, id, classification, gpa);
		cout << endl;
		return cnt;
	}

	newId = getId();     // get id # of student user wishes to add
	if (newId == -1) {   // if id # is -1, then return to main menu
		return cnt;
	}

	newClassification = getClassification(); // get classification of student user wishes to add
	if (newClassification == "-1") {         // if classificatio is "-1" then return to main menu
		return cnt;
	}

	newGpa = getGpa();  // get gpa of student user wishes to add
	if (newGpa == -1) { // if gpa is -1 then return to main menu
		return cnt;
	}

	id[cnt] = newId;                         // adds id of new student into array
	classification[cnt] = newClassification; // adds classification of new student into array
	gpa[cnt] = newGpa;                       // adds gpa of new student into array

	cout << endl;                                  // skips a line
	cout << "Student added successfully." << endl; // output message
	cout << endl;

	sortByLastName(cnt, name, id, classification, gpa);
	return cnt + 1;  // return the new total number of students in roster
}

void showRoster(int cnt, string name[CLASS_CAP][2], int id[CLASS_CAP], string classification[], double gpa[CLASS_CAP]) {
	// displays the current student roster
	int i;

	sortByLastName(cnt, name, id, classification, gpa);  // sorts students by last name
	cout << endl;
	cout << "   ";
	printHeaderRow();                                    // prints header row
	cout << endl;

	for (i = 0; i < cnt; i++) {                          // prints every student record
		cout << setw(3) << left << (i + 1);
		printOneRec(i, name, id, classification, gpa);
	}
	cout << endl;
}
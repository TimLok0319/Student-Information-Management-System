#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>
#include <string>
#include <iomanip>
#include <cmath>
using namespace std;

//Global Variables
ifstream inFile;
int studNum = 0;
const int stud_max = 1000;
struct student {
	string name = {}, ID = {}, form = {};
	int markList[7] = {};
	bool found = false;
	int resultType = 0;
};
student studList[stud_max];

double totalMarkArr[4][7] = { { 0,0,0,0,0,0,0 } ,{ 0,0,0,0,0,0,0 } ,{ 0,0,0,0,0,0,0 } ,{ 0,0,0,0,0,0,0 } };
double avgArr[4][7];
int highestMarkArr[4][7];
string highestMarkStud[4][7];
string all_highestForm[7];
double stdevNumeratorArr[4][7];
double stdevArr[4][7];

const string studInfo[3] = { "ID","Name","form" }, subject[7] = { "BM","BI","BC","MATH","SCI","SEJ","GEO" };
const string cyan = "36";
const string brightRed = "91";
const string brightYellow = "93";
const string grey = "90";
const string brightMagenta = "94";
const string brightGreen = "92";

//Function Prototypes
bool idExistsInFile(const string& ID);
int getValidMarks(const string& subject);
bool isValidID(const string& ID);
bool isNumber(const string& str);
bool isValidName(string name);
void addStudent();
void displayCalc();
void calculate(double f1, double f2, double f3);
string askOption(string errorType);
string toUpperString(string& input);
string setcolour(string colourcode);
string resetcolour();
void printUI(int type);
int studNum_counter();
void studGroup();
void studDisplay();
void alphabeticSort();
string search();
string filter();
void searchFilterMenu();
int check(string id);
string inputstr(string type);
int inputnum();
string ask(string question);
int mark();
string form();
void save_file(string type);
void edit(string id);
void del(string id);
void edit_and_del();
bool fileExistOrEmpty();

////////////////////////////////////////////////*Main Function*////////////////////////////////////////////////
int main() {
	inFile.open("student.txt");
	string mainMenuOption;

	do {
		inFile.close();
		studNum_counter();
		studGroup();
		system("cls");
		cout << setcolour(cyan) << "===========================<<Main Menu>>=========================" << resetcolour() << endl;
		cout << "Welcome to the JKJT Student Info Manager.\nPlease choose from the following option" << endl << endl;
		cout << "1.) Add new student(s) ---------1" << endl;
		cout << "2.) Delete/Edit student(s) -----2" << endl;
		cout << "3.) Search/Filter student(s) ---3" << endl;
		cout << "4.) Student's Mark Report-------4" << endl;
		cout << "\nDisplay Student(s) List ---A" << endl;
		cout << setcolour(brightRed) << "Exit the program ---X" << resetcolour() << endl;
		cout << setcolour(cyan) << "=================================================================" << resetcolour() << endl;
		mainMenuOption = askOption("mainMenu");

		if (mainMenuOption == "1") {
			addStudent();
		}
		else if (mainMenuOption == "2") {
			while (fileExistOrEmpty()) {
				edit_and_del();
				break;
			}
		}
		else if (mainMenuOption == "3") {
			while (fileExistOrEmpty()) {
				searchFilterMenu();
				break;
			}
		}
		else if (mainMenuOption == "4") {
			while (fileExistOrEmpty()) {
				displayCalc();
				break;
			}
		}
		else if (mainMenuOption == "A") {
			while (fileExistOrEmpty()) {
				system("cls");
				studDisplay();
				cout << "*Total of " << studNum << " student(s) recorded." << endl;
				system("pause");
				break;
			}
		}
	} while (mainMenuOption != "X");
}

////////////////////////////////////////////////////*Function Defining*///////////////////////////////////////////////////////////////////

//---------------------------------------------------Universal Function---------------------------------------------
bool fileExistOrEmpty() {
	inFile.open("student.txt");
	if (inFile.is_open() && studNum != 0) {
		inFile.close();
		return true;
	}
	else {
		cout << setcolour(brightRed) << "\nPlease add students record into the system before edit/delete/search/filter/view student report!" << resetcolour() << endl;
		system("pause");
		inFile.close();
		return false;
	}
}

string setcolour(string colourCode) {
	return  "\033[" + colourCode + "m";

}

string resetcolour() {
	return "\033[0m";
}

string setColourSubStr(string text, string colourCode, int length, int start) {
	return  text.substr(0, start) + colourCode + text.substr(start, length) + "\033[0m" + text.substr(start + length);
}

void printUI(int type) {
	if (type == 1) {
		cout << setcolour(cyan) << "----------------------------------------------------------------------------------------------" << resetcolour() << endl;
	}
	else if (type == 2) {
		cout << setcolour(cyan) << "==============================================================================================" << resetcolour() << endl;
	}
	else if (type == 3) {
		cout << setw(13) << right << "Name" << setw(22) << right << "ID" << setw(11) << right << "Form" << setw(5) << right
			<< "BM" << setw(6) << right << "BI" << setw(6) << right << "BC" << setw(7) << right << "MATH" << setw(6) << right << "SCI"
			<< setw(6) << right << "SEJ" << setw(6) << right << "GEO" << endl;
	}
	else if (type == 4) {
		cout << setcolour(cyan) << "========================================" << resetcolour() << endl;
	}

}

int studNum_counter() {
	string line;
	inFile.open("student.txt");
	int counter = 0;


	if (!inFile.eof()) {
		while (getline(inFile, line))
			counter++;
	}

	studNum = counter / 4;
	inFile.close();
	return(studNum);
}

//------------------------------------------------------Student 1---------------------------------------------------
bool idExistsInFile(const string& ID)
{
	ifstream inFile("student.txt");
	string line;

	while (getline(inFile, line)) {
		if (line == ID) { // Check if this line matches the ID
			inFile.close();
			return true;  // ID found, return true
		}
		for (int i = 0; i < 3; ++i) { // Skip the next 3 lines (name, form, marks)
			getline(inFile, line);
		}
	}
	inFile.close();
	return false;  // ID not found, return false
}

bool isNumber(const string& str) {
	for (char const& c : str) {
		if (!isdigit(c)) {
			return false; //Form enter by user not digit, return false
		}
	}
	return true;
}

bool isValidName(string name)
{
	for (int i = 0; i < name.length(); i++) {
		if (!isalpha(name[i]) && !(name[i] == ' '))
			return false; //If name enter by user consist of digit, return false
	}
	return true;
}

bool isValidID(const string& ID) {
	//Data validation function (Check if the id enter by user is in correct form)
	if (ID.length() != 9) {
		cout << setcolour(brightRed) << "ID must be exactly 9 characters long." << resetcolour() << endl;
		return false;
	}

	if (!isdigit(ID[0]) || !isdigit(ID[1])) {
		cout << setcolour(brightRed) << "First two characters must be digits." << resetcolour() << endl;
		return false;
	}

	if (!isalpha(ID[2]) || !isalpha(ID[3]) || !isalpha(ID[4])) {
		cout << setcolour(brightRed) << "Characters 3 to 5 must be letters." << resetcolour() << endl;
		return false;
	}

	if (!isdigit(ID[5]) || !isdigit(ID[6]) || !isdigit(ID[7]) || !isdigit(ID[8])) {
		cout << setcolour(brightRed) << "Last four characters must be digits." << resetcolour() << endl;
		return false;
	}

	return true;
}

int getValidMarks(const string& subject) {
	//Check if the marks enter by user is an positive integer
	string input;
	int marks;
	cout << subject << ": ";
	while (true) {
		cin >> input;
		if (isNumber(input)) {
			marks = stoi(input);
			if (marks >= 0 && marks <= 100) {
				break;
			}
		}
		cout << setcolour(brightRed) << "Please enter valid marks (0-100)! " << resetcolour() << endl;
		cout << subject << ": ";
	}
	return marks;
}

void addStudent() {
	system("cls");
	string name, ID, form;
	int BM, BI, BC, MATH, SCI, SEJ, GEO;
	string option;
	ofstream wfile;

	do {
		wfile.open("student.txt", ios::app);
		system("cls");
		cout << setcolour(cyan) << "============Add New Student============" << resetcolour() << endl;
		cout << " Example of Student's Record: " << setcolour(grey) << "\n(Please follow the given format)\n\n" << resetcolour() << "Student ID : 23ABC1234 " << setcolour(grey) << "(2Digits3Chars4Digits)" << resetcolour() << "\nName : Peter Griffin\nForm : 1 " << setcolour(grey) << "(Only form 1-3 are allowed)" << resetcolour() << "\nBM : 100\nBC : 100\nBI : 100\nMATH : 100\nSCI : 100\nSEJ : 100\nGEO : 100\n";
		printUI(4);
		cout << setcolour(brightYellow) << "Please enter student information and marks\n" << resetcolour();
		bool validID = false;
		do {
			cout << "Student ID (Eg: 12ACD1234): ";
			getline(cin, ID);

			// Convert to uppercase
			for (char& c : ID) {
				c = toupper(c);
			}

			// Check if the ID format is valid
			validID = isValidID(ID);

			// Check if the ID already exists
			if (validID && idExistsInFile(ID)) {
				cout << setcolour(brightRed) << "This ID already exists! Please enter a different ID." << resetcolour() << endl;
				validID = false;
			}

		} while (!validID);

		bool validName = false;
		while (!validName) {
			cout << "Student name: ";
			getline(cin, name);

			// Check if the name consist only alphabet
			if (isValidName(name))
				validName = true;
			else
				cout << setcolour(brightRed) << "Name should only consists of character!" << resetcolour() << endl;
		}

		// Validate form input
		bool validForm = false;
		do {
			cout << "Form: ";
			cin >> form;
			if (!isNumber(form) || (form != "1" && form != "2" && form != "3")) {
				cout << setcolour(brightRed) << "You can only enter 1/2/3! Please enter again." << resetcolour() << endl;
			}
			else {
				validForm = true;
			}
		} while (!validForm);

		BM = getValidMarks("BM");
		BI = getValidMarks("BI");
		BC = getValidMarks("BC");
		MATH = getValidMarks("Math");
		SCI = getValidMarks("Sci");
		SEJ = getValidMarks("Sejarah");
		GEO = getValidMarks("Geo");

		//Write data to file
		if (studNum == 0 || !fileExistOrEmpty())
			wfile << ID << endl;
		else
			wfile << endl << ID << endl;
		wfile << name << endl;
		wfile << form << endl;
		wfile << BM << " " << BI << " " << BC << " " << MATH << " " << SCI << " " << SEJ << " " << GEO;

		cout << "\nDo you want to key in another record? (Y/N) \n";
		cin.ignore();
		option = askOption("addMenu");
		cout << endl;
		wfile.close();
	} while (option == "Y");
}

void calculate(double f1, double f2, double f3) {

	for (int a = 0; a < 4; a++) {
		for (int b = 0; b < 7; b++) {
			totalMarkArr[a][b] = 0;
			avgArr[a][b] = 0;
			stdevArr[a][b] = 0;
			stdevNumeratorArr[a][b] = 0;
			highestMarkArr[a][b] = 0;
		}
	}

	for (int x = 0; x < studNum; x++) {
		if (studList[x].form == "1") {
			for (int i = 0; i < 7; i++) {
				//find highest mark
				if (f1 == 1) {
					highestMarkArr[0][i] = studList[x].markList[i];
					highestMarkStud[0][i] = studList[x].name;
				}
				else {
					if (highestMarkArr[0][i] < studList[x].markList[i]) {
						highestMarkArr[0][i] = studList[x].markList[i];
						highestMarkStud[0][i] = studList[x].name;
					}
				}
				//find average -- save in array
				totalMarkArr[0][i] += studList[x].markList[i]; //find total marks for each subject
				avgArr[0][i] = double(totalMarkArr[0][i]) / f1;
			}
		}
		else if (studList[x].form == "2") {
			for (int i = 0; i < 7; i++) {
				if (f2 == 1) {
					highestMarkArr[1][i] = studList[x].markList[i];
					highestMarkStud[1][i] = studList[x].name;
				}
				else {
					if (highestMarkArr[1][i] < studList[x].markList[i]) {
						highestMarkArr[1][i] = studList[x].markList[i];
						highestMarkStud[1][i] = studList[x].name;
					}
				}
				totalMarkArr[1][i] += studList[x].markList[i];
				avgArr[1][i] = double(totalMarkArr[1][i]) / f2;
			}
		}
		else if (studList[x].form == "3") {
			for (int i = 0; i < 7; i++) {
				if (f3 == 1) {
					highestMarkArr[2][i] = studList[x].markList[i];
					highestMarkStud[2][i] = studList[x].name;
				}
				else {
					if (highestMarkArr[2][i] < studList[x].markList[i]) {
						highestMarkArr[2][i] = studList[x].markList[i];
						highestMarkStud[2][i] = studList[x].name;
					}
				}
				//find average -- save in array
				totalMarkArr[2][i] += studList[x].markList[i]; //find total marks for each subject
				avgArr[2][i] = double(totalMarkArr[2][i]) / f3;
			}
		}

		for (int i = 0; i < 7; i++) {
			if (x == 0) {
				highestMarkArr[3][i] = studList[x].markList[i];
				highestMarkStud[3][i] = studList[x].name;
				all_highestForm[i] = studList[x].form;
			}
			else {
				if (highestMarkArr[3][i] < studList[x].markList[i]) {
					highestMarkArr[3][i] = studList[x].markList[i];
					highestMarkStud[3][i] = studList[x].name;
					all_highestForm[i] = studList[x].form;
				}
			}
			totalMarkArr[3][i] += studList[x].markList[i];
			avgArr[3][i] = double(totalMarkArr[3][i] / studNum); //find average for f1+f2+f3
		}
	}

	//stdev
	for (int x = 0; x < studNum; x++) {  //Find the numerator for standard deviation [Sum of (x - mean)^2]
		if (studList[x].form == "1") {
			for (int i = 0; i < 7; i++) {
				stdevNumeratorArr[0][i] += pow(studList[x].markList[i] - avgArr[0][i], 2);
			}
		}
		else if (studList[x].form == "2") {
			for (int i = 0; i < 7; i++) {
				stdevNumeratorArr[1][i] += pow(studList[x].markList[i] - avgArr[1][i], 2);
			}
		}
		else if (studList[x].form == "3") {
			for (int i = 0; i < 7; i++) {
				stdevNumeratorArr[2][i] += pow(studList[x].markList[i] - avgArr[2][i], 2);
			}
		}
		for (int i = 0; i < 7; i++) {
			stdevNumeratorArr[3][i] += pow(studList[x].markList[i] - avgArr[3][i], 2);
		}
	}

	for (int i = 0; i < 7; i++) { //Calculate standard deviation
		if (f1 > 1)
			stdevArr[0][i] = sqrt(stdevNumeratorArr[0][i] / f1);
		if (f2 > 1)
			stdevArr[1][i] = sqrt(stdevNumeratorArr[1][i] / f2);
		if (f3 > 1)
			stdevArr[2][i] = sqrt(stdevNumeratorArr[2][i] / f3);
		if (studNum > 1)
			stdevArr[3][i] = sqrt(stdevNumeratorArr[3][i] / studNum);
	}
}

void displayCalc()
{
	string p4view; //User option to choose which form's student report to generate
	string option = "Y";

	do {
		studNum_counter();
		double f1 = 0, f2 = 0, f3 = 0;
		for (int x = 0; x < studNum; x++) {
			if (studList[x].form == "1") {
				f1++; //total number of form 1 student
			}
			else if (studList[x].form == "2") {
				f2++;
			}
			else if (studList[x].form == "3") {
				f3++;
			}
		}
		system("cls");

		cout << setcolour(cyan) << "=====================Student Report======================" << resetcolour() << endl;
		cout << setcolour(brightYellow) << "Please choose the form that you wish to view [all/1/2/3]: " << resetcolour();
		p4view = askOption("p4view");
		cout << setcolour(cyan) << "=========================================================" << resetcolour() << endl;

		system("cls");
		cout << "---------------------------------------------" << endl;
		if (p4view == "ALL")
			cout << "Student Report for " << setcolour("44") << "All Form(s)" << resetcolour() << " had been generated." << endl;
		else
			cout << "Student Report for " << setcolour("44") << "Form " + p4view << resetcolour() << " had been generated." << endl;
		cout << endl;
		calculate(f1, f2, f3);
		cout << fixed << setprecision(2);
		if (p4view == "1") { //Form 1 student report
			for (int k = 0; k < 7; k++) {
				cout << setcolour(brightGreen) << subject[k] << endl;
				cout << "---" << resetcolour() << endl;
				cout << setcolour(brightYellow) << "Average: " << setcolour(cyan) << avgArr[0][k] << resetcolour();
				cout << setcolour(brightYellow) << "\nStandard Deviation: " << setcolour(cyan) << stdevArr[0][k] << resetcolour();
				cout << setcolour(brightYellow) << "\nHighest Score: " << setcolour(cyan) << highestMarkArr[0][k] << resetcolour();;
				cout << setcolour(brightYellow) << "\nStudent Name: " << setcolour(cyan) << highestMarkStud[0][k] << resetcolour() << endl << endl;
			}
		}

		else if (p4view == "2") { //Form 2 student report
			for (int k = 0; k < 7; k++) {
				cout << setcolour(brightGreen) << subject[k] << endl;
				cout << "---" << resetcolour() << endl;
				cout << setcolour(brightYellow) << "Average: " << setcolour(cyan) << avgArr[1][k] << resetcolour();
				cout << setcolour(brightYellow) << "\nStandard Deviation: " << setcolour(cyan) << stdevArr[1][k] << resetcolour();
				cout << setcolour(brightYellow) << "\nHighest Score: " << setcolour(cyan) << highestMarkArr[1][k] << resetcolour();;
				cout << setcolour(brightYellow) << "\nStudent Name: " << setcolour(cyan) << highestMarkStud[1][k] << resetcolour() << endl << endl;
			}
		}
		else if (p4view == "3") { //Form 3 student report
			for (int k = 0; k < 7; k++) {
				cout << setcolour(brightGreen) << subject[k] << endl;
				cout << "---" << resetcolour() << endl;
				cout << setcolour(brightYellow) << "Average: " << setcolour(cyan) << avgArr[2][k] << resetcolour();
				cout << setcolour(brightYellow) << "\nStandard Deviation: " << setcolour(cyan) << stdevArr[2][k] << resetcolour();
				cout << setcolour(brightYellow) << "\nHighest Score: " << setcolour(cyan) << highestMarkArr[2][k] << resetcolour();;
				cout << setcolour(brightYellow) << "\nStudent Name: " << setcolour(cyan) << highestMarkStud[2][k] << resetcolour() << endl << endl;
			}
		}

		else if (p4view == "ALL") { //All form student report
			for (int k = 0; k < 7; k++) {
				cout << setcolour(brightGreen) << subject[k] << endl;
				cout << "---" << resetcolour() << endl;
				cout << setcolour(brightYellow) << "Average: " << setcolour(cyan) << avgArr[3][k] << resetcolour();
				cout << setcolour(brightYellow) << "\nStandard Deviation: " << setcolour(cyan) << stdevArr[3][k] << resetcolour();
				cout << setcolour(brightYellow) << "\nHighest Score: " << setcolour(cyan) << highestMarkArr[3][k] << resetcolour();
				cout << setcolour(brightYellow) << "\nStudent Name: " << setcolour(cyan) << highestMarkStud[3][k] << " (Form " + all_highestForm[k] + ")" << resetcolour() << endl << endl;;
				//cout << setcolour(brightYellow) << "\nForm: " << setcolour(cyan) << all_highestForm[k] << resetcolour() << endl << endl;
			}
		}
		cout << "---------------------------------------------" << endl;
		cout << setcolour(brightYellow) << "Do you want to continue? (Y/N): " << resetcolour();
		option = askOption("addMenu");
	} while (option == "Y");
}

//------------------------------------------------------Student 2---------------------------------------------------
string askOption(string errorType) {
	// Data Validation function
	string option = {};
	cout << "\nEnter your Option: ";
	getline(cin, option);
	option = toUpperString(option);

	if (errorType == "snfMenu") {
		while (option != "1" && option != "2" && option != "A" && option != "N" && option != "X") {
			cout << setcolour(brightRed) << "Invalid option! Please Enter again." << resetcolour() << endl;
			cout << "Enter your Option: ";
			getline(cin, option);
			option = toUpperString(option);
		}
	}

	else if (errorType == "filterMenu1") {
		while (option != "1" && option != "2" && option != "3" && option != "4" && option != "5" && option != "6" &&
			option != "7" && option != "8" && option != "9" && option != "10" && option != "Y" && option != "N" && option != "X" && option != "C") {
			cout << setcolour(brightRed) << "Invalid option! Please Enter again." << resetcolour() << endl;
			cout << "Enter your Option: ";
			getline(cin, option);
			option = toUpperString(option);
		}
	}
	else if (errorType == "filterSubject") {
		while (option != "1" && option != "2") {
			cout << setcolour(brightRed) << "Invalid option! Please Enter again." << resetcolour() << endl;
			cout << "Enter your Option: ";
			getline(cin, option);
			option = toUpperString(option);
		}
	}
	else if (errorType == "filterMenu2") {
		while (option != "1" && option != "2" && option != "3" && option != "4" && option != "N" && option != "X") {
			cout << setcolour(brightRed) << "Invalid option! Please Enter again." << resetcolour() << endl;
			cout << "Enter your Option: ";
			getline(cin, option);
			option = toUpperString(option);
		}
	}
	else if (errorType == "mainMenu") {
		while (option != "1" && option != "2" && option != "3" && option != "4" && option != "X" && option != "A") {
			cout << setcolour(brightRed) << "Invalid option! Please Enter again." << resetcolour() << endl;
			cout << "Enter your Option: ";
			getline(cin, option);
			option = toUpperString(option);
		}
	}
	else if (errorType == "edMenu") {
		while (option != "1" && option != "2" && option != "X") {
			cout << setcolour(brightRed) << "Invalid option! Please Enter again." << resetcolour() << endl;
			cout << "Enter your Option: ";
			getline(cin, option);
			option = toUpperString(option);
		}
	}
	else if (errorType == "addMenu") {
		while (option != "Y" && option != "N") {
			cout << setcolour(brightRed) << "Invalid option! Please Enter again." << resetcolour() << endl;
			cout << "Enter your Option: ";
			getline(cin, option);
			option = toUpperString(option);
		}
	}
	else if (errorType == "p4view") {
		while (option != "1" && option != "2" && option != "3" && option != "ALL") {
			cout << setcolour(brightRed) << "Invalid option! Please Enter again." << resetcolour() << endl;
			cout << "Enter your Option: ";
			getline(cin, option);
			option = toUpperString(option);
		}
	}
	else if (errorType == "filterSM") {
		while (option != "1" && option != "2") {
			cout << setcolour(brightRed) << "Invalid option! Please Enter again." << resetcolour() << endl;
			cout << "Enter your Option: ";
			getline(cin, option);
			option = toUpperString(option);
		}
	}
	return option;
}

void alphabeticSort() {
	//Array to sort student name in the list from alphabetic order
	student temp[stud_max];
	for (int j = 0; j < studNum; j++) {
		for (int k = 0; k < (studNum - 1); k++) {

			if (studList[k].name.compare(studList[k + 1].name) == 1) {
				temp[k] = studList[k];
				studList[k] = studList[k + 1];
				studList[k + 1] = temp[k];
			}
		}
	}
}

void studDisplay() {
	//Function to display every recorded student in the file
	cout << setcolour(cyan) << "==========================================Student(s)==========================================" << resetcolour() << endl;
	printUI(3);
	printUI(2);

	for (int c = 0; c < studNum; c++) {
		cout << "  " << setw(3) << right << c + 1 << ") " << setw(22) << left << studList[c].name << setw(15) << left << studList[c].ID << setw(5) << left << studList[c].form;

		for (int i = 0; i < 7; i++) {
			cout << setw(6) << studList[c].markList[i];
		}
		cout << endl;
	}
	printUI(2);
	printUI(1);
}

void studGroup() {
	//Function to add every recorded student into the array of struct.
	studNum_counter();
	inFile.open("student.txt");
	for (int x = 0; x < studNum; x++) {
		getline(inFile, studList[x].ID);
		getline(inFile, studList[x].name);
		getline(inFile, studList[x].form);

		for (int j = 0; j < 7; j++) {
			inFile >> studList[x].markList[j];
		}
		inFile.ignore();
	}

	for (int x = 0; x < studNum; x++) {
		studList[x].name = toUpperString(studList[x].name); // Store every student's name as uppercase
	}
	alphabeticSort();
	inFile.close();
}

string toUpperString(string& input) {
	for (int i = 0; i < input.length(); i++) {
		input[i] = toupper(input[i]);
	}
	return input;
	//Convert string to uppercase
}

void toEmptyArr(student(&arrToEmpty)[stud_max]) {
	//Function to empty a array of struct
	student emptyArr[stud_max];
	for (int i = 0; i < stud_max; i++) {
		arrToEmpty[i] = emptyArr[i];
	}
}

void searchFilterMenu() {

	studGroup();
	string  menuOption;

	do {
		system("cls");
		cout << setcolour(cyan) << "===========Search/Filter Menu===========" << resetcolour() << endl;
		cout << "Please choose from the following option: \n1.) Search ---1\n2.) Filter ---2\n\nDisplayAllStudent ---A\nExit(toMainMenu) ---X\n" << endl;
		printUI(4);
		while (menuOption != "1" && menuOption != "2" && menuOption != "A" && menuOption != "X")
			menuOption = askOption("snfMenu");

		if (menuOption == "1") {
			menuOption = search();
		}
		else if (menuOption == "2") {
			menuOption = filter();
		}
		else if (menuOption == "A") {
			system("cls");
			studDisplay();
			cout << setcolour(brightYellow) << "\n>> Do you want to go to  << " << resetcolour() << "\nSearch ---1\nFilter ---2\nExit(toSearch/Filter Menu) ---N\nExit(toMain Menu) ---X\n";
			menuOption = askOption("snfmenu");
		}
	} while (menuOption != "X"); // X as sentinel to exit the function;
}

string search()
{
	int resultNum;
	string searchWord, menuOption;
	student searchResult[stud_max];

	do {
		system("cls");
		toEmptyArr(searchResult); //initialise an array that store student with matching searchwords
		resultNum = 0;
		for (int i = 0; i < studNum; i++) { //reinitialise each student's found flag to false to avoid skipped in search loop
			studList[i].found = false;
		}

		cout << setcolour(cyan) << "==============================Search==============================" << resetcolour();
		cout << setcolour(brightYellow) << "\nPlease key in the search word (Name,ID,Form): " << resetcolour();
		getline(cin, searchWord);
		toUpperString(searchWord);
		system("cls");
		cout << "Search Result for keyword: " << searchWord << endl;

		for (int i = 0; i < studNum; i++) { // Loop through all students in the array StudList
			if (studList[i].found == false) { // avoid multiple search on same student
				if (studList[i].name.find(searchWord) != string::npos) {// search for keyword in name of each  student
					resultNum++;
					searchResult[resultNum - 1] = studList[i];
					searchResult[resultNum - 1].resultType = 1;
					studList[i].found = true;
				}

				if (studList[i].ID.find(searchWord) != string::npos) {// search for keyword in ID of each student
					if (studList[i].found == true) // if the student have matching info in both name and ID
						searchResult[resultNum - 1].resultType = 3;
					else {
						resultNum++;
						searchResult[resultNum - 1] = studList[i];
						searchResult[resultNum - 1].resultType = 0;
						studList[i].found = true;
					}
				}

				if (studList[i].form.find(searchWord) != string::npos) { // proceed to copy student details if find() doesnot return string :: npos
					if (studList[i].found == true)// if the student have matching info in both name and ID
						searchResult[resultNum - 1].resultType = 4;

					else {
						resultNum++;
						searchResult[resultNum - 1] = studList[i]; // copy the student details into the array searchResult
						searchResult[resultNum - 1].resultType = 2;
						studList[i].found = true; // set found = true to avoid multiple search in next iteration
					}
				}
			}
		}
		if (resultNum > 0) { // only display the array searchResult if at least one student have the matching searchword
			printUI(2);
			printUI(3);
			printUI(2);
			for (int x = 0; x < resultNum; x++)
			{
				switch (searchResult[x].resultType) { // display coloured text for the substring of the information that matches the search word
				case 0:
					cout << "  " << setw(3) << right << x + 1 << ") " << setw(21) << left << searchResult[x].name
						<< setw(24) << left << setColourSubStr(searchResult[x].ID, "\033[92m", searchWord.length(), searchResult[x].ID.find(searchWord)) << setw(6) << left << searchResult[x].form;
					break;
				case 1:
					cout << "  " << setw(3) << right << x + 1 << ") " << setw(30) << left << setColourSubStr(searchResult[x].name, "\033[92m", searchWord.length(), searchResult[x].name.find(searchWord))
						<< setw(15) << left << searchResult[x].ID << setw(6) << left << searchResult[x].form;
					break;
				case 2:
					cout << "  " << setw(3) << right << x + 1 << ") " << setw(21) << left << searchResult[x].name
						<< setw(15) << left << searchResult[x].ID << setcolour(brightGreen) << setw(6) << left << searchResult[x].form << resetcolour();
					break;
				case 3:
					cout << "  " << setw(3) << right << x + 1 << ") " << setw(30) << left << setColourSubStr(searchResult[x].name, "\033[92m", searchWord.length(), searchResult[x].name.find(searchWord))
						<< setw(24) << left << setColourSubStr(searchResult[x].ID, "\033[92m", searchWord.length(), searchResult[x].ID.find(searchWord)) << setw(6) << left << searchResult[x].form;
					break;
				case 4:
					cout << "  " << setw(3) << right << x + 1 << ") " << setw(21) << left << searchResult[x].name
						<< setw(24) << left << setColourSubStr(searchResult[x].ID, "\033[92m", searchWord.length(), searchResult[x].ID.find(searchWord)) << setcolour(brightGreen) << setw(6) << left << searchResult[x].form << resetcolour();
					break;
				}


				for (int i = 0; i < 7; i++) {
					cout << setw(6) << searchResult[x].markList[i];
				}
				cout << endl;
			}
			printUI(2);
		}

		else {
			printUI(2);
			cout << setcolour(brightRed) << setw(50) << right << "(No result found!)" << resetcolour() << endl;
			printUI(2);
		}
		cout << "*Total of " << resultNum << " search result (s) found." << endl; // Display the number of results found

		cout << setcolour(brightYellow) << "\n>> Search Again ? << " << resetcolour() << "\nSearch ---1\nFilter ---2\nExit(toSearchEngineMenu) ---N\nExit(toMain Menu) ---X\n ";
		menuOption = askOption("snfMenu"); // Ask user for next operation and loop again if option != 'X'
	} while (menuOption == "1");

	return menuOption;
}

string filter() {

	system("cls");
	string filterOption;
	string f_ID = {}, f_form = {}, f_name = {}, menuOption = {};
	int f_markList[7] = { -1,-1,-1,-1,-1,-1,-1 }, f_subjectNum = 0, counter = 0, resultNum;
	string f_subject[7] = {};
	student filterList[stud_max];

	do { // reset the flag in struct array of each student to false to avoid skipping of student during filter matching
		for (int i = 0; i < studNum; i++)
			studList[i].found = false;
		resultNum = 0; // reset number of result found

		system("cls");
		cout << setcolour(cyan) << "=======================Filter===============================" << resetcolour() << endl;
		cout << setcolour(brightYellow) << "Please select in the information(s) that you want to filter " << resetcolour() << endl;
		cout << "--------------------------------------" << endl;
		cout << setw(13) << left << " ID   ---1 " << setw(13) << left << "BM   ---4 " << setw(13) << left << "MATH  ---7" << endl;
		cout << setw(13) << left << " Name ---2 " << setw(14) << left << "BI   ---5 " << setw(13) << left << "SCI  ---8" << endl;
		cout << setw(13) << left << " Form ---3" << setw(14) << left << "BC   ---6" << setw(13) << left << "SEJ  ---9" << endl;
		cout << setw(36) << right << "GEO ---10" << endl;
		cout << "--------------------------------------" << endl;

		// Display the selected filter and if nothing is selected, display blank
		cout << setcolour(brightYellow) << "\nSelected Filter List:" << resetcolour() << endl;
		cout << "---------------------" << endl;
		if (f_ID != "")
			cout << " ID: " << f_ID << endl;

		if (f_name != "")
			cout << " Name: " << f_name << endl;

		if (f_form != "")
			cout << " Form: " << f_form << endl;

		cout << "\n(By Subject)" << endl;
		for (int i = 0; i < 7; i++) {
			if (f_subject[i] == "")
				continue;
			else
				cout << " " << f_subject[i] << endl;
		}

		cout << "\n(By Marks)" << endl;
		for (int i = 0; i < 7; i++) {
			if (f_markList[i] != -1) {
				cout << " " << subject[i] + ": " << f_markList[i] << endl;
			}
		}

		cout << "---------------------\n" << endl;
		cout << "Clear All filter ---C" << endl;
		cout << setcolour("91") << "Exit(Search & Filter Menu) ---N" << endl;
		cout << "Exit(Main Menu) ---X" << resetcolour() << endl;
		cout << setcolour("92") << "Proceed to Filter ---Y" << resetcolour() << endl;
		cout << setcolour(cyan) << "============================================================\n" << resetcolour();

		filterOption = askOption("filterMenu1");

		if (filterOption != "N" && filterOption != "X" && filterOption != "C") {
			string markOrsubject = {}; // Variable to store user option

			if (filterOption == "1") { //Prompt user to input the value of chosen filter
				cout << "ID: ";
				getline(cin, f_ID);
				f_ID = toUpperString(f_ID);
			}
			else if (filterOption == "2") {
				cout << "Name: ";
				getline(cin, f_name);
				f_name = toUpperString(f_name);
			}
			else if (filterOption == "3") {
				cout << "Form: ";
				getline(cin, f_form);
			}
			//If user option is a subject, ask user whether to filter by mark or by whole subject
			else if (filterOption == "4" || filterOption == "5" || filterOption == "6" || filterOption == "7"
				|| filterOption == "8" || filterOption == "9" || filterOption == "10") {
				cout << "Filter by \n Mark ---1\n Subject ---2" << endl;
				markOrsubject = askOption("filterSM");
				if (markOrsubject == "1") { // if by mark, prompt user to enter the mark of the subject 
					cout << subject[stoi(filterOption) - 4] + ": "; // convert option of user from string to int and minus by 4 to index to the chosen subject
					cin >> f_markList[stoi(filterOption) - 4];
					//Example: in subject[0] = "BM". To choose BM in the previous menu,user need to enter "4". To index to the BM,which is 0,minus user option by 4 (4-4=0)
					cin.ignore();
				}
				else { // if by subject add the subject into the filtered subject array.
					f_subjectNum++;
					f_subject[stoi(filterOption) - 4] = subject[stoi(filterOption) - 4];
				}
			}
		}

		if (filterOption == "C") { // Clear all applied filter
			for (int i = 0; i < 7; i++) {
				f_markList[i] = -1;
				f_subject[i] = "";
				f_name = ""; f_ID = ""; f_form = "";
			}
		}

		if (filterOption == "Y") { //Proceed to filter
			for (int i = 0; i < studNum; i++) {
				counter = 0; // Every time the student info passes a condition. counter +1, hence if all condition are passed, counter = 7;
				if (!studList[i].found && studList[i].ID.find(f_ID) != string::npos) {
					if (studList[i].name.find(f_name) != string::npos) {//If no filter are added for any of the attribute, the students are considered pass for next condition checking
						if (studList[i].form.find(f_form) != string::npos) {
							for (int j = 0; j < 7; j++) {
								if (studList[i].markList[j] == f_markList[j] || f_markList[j] == -1) { // -1 are used to represent empty 
									counter++;
								}
							}
						}
					}
				}
				if (counter == 7) {
					resultNum++;
					filterList[resultNum - 1] = studList[i]; // If student passed all the condition, add the student into the result list
					studList[i].found = true;
				}
			}
			system("cls");
			cout << "Filtered List" << endl;
			int isEmpty = 0;
			for (int i = 0; i < 7; i++) {
				if (f_subject[i].empty())
					isEmpty++;
			} // variable isEmpty is to check whether user added at least one subject as filter
			if (resultNum > 0)
			{
				if (isEmpty == 7) { // Case 1: if no subject are selected, the program will display all subject marks of the student
					printUI(2);
					printUI(3);
					printUI(1);

					for (int i = 0; i < resultNum; i++) { // display the filtered student details
						cout << "  " << setw(3) << right << i + 1 << ") " << setw(21) << left << filterList[i].name << setw(16) << left << filterList[i].ID << setw(5) << left << filterList[i].form;

						for (int j = 0; j < 7; j++) {// display the filtered student's marks
							cout << setw(6) << left << filterList[i].markList[j];
						}
						cout << endl;
					}
				}
				else { // Case 2: else the program will only display subject that are selected.
					printUI(2);
					cout << setw(10) << left << " " << setw(23) << left << "Name" << setw(9) << left << "ID" << setw(9) << left << "Form";
					for (int j = 0; j < 7; j++) {
						if (f_subject[j] == subject[j]) // if f_subject[0] (BM) = subject[0](BM) as if BM is not selected, f_subject[0] will be " " (not equal to subject[0])
							cout << setw(8) << left << f_subject[j];// Display the label
					}
					cout << endl;
					printUI(2);
					for (int i = 0; i < resultNum; i++) {
						cout << "  " << setw(3) << right << i + 1 << ") " << setw(21) << left << filterList[i].name << setw(15) << left << filterList[i].ID << setw(8) << left << filterList[i].form;
						for (int j = 0; j < 7; j++) {
							if (f_subject[j] == subject[j])
								cout << setw(8) << left << filterList[i].markList[j]; // Display the mark
						}
						cout << endl;
					}
				}
				printUI(2);
			}

			else {
				printUI(2);
				cout << setcolour(brightRed) << setw(50) << right << "(No result found!)" << resetcolour() << endl;
				printUI(2);
			}

			cout << setcolour(brightYellow) << "\nFilter Applied:" << resetcolour() << endl;
			cout << "---------------------" << endl;

			if (f_ID != "") // Display the selected filter for clarification during checking
				cout << " ID: " << f_ID << endl;

			if (f_name != "")
				cout << " Name: " << f_name << endl;

			if (f_form != "")
				cout << " Form: " << f_form << endl;

			cout << "\n(By Subject)" << endl;
			for (int i = 0; i < 7; i++) {
				if (f_subject[i] == "")
					continue;
				else
					cout << " " << f_subject[i] << endl;
			}
			cout << "\n(By Marks)" << endl;
			for (int i = 0; i < 7; i++) {
				if (f_markList[i] != -1) {
					cout << " " << subject[i] + ": " << f_markList[i] << endl;
				}
			}
			cout << "---------------------" << endl;

			cout << setcolour(brightYellow) << "\n>> Filter again? << " << resetcolour() << endl;
			cout << setw(27) << left << "Clear All filter(s) ---1" << "Add Another Filter ---2" << "\nDisplay All student ---3"
				<< setw(26) << right << "Search ---4" << "\n\nExit to Search/Filter Menu ---N\nExit to Main Menu ---X" << endl;
			cout << "----------------------------------------------------";

			filterOption = askOption("filterMenu2");

			if (filterOption == "1") { // Reset the applied filter list
				for (int i = 0; i < 7; i++) {
					f_markList[i] = -1;
					f_subject[i] = "";
					f_name = ""; f_ID = ""; f_form = "";
				}
			}
			else if (filterOption == "3") {
				menuOption = "A";
			}
			else if (filterOption == "4") {
				menuOption = "1";
			}
			else { // if "2" was chosen, it will loop back to the previous menu without resetting the applied filter list
				menuOption = filterOption;
			}
		}
		else if (filterOption == "N" || filterOption == "X")
			menuOption = filterOption;

	} while (menuOption != "N" && menuOption != "X" && menuOption != "1" && menuOption != "A");

	return menuOption; // Return the menuOption so to the filer/searchMenu()

}
//------------------------------------------------------Student 3---------------------------------------------------
int check(string id) {
	//check id exist or not
	studGroup();
	for (int i = 0; i < studNum; i++) {
		//read studList and compare to user input id if id exist return student number in list else return no found message
		if (id == studList[i].ID) {

			cout << setcolour(brightYellow) << "\n>>Chosen Student<<" << resetcolour() << endl;
			cout << "Student ID: " << studList[i].ID << "\nName: " << studList[i].name << "\nForm: " << studList[i].form << endl;
			for (int j = 0; j < 7; j++)
			{
				cout << subject[j] + ": " << studList[i].markList[j] << endl;
			}
			cout << endl;
			return i;
		}
	}
	return -1;


}

string inputstr(string type) {
	//change input string to lower or upper

	string str;
	getline(cin, str);

	for (int i = 0; i < str.size(); i++) {
		if (type == "tolower") {
			//add 32 to accoring lower ascii value
			if (str[i] >= 'A' && str[i] <= 'Z') {
				str[i] += 32;
			}

		}
		else if (type == "toupper") {
			//minus 32 to accoring upper ascii value
			if (str[i] >= 'a' && str[i] <= 'z') {
				str[i] -= 32;
			}
		}
	}
	return str;

}

int inputnum() {
	//check input number is integer or not

	string num;
	int result = 0;
	getline(cin, num);

	for (int i = 0; i < num.size(); i++) {
		//read input number one by one, if input is number conver to corresponding number, else return invalid number message

		if (num[i] >= '0' && num[i] <= '9') {
			result = result * 10 + (num[i] - '0');
		}
		else {
			return -1;
		}

	}
	return result;
}

string ask(string question) {
	//ask user to continue or not

	string continue_option = "None";
	while (!(continue_option == "y" || continue_option == "n")) {
		cout << question;
		continue_option = inputstr("tolower");
		if (!(continue_option == "y" || continue_option == "n")) { cout << setcolour(brightRed) << "Invalid input,please enter again" << resetcolour() << endl; }
	}

	return continue_option;
}

int mark() {
	//check mark range

	int num;
	do {
		cout << "Please enter the mark: ";
		num = inputnum();
		if (!(num >= 0 && num <= 100)) {
			cout << setcolour(brightRed) << "Invalid input,please enter again, mark should between 0 to 100" << resetcolour() << endl;
		}
	} while (!(num >= 0 && num <= 100));
	return num;
}

string form() {
	//check form range

	string num;
	do {
		cout << "Please update the form: ";
		getline(cin, num);
		if (!(num == "1" || num == "2" || num == "3")) {
			cout << setcolour(brightRed) << "Invalid input,please enter again, form should be between 1 to 3" << resetcolour() << endl;
		}
	} while (!(num == "1" || num == "2" || num == "3"));
	return num;

}

void save_file(string type) {
	//save file for edit or delete

	int count = 0;
	ofstream f;
	f.open("student.txt");

	if (f.is_open()) {

		if (type == "edit") {
			//save file by according order

			count = 0;
			while (studList[count].form != "") {
				f << studList[count].ID << endl;
				f << studList[count].name << endl;
				f << studList[count].form << endl;
				f << studList[count].markList[0] << " " << studList[count].markList[1] << " " << studList[count].markList[2] << " " << studList[count].markList[3] << " " << studList[count].markList[4] << " " << studList[count].markList[5] << " " << studList[count].markList[6];
				count++;
				if (studList[count].form == "1" || studList[count].form == "2" || studList[count].form == "3") { f << endl; continue; }
			}
		}

		else if (type == "delete") {
			//save file by accoring order,  when student record is delete in studList it will no save

			count = 0;
			while ((studList[count].form != "") || (studList[count + 1].form != "")) {

				if (studList[count].form != "") {
					f << studList[count].ID << endl;
					f << studList[count].name << endl;
					f << studList[count].form << endl;
					f << studList[count].markList[0] << " " << studList[count].markList[1] << " " << studList[count].markList[2] << " " << studList[count].markList[3] << " " << studList[count].markList[4] << " " << studList[count].markList[5] << " " << studList[count].markList[6];
					if (studList[count].form != "" && (studList[count + 1].form != "" || studList[count + 2].form != "")) { f << endl; }
				}

				//clean old studList for not read old record
				studList[count].ID = "";
				studList[count].name = "";
				studList[count].form = "";
				studList[count].markList[0] = 0;
				studList[count].markList[1] = 0;
				studList[count].markList[2] = 0;
				studList[count].markList[3] = 0;
				studList[count].markList[4] = 0;
				studList[count].markList[5] = 0;
				studList[count].markList[6] = 0;
				count++;

			}
		}
		f.close();
	}
}

void edit(string id) {
	//edit 

	int count = -1; bool found = 0;
	string select, subject_option, continue_option;

	while (count == -1) {
		//read user input id and check 
		cout << setcolour(brightGreen) << "EDIT >> " << setcolour(brightYellow) << "Please key in the student id that you want to edit: " << resetcolour();
		count = check(inputstr("toupper"));
		if (count == -1) {
			cout << setcolour(brightRed) << "Student record not found! Please enter again" << resetcolour() << endl;
		}
	}

	do {
		//read user select edit which information

		select = "0";
		while (!(select == "1" || select == "2" || select == "3")) {
			cout << setcolour(brightYellow) << "Please select the attribute that you want to edit:\n(Name ---1   Form ---2   Marks ---3)\n\n" << resetcolour() << "User Input: ";
			getline(cin, select);
			if (!(select == "1" || select == "2" || select == "3")) { cout << setcolour(brightRed) << "Invalid input,please enter again" << resetcolour() << endl; }
		}

		if (select == "1") {
			cout << "Please update the name: ";
			getline(cin, studList[count].name);
		}


		else if (select == "2") {
			studList[count].form = form();
		}

		else if (select == "3") {

			subject_option = "None";
			found = 0;
			while (subject_option == "None") {
				//read user edit subject

				cout << "Please enter the subject that you want to change the mark(BM/BI/BC/MATH/SCI/SEJ/GEO): ";
				subject_option = inputstr("toupper");
				for (int i = 0; i < 7; i++) {
					if (subject_option == subject[i]) { studList[count].markList[i] = mark(); found = 1; }
				}
				if (!found) { cout << setcolour(brightRed) << "Invalid Input! Please enter again." << resetcolour() << endl; subject_option = "None"; continue; }
			}
		}

		continue_option = ask("Do you still have anything to edit for this student?(y/n): ");
	} while (continue_option == "y");

	save_file("edit");
}

void del(string id) {
	//delete

	int count = -1;
	while (count == -1) {
		//read user input id and check 

		cout << setcolour("94") << "DELETE >> " << setcolour(brightYellow) << "Please key in the student id that you want to  delete : " << resetcolour();
		count = check(inputstr("toupper"));
		if (count == -1) { cout << setcolour(brightRed) << "Student record not found! Please enter again" << resetcolour() << endl; continue; }
		cout << setcolour(brightRed) << "<<* The chosen Student's Record will be deleted *>>" << resetcolour() << endl << endl;
	}

	//let delete record equal to empty
	studList[count].form = "";
	save_file("delete");
}

void edit_and_del() {
	//read user choice for edit or delete

	string id, choice;
	string continue_option = "y", subject = "None";

	while (true) {

		//check record is empty or not
		if (studNum_counter() == 0) { cout << "No record found, Please add new student first" << endl; system("pause"); break; }

		//dispay menu and read user input for edit or delete
		system("cls");
		cout << setcolour(cyan) << "=============Edit/Delete Menu============" << resetcolour() << endl;
		cout << "Please choose from the following option: \n1.) Edit ---1\n2.) Delete ---2\n\nExit(toMainMenu) ---X\n" << endl;
		printUI(4);
		choice = askOption("edMenu");

		if (choice == "1") {
			//go to edit funtion

			do {
				//clean screen, get new studList and display studList
				system("cls");
				studGroup();
				studDisplay();

				edit(id);
				continue_option = ask("Edit for another student(s)? (y/n) : ");
			} while (continue_option == "y");
			cout << setcolour(brightGreen) << "\nStudent List updated successfully!\n" << resetcolour();
			system("Pause");

		}

		else if (choice == "2") {
			//go to del funtion

			do {
				//clean screen, get new studList and display studList
				system("cls");
				studGroup();
				studDisplay();

				del(id);
				continue_option = ask("Do you want to continue to delete other record?(y/n): ");
				if (studNum_counter() == 0) { cout << endl; break; }
			} while (continue_option == "y");
			cout << setcolour(brightGreen) << "\nStudent List updated successfully!\n" << resetcolour();
			system("Pause");

		}
		else if (choice == "X") { cout << "\nexit" << endl; break; }
		else { cout << setcolour(brightRed) << "Invalid input,please enter again" << resetcolour() << endl; }
		cout << endl;
	}
}
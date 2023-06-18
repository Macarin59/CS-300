#include <stdexcept>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;

// Course structure
struct Course {
	string courseNumber;
	string courseName;
	vector<string> prerequisites;
};

// Helper function that allows prints basic course information
void PrintFormatter(Course course) {
	cout << course.courseNumber << " : " << course.courseName << endl;
}

// Helper function that splits a string using a delimiter
vector<string> Split(string str, char delim) {
	vector<string> split;
	int pos = 0;
	for (int i = 0; i <= str.size(); i++) {
		if ((str[i] == delim) || (str.size() == i)) {
			string temp;
			temp.append(str, pos, i - pos);
			split.push_back(temp);
			pos = i + 1;
		}
	}
	return split;
}

// Function to parse the data given by the Reader() and ensure its validity
// Uses the Split() helper function to help validate lines
void Parser(vector<string> courseInfo) {
	// Test vectors
	vector<string> courseNums;
	vector<string> prereqs;
	// Loop checks for missing parameters and loads the test vectors
	for (int i = 0; i < courseInfo.size(); ++i) {
		vector<string> line = Split(courseInfo.at(i), ',');
		if (line.size() < 2) {
			throw "File formatting error: missing parameters";
		}
		courseNums.push_back(line.at(0)); // Loads the courseNums test vector
		if (line.size() > 2) {
			for (int j = 2; j < line.size(); j++) {
				prereqs.push_back(line.at(j)); // Loads the prereqs test vector
			}
		}
	}
	// Loop cycles through the test vectors to ensure all prerequisites are valid
	for (int i = 0; i < prereqs.size(); ++i) {
		for (int j = 0; j < courseNums.size(); j++) {
			if (prereqs.at(i) == courseNums.at(j)) {
				break;
			}
			// If the loop reaches the last course number without finding a match, the prerequisite is invalid
			else if (j == courseNums.size() - 1) {
				string s = prereqs.at(i) + " does not exist in course list";
				const char* msg = s.c_str();
				throw msg;
			}
		}
	}
}

// Function that opens, reads, and closes a given file
// The Parser() function ensures the data is valid.
vector<string> Reader(string fileName) {
	vector<string> courseInfo;
	string line;
	ifstream file(fileName);
	// Throws error if the file did not open properly
	if (!file.is_open()) {
		throw "File failed to open";
	}
	while (file.good()) {
		while (getline(file, line)) {
			courseInfo.push_back(line);
		}
	}
	file.close();
	Parser(courseInfo);
	return courseInfo;
}

// Function to load courses into a given course vector from a text file
// Uses the Reader() function to obtain data from a text file
// Uses the helper Split() function to process the raw data
void loadCourses(string fileName, vector<Course>* courses) {
	vector<string> courseInfo = Reader(fileName);
	// Loop loads courses one at a time
	for (int i = 0; i < courseInfo.size(); ++i) {
		Course course;
		vector<string> line = Split(courseInfo.at(i), ',');
		course.courseNumber = line.at(0);
		course.courseName = line.at(1);
		for (int j = 2; j < line.size(); j++) {
			course.prerequisites.push_back(line.at(j));
		}
		courses->push_back(course);
	}
}

// Function to print the course information of a user given course number
// Throws an exception if the course is not found
void printCourseInformation(vector<Course> courses, string courseNumber) {
	Course course;
	for (int i = 0; i < courses.size(); ++i) {
		course = courses.at(i);
		if (course.courseNumber == courseNumber) {
			PrintFormatter(course);
			// Special formatting for the prerequisites
			if (course.prerequisites.size() > 0) {
				cout << "Prerequisites: ";
				for (int j = 0; j < course.prerequisites.size() - 1; j++) {
					cout << course.prerequisites.at(j) << ", ";
				}
				cout << course.prerequisites.at(course.prerequisites.size() - 1) << endl;
			}
			// Return statement ensures the function stops once the selected course is found.
			return;
		}
	}
	// Can only be reached if no course was found
	throw "Course not found";
}

// Helper function for PrintSorted()
// Uses the selection sort alogrithm to sort the given course vector.
void Sort(vector<Course>* courses) {
	int i;
	int j;
	int min;
	Course temp;

	for (i = 0; i < courses->size() - 1; i++) {
		min = i;
		for (j = i + 1; j < courses->size(); j++) {
			if (courses->at(j).courseNumber < courses->at(min).courseNumber) {
				min = j;
			}
		}

		if (min != i) {
			temp = courses->at(min);
			courses->at(min) = courses->at(i);
			courses->at(i) = temp;
		}
	}
}

// Function to sort and display the vector of courses
// Uses the helper function Sort()
void PrintSorted(vector<Course> courses) {
	Sort(&courses);
	for (int i = 0; i < courses.size(); ++i) {
		PrintFormatter(courses.at(i));
	}
}

// Main method
int main() {
	// Set up variables
	string fileName;
	int choice = 0;
	vector<Course> courses;
	string courseNumber;
	
	// Input for file name
	// Runs until a valid file name is received
	while (true) {
		// Gets file name from user
		cout << "Enter file name: ";
		cin >> fileName;
		cout << endl;
		// Validation for user input
		ifstream file;
		file.open(fileName);
		if (!file) {
			cout << "File does not exist." << endl;
			cout << "Try again." << endl;
		}
		// Breaks out of validation loop
		else {
			break;
		}
	}

	// Main menu for program
	// Choice 4 exits
	while (choice != 4) {
		// Exception handling
		try {
			// Main menu dialogue
			cout << "Menu" << endl;
			cout << "1. Load Data Structure" << endl;
			cout << "2. Print Course List" << endl;
			cout << "3. Print Course" << endl;
			cout << "4. Exit" << endl;
			cout << "Enter choice: ";
			cin >> choice;
			cout << endl;

			// Menu switch based on user input
			switch (choice) {
			// Loads data structure with data from the file given
			// Note: Must be run before using option 2 and 3
			case 1:
				loadCourses(fileName, &courses);
				cout << "Courses have been loaded" << endl;

				break;
			// Prints a sorted list of courses
			case 2:
				// Throws exception if courses was not loaded yet
				if (courses.size() < 1) {
					throw "You need to load courses first";
				}
				PrintSorted(courses);

				break;
			// Prints information about a course given by the user
			case 3:
				// Throws exception if courses was not loaded yet
				if (courses.size() < 1) {
					throw "You need to load courses first";
				}
				// User dialogue for course number
				cout << "Enter a course number: ";
				cin >> courseNumber;
				cout << endl;
				// Case insensitive formatting
				for (int i = 0; i < courseNumber.length(); ++i) {
					courseNumber[i] = toupper(courseNumber[i]);
				}
				printCourseInformation(courses, courseNumber);

				break;
			// Goodbye message
			case 4:
				cout << "Have a great day!" << endl;

				break;
			// Default case throws an exception
			default:
				throw "Not a valid menu option, try again.";
			}
			cout << endl;
		}
		// Catches exceptions and allows for the menu to continue without crashing
		catch (const char* msg) {
			// Clears cin
			cin.clear();
			cin.ignore(10000, '\n');
			// Outputs error message
			cout << msg << endl;
			cout << endl;
		}
	}
}
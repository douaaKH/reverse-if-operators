// ReverseIf.cpp : This file contains the 'main' function. Program execution begins and ends there.
// By Douaa Khalil
// 25/02/2021
//

/*
	Usage: run the executable file in the command window, adding the name of the file.cpp you want to test
	ex.: ./a filename.cpp

	NOTES: 
	1. This code looks for the operators exclusively in lines that contain at least one if statement, instead of looking for them in the entire code,
	i.e. lines that do not contain at least one if statement (or are not part of a one) are not further processed
	2. Lines that do contain at least one if statement (or are part of a multiple-lines one) are passed to the reverseLogRelOperators, 
	for operators to be reversed
	3. The logical NOT (!) case is handled in one direction, i.e. !bool is reversed to bool, but bool to !bool is not handled as it's more complicated
	4. This code does not require the .cpp files tested to be properly formatted 
*/

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

bool isIfStatement(int pos, int const dim, string const& s) {
	if(isalpha(s.at(pos - 1)) || isalpha(s.at(pos + dim))) return false;
	
	// else
    return true;
}

void reverseLogRelOperators(std::string& s) {
    size_t i = 0;
    while (i < s.size()) {
		
		// ignore alphanumerical characters
        if (isalnum(s[i])) { i++; continue; }

        if (s[i] == '!') {
            if (s[i + 1] != '=') {
				// Logical NOT
                s.replace(i, 1, "");
                i++;
                continue;
            }
			// !=
            s.replace(i, 2, "==");
            i += 2;
            continue;
        }

        if (s[i] == '=' && s[i+1] == '=') {
			// ==
            s.replace(i, 2, "!=");
            i += 2;
            continue;
        }

        if (s[i] == '&' && !isalnum(s[i+1])) {
            // &&
			s.replace(i, 2, "||");
            i += 2;
            continue;
        }

        if (s[i] == '|') {
			// ||
            s.replace(i, 2, "&&");
            i += 2;
            continue;
        }

        if (s[i] == '>') {
            if (s[i + 1] == '=') {
				// >=
                s.replace(i, 2, "<=");
                i += 2;
                continue;
            }
			// >
            s.replace(i, 1, "<");
            i++;
            continue;
        }

        if (s[i] == '<') {
            if (s[i + 1] == '=') {
				// <=
                s.replace(i, 2, ">=");
                i += 2;
                continue;
            }
			// <
            s.replace(i, 1, ">");
            i++;
            continue;
        }

		// ignore characters such as "("
        i++;
    }

}

int main(int argc, char* argv[]) {
	
    // check the number of parameteres
    if (argc < 2) {
       cerr << "Insufficient arguments!" << endl << "usage: " << argv[0] << " filename-to-be-tested" << endl;
       return 1;
    }
	
	// declaring input and output files
	ifstream CPPFile;
	ofstream CPPFile_ReversedIf;
	
    // string to store each line text
    string fileLineText;

    // string to store multiple-lines if
    string allIf = "";
	
	// check spelling of filename-to-be-tested
	// if it has the .cpp extension
	string filename = string(argv[1]);
	size_t extPos = filename.find(".cpp");
	if(extPos == string::npos){
		// not found
		// Read from the c++ file
		CPPFile.open(string("./TestCPPFiles/") + filename + string(".cpp"));
		
		// test input file opened
		if(!CPPFile) {
			cerr << "Couldn't open your file, check if it exists." << endl;
			return 1;
		}

		// Create and open a text file
		CPPFile_ReversedIf.open(string("./TestCPPFiles/") + filename + string("-reversedIF.cpp"));
		
	} else {
		// found
		// Read from the c++ file
		CPPFile.open(string("./TestCPPFiles/") + filename);

		// test input file opened
		if(!CPPFile) {
			cerr << "Couldn't open your file, check if it exists." << endl;
			return 1;
		}

		// Create and open a text file
		size_t filenameSize = filename.size();
		CPPFile_ReversedIf.open(string("./TestCPPFiles/") + filename.substr(0, filenameSize - string(".cpp").size())  + string("-reversedIF.cpp"));
	}
			
	// test output file creation
	if(!CPPFile_ReversedIf) {
		cerr << "Couldn't create output file." << endl;
		return 1;
	}	

    // position at which the if statement is detected
    size_t pos = 0;

    // counters use to delimit an if-statement
    int countLP; // number of left parenthesis in if-statement
    int countRP = -1; // number of right parenthesis in if-statement
	
	// string indexing
    size_t start;
    size_t temp;

    while (getline(CPPFile, fileLineText)) {
        // find possible first occurence of an if statement
        pos = fileLineText.find("if");

        // no possible if statement in this line
		// continue to next line
        if (pos == string::npos) {
            // write in the file the unchanged line
            CPPFile_ReversedIf << fileLineText << endl;
            continue;
        }

        // possible if statement detected
        while (pos != string::npos) {

            // check if there is a real if statement in the line
            if (!isIfStatement(pos, 2, fileLineText)) {
                pos = fileLineText.find("if", pos + 1);
                continue;
            }

            // found an if statement at pos
            // restart
            allIf = "";
            countLP = 0;
            countRP = 0;
            start = pos;
			
            do {
                allIf += fileLineText; // concatenate this line to allIf

                do {
                    temp = fileLineText.find("(", start);
                    if (temp != string::npos) {
                        countLP++;
                        start = temp + 1;
                    }
                    else {
                        break;
                    }

                } while (start != string::npos);

                if (countLP > 0) {
					// restart
                    start = 0;
                    //countRP = 0;

                    do {
                        temp = fileLineText.find(")", start);
                        if (temp != string::npos) {
                            countRP++;
                            start = temp + 1;
                        }
                        else {
                            break;
                        }

                    } while (countRP < countLP && start != string::npos);

                    if (countRP == countLP) {
						// break from do while (countLP > countRP) loop
						// if statement is in a single line
                        break;
                    }
					
					// if statement is in a multiple-line
                    allIf += string("\n");
                    getline(CPPFile, fileLineText);
                    start = 0;
                }
            } while (countLP > countRP); // there is more left parenthesis than right parenthesis => if statement not finished or there are multiple if 

            reverseLogRelOperators(allIf);
            CPPFile_ReversedIf << allIf << endl;

            break;
        }
    }

    // close files
    CPPFile.close();
    CPPFile_ReversedIf.close();
}
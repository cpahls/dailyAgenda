/**
 *   @file: appointment_main.cc
 * @author: Christian Pahls
 *   @date: 04/24/2024
 *  @brief: This program reads in a daily agenda and allows the user to make changes.
*/

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <string>
#include "appointment.h"
using namespace std;

// Helper functions
vector<string> get_agenda(string sFile) {
    // Reads agenda data from text file into a vector

    // Define variables
    string line;
    vector<string> vAgenda;

    // Read agenda text file into vector (skips empty lines :)
    ifstream inFile(sFile);

    while (getline(inFile, line)) {
        if (line.length() < 1 || (line.length() == 1 && isspace(line.at(0)))) {
            // Skip empty lines or lines with only spaces
            continue;
        }
        // Process the non-empty line
        vAgenda.push_back(line);
    }
    inFile.close(); // Closes text file

    return vAgenda;
}

vector<string> print_schedule(vector<string> vData) {
    // Prints (to screen) the daily schedule (formatted output)

    int titleW, yrW, moW, dayW, timeW, durW;

    titleW = 40;
    yrW = 6;
    moW = 8;
    dayW = 6;
    timeW = 10;
    durW = 10;

    // Print report headers
    cout << left << setw(titleW) << "Meeting Title"
         << left << setw(yrW) << "Year"
         << left << setw(moW) << "Month"
         << left << setw(dayW) << "Day"
         << left << setw(timeW) << "Time"
         << left << setw(durW) << "Duration" << "\n";
    cout << "--------------------------------------------------------------------------------" << "\n";

    for (const auto& s : vData) {
        Appointment apptObj(s);
        apptObj.printSch();
    }

    return vData;
}


vector<string> print_at_time(vector<string> vData, string time) {
    // Prints the daily agenda of appointments at a specific time (military time)

    int iTime, titleW, yrW, moW, dayW, timeW, durW;

    iTime = stoi(time);
    titleW = 40;
    yrW = 6;
    moW = 8;
    dayW = 6;
    timeW = 10;
    durW = 10;

    // Print report headers
    cout << left << setw(titleW) << "Meeting Title"
         << left << setw(yrW) << "Year"
         << left << setw(moW) << "Month"
         << left << setw(dayW) << "Day"
         << left << setw(timeW) << "Time"
         << left << setw(durW) << "Duration" << "\n";
    cout << "--------------------------------------------------------------------------------" << "\n";

for (const auto& s : vData) {
        Appointment apptObj(s);
        apptObj.printAtTime(iTime);
    }

    return vData;
}


string add_appointment(string sFile, string sAppt) {
    // Adds an appointment to the text file

    fstream outfile(sFile, ios::app);   // open file for append
    if (outfile.is_open()) {
        outfile << "\n" << sAppt;   // append new appointment
        outfile.close();
        cout << "Appointment added successfully." << endl;
    } else {
        cerr << "Error opening the file." << endl;
    }
}


vector<string> delete_title(vector<string> vData, string sTitle, string sFile) {
    // Deletes an appointment based on specific title

    int i;
    i = 0;

    // Clear item in array where appointment title matches title entered
    for (const auto& s : vData) {
        Appointment apptObj(s);
        //cout << apptObj.deleteWithTitle(sTitle) << endl;
        if (apptObj.deleteWithTitle(sTitle) == 1) {
            vData[i] = "";
        }
        i++;
    }

    // Write updated array back out to text file (replaces existing file)
    ofstream outFile(sFile);
    i = 0;
    for (const auto& s : vData) {
        outFile << vData[i] << "\n";
        i++;
    }
    outFile.close();

    return vData;
}


vector<string> delete_time(vector<string> vData, string sTime, string sFile) {
    // Deletes an appointment based on specific start time (military time)

    int i, iTime;

    i = 0;
    iTime = stoi(sTime);    //convert from string to integer

    // Clear item in array where appointment time matches time entered (military time)
    for (const auto& s : vData) {
        Appointment apptObj(s);
        if (apptObj.deleteAtTime(iTime) == 1) {
            vData[i] = "";
        }
        i++;
    }

    // Write updated array back out to text file (replaces existing file)
    ofstream outFile(sFile);
    i = 0;
    for (const auto& s : vData) {
        outFile << vData[i] << "\n";
        i++;
    }
    outFile.close();

    return vData;
}

//-----------------------------------------------------------------------------------------------------

int main(int argc, char* argv[]) {

// Define variables
    vector<string> vAgenda;
    string sArg, sFile;

    sFile = "agenda.txt";

// Read text file into vector
    vAgenda = get_agenda(sFile);

// Perform different actions based on command line argument(s)
    if (argc > 1 && argc <= 3) {
        sArg = argv[1];
        // Print daily schedule sorted by start time in standard time format
        if (sArg=="-ps") {
            print_schedule(vAgenda);
        // Print all appointments at specified military time;
        } else if (sArg=="-p") {
            print_at_time(vAgenda, argv[2]);
        // Add an appointment given in the format: title|year|month|day|time|duration
        } else if (sArg=="-a") {
            add_appointment(sFile, argv[2]);
        // Delete all appointments that match a title exactly
        } else if (sArg=="-dt") {
            delete_title(vAgenda, argv[2], sFile);
        // Delete all appointments that match the starting military time exactly
        } else if (sArg=="-dm") {
            delete_time(vAgenda, argv[2], sFile);
        } else {
            // Output if argument given is not defined above
            cout << "Argument not recognized, please try again." << endl;
        }
    } else if (argc <= 1) {
        // Output if no argument given in addition to ./a.out
        cout << "Error: no arguments given." << endl;
    } else if (argc > 3) {
        // Output if too many arguments are given
        cout << "Error: too many arguments given." << endl;
    }

    return 0;
} // End of main
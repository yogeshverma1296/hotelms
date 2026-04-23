#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <string>
#include <algorithm>
using namespace std;

// ====================== Guest Class ======================
class Guest {
public:
    string roomNumber, name, gender, contact;
    string checkInDate, checkOutDate, roomType, paymentStatus;
    int nights;
    float billAmount;

    Guest() : nights(0), billAmount(0.0) {}

    void display() const {
        cout << "\033[1;36m\n---------------------------------------------\n\033[0m";
        cout << "Room Number: " << roomNumber
             << "\nGuest Name: " << name
             << "\nGender: " << gender
             << "\nContact: " << contact
             << "\nCheck-In Date: " << checkInDate
             << "\nCheck-Out Date: " << checkOutDate
             << "\nRoom Type: " << roomType
             << "\nNights Stayed: " << nights
             << "\nBill Amount: " << fixed << setprecision(2) << billAmount
             << "\nPayment Status: " << paymentStatus << "\n";
    }

    string toFileString() const {
        stringstream ss;
        ss << roomNumber << "," << name << "," << gender << ","
           << contact << "," << checkInDate << "," << checkOutDate << ","
           << roomType << "," << nights << "," << billAmount << ","
           << paymentStatus;
        return ss.str();
    }
};

// ====================== HotelManagement Class ======================
class HotelManagement {
private:
    vector<Guest> guests;

    // Utility function to clear screen
    void clearScreen() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

public:
    // Constructor
    HotelManagement() {
        loadGuests();
    }

    // Load guests from file
    void loadGuests() {
        ifstream file("hotel_records.txt");
        if (!file) {
            cout << "\033[1;33mNo existing records found. Starting fresh...\033[0m\n";
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            Guest g;
            string nightsStr, billStr;
            getline(ss, g.roomNumber, ',');
            getline(ss, g.name, ',');
            getline(ss, g.gender, ',');
            getline(ss, g.contact, ',');
            getline(ss, g.checkInDate, ',');
            getline(ss, g.checkOutDate, ',');
            getline(ss, g.roomType, ',');
            getline(ss, nightsStr, ',');
            g.nights = stoi(nightsStr);
            getline(ss, billStr, ',');
            g.billAmount = stof(billStr);
            getline(ss, g.paymentStatus, ',');
            guests.push_back(g);
        }
        file.close();
    }

    // Save all guests to file
    void saveGuests() {
        ofstream file("hotel_records.txt");
        for (auto &g : guests)
            file << g.toFileString() << "\n";
        file.close();
    }

    // Add new guest
    void addGuest() {
        clearScreen();
        Guest g;
        cout << "Enter Room Number: ";
        cin >> g.roomNumber;

        // Check if room is already booked
        for (auto &existing : guests) {
            if (existing.roomNumber == g.roomNumber) {
                cout << "\033[1;31mRoom already occupied! Try another room.\033[0m\n";
                return;
            }
        }

        cin.ignore();
        cout << "Enter Guest Name: ";
        getline(cin, g.name);
        cout << "Enter Gender (M/F): ";
        getline(cin, g.gender);
        cout << "Enter Contact Number: ";
        getline(cin, g.contact);
        cout << "Enter Check-In Date (DD/MM/YYYY): ";
        getline(cin, g.checkInDate);
        cout << "Enter Check-Out Date (DD/MM/YYYY): ";
        getline(cin, g.checkOutDate);
        cout << "Enter Room Type (Single/Double/Deluxe): ";
        getline(cin, g.roomType);
        cout << "Enter Number of Nights Stayed: ";
        cin >> g.nights;
        cout << "Enter Bill Amount: ";
        cin >> g.billAmount;
        cin.ignore();
        cout << "Enter Payment Status (Paid/Pending): ";
        getline(cin, g.paymentStatus);

        guests.push_back(g);
        saveGuests();

        cout << "\033[1;32mGuest added successfully!\033[0m\n";
    }

    // View all guests
    void viewAllGuests() {
        clearScreen();
        if (guests.empty()) {
            cout << "No guest records found.\n";
            return;
        }
        for (auto &g : guests)
            g.display();
    }

    // Search guest
    void searchGuest() {
        clearScreen();
        string room;
        cout << "Enter Room Number to search: ";
        cin >> room;

        bool found = false;
        for (auto &g : guests) {
            if (g.roomNumber == room) {
                g.display();
                found = true;
            }
        }

        if (!found)
            cout << "No record found for Room Number " << room << endl;
    }

    // Delete guest
    void deleteGuest() {
        clearScreen();
        string room;
        cout << "Enter Room Number to delete: ";
        cin >> room;

        auto it = remove_if(guests.begin(), guests.end(), [&](Guest &g) {
            return g.roomNumber == room;
        });

        if (it != guests.end()) {
            guests.erase(it, guests.end());
            saveGuests();
            cout << "\033[1;31mGuest record deleted successfully!\033[0m\n";
        } else {
            cout << "Room not found!\n";
        }
    }

    // Summary Report
    void summaryReport() {
        clearScreen();
        int single = 0, dbl = 0, deluxe = 0, pending = 0, paid = 0;
        float totalRevenue = 0;

        for (auto &g : guests) {
            string rt = g.roomType;
            transform(rt.begin(), rt.end(), rt.begin(), ::tolower);

            if (rt == "single") single++;
            else if (rt == "double") dbl++;
            else if (rt == "deluxe") deluxe++;

            string pay = g.paymentStatus;
            transform(pay.begin(), pay.end(), pay.begin(), ::tolower);
            if (pay == "paid") paid++;
            else pending++;

            totalRevenue += g.billAmount;
        }

        cout << "\033[1;33m===== HOTEL SUMMARY REPORT =====\033[0m\n";
        cout << "Total Guests: " << guests.size() << "\n";
        cout << "Single Rooms: " << single << "\n";
        cout << "Double Rooms: " << dbl << "\n";
        cout << "Deluxe Rooms: " << deluxe << "\n";
        cout << "Payments Pending: " << pending << "\n";
        cout << "Payments Completed: " << paid << "\n";
        cout << "Total Revenue (Approx.): " << fixed << setprecision(2) << totalRevenue << "\n";
    }

    // Main Menu
    void menu() {
        int choice;
        while (true) {
            cout << "\033[1;34m\n===== HOTEL MANAGEMENT SYSTEM =====\033[0m\n";
            cout << "1. Add New Guest\n";
            cout << "2. View All Guests\n";
            cout << "3. Search Guest\n";
            cout << "4. Delete Guest\n";
            cout << "5. Summary Report\n";
            cout << "6. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1: addGuest(); break;
            case 2: viewAllGuests(); break;
            case 3: searchGuest(); break;
            case 4: deleteGuest(); break;
            case 5: summaryReport(); break;
            case 6:
                cout << "\033[1;33mExiting... All data saved successfully!\033[0m\n";
                saveGuests();
                return;
            default:
                cout << "Invalid choice. Try again!\n";
            }
        }
    }
};

// ====================== MAIN FUNCTION ======================
int main() {
    HotelManagement hotel;
    hotel.menu();
    return 0;
}

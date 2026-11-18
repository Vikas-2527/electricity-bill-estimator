// Electricity Bill Estimator - OOP Mini Project (SDG 7)
// Compile: g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o estimator

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <string>
#include <limits>
#include <iomanip>
#include <cstdlib>
using namespace std;

const double RATE_PER_UNIT = 8.0;          // assumed rate (Rs per unit)
const string FILE_NAME = "appliances.txt";

// ---------- Abstract base class ----------
class Appliance {
protected:
    string name;
    double watts;
    double hours;
public:
    Appliance(const string& n, double w, double h) : name(n), watts(w), hours(h) {}
    virtual ~Appliance() = default;

    virtual double calculateUnits() const = 0;      // pure virtual -> polymorphism
    virtual string getType() const = 0;
    virtual int getQuantity() const { return 1; }
    virtual string getWarning() const { return ""; }

    string serialize() const {
        ostringstream out;
        out << getType() << "|" << name << "|" << watts << "|" << hours
            << "|" << getQuantity();
        return out.str();
    }

    void display(int no) const {
        cout << "#" << no << " [" << getType() << "] " << name << "\n"
             << "   Watts: " << watts << " | Hours/day: " << hours
             << " | Qty: " << getQuantity() << "\n"
             << "   Units/day: " << fixed << setprecision(2) << calculateUnits()
             << " kWh | Cost/day: Rs " << calculateUnits() * RATE_PER_UNIT << "\n";
        if (!getWarning().empty())
            cout << "   WARNING: " << getWarning() << "\n";
    }
};

// ---------- Derived classes ----------
class Bulb : public Appliance {
    int quantity;
public:
    Bulb(const string& n, double w, double h, int q)
        : Appliance(n, w, h), quantity(q) {}
    double calculateUnits() const override { return quantity * watts * hours / 1000.0; }
    string getType() const override { return "Bulb"; }
    int getQuantity() const override { return quantity; }
};

class Fan : public Appliance {
public:
    Fan(const string& n, double w, double h) : Appliance(n, w, h) {}
    double calculateUnits() const override { return watts * hours / 1000.0; }
    string getType() const override { return "Fan"; }
};

class AC : public Appliance {
public:
    AC(const string& n, double w, double h) : Appliance(n, w, h) {}
    double calculateUnits() const override { return watts * hours / 1000.0; }
    string getType() const override { return "AC"; }
    string getWarning() const override {
        return hours > 8 ? "High usage! AC runs more than 8 hours/day." : "";
    }
};

// ---------- Input helpers (validation) ----------
int readInt(const string& prompt, int lo, int hi) {
    int v;
    while (true) {
        cout << prompt;
        if (cin >> v && v >= lo && v <= hi) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return v;
        }
        if (cin.eof()) exit(0);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Enter a number from " << lo << " to " << hi << ".\n";
    }
}

double readDouble(const string& prompt, double lo, double hi) {
    double v;
    while (true) {
        cout << prompt;
        if (cin >> v && v >= lo && v <= hi) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return v;
        }
        if (cin.eof()) exit(0);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Enter a number from " << lo << " to " << hi << ".\n";
    }
}

string readName(const string& prompt) {
    string s;
    while (true) {
        cout << prompt;
        getline(cin, s);
        if (cin.eof()) exit(0);
        for (char& c : s) if (c == '|') c = ' ';   // '|' is our file separator
        if (!s.empty()) return s;
        cout << "Name cannot be empty.\n";
    }
}

// ---------- File handling ----------
void saveToFile(const vector<unique_ptr<Appliance>>& list) {
    ofstream out(FILE_NAME);
    if (!out) {
        cout << "Could not open file for saving.\n";
        return;
    }
    for (const auto& a : list) out << a->serialize() << "\n";
}

void loadFromFile(vector<unique_ptr<Appliance>>& list) {
    ifstream in(FILE_NAME);
    if (!in) return;                                // first run: no file yet
    string line;
    while (getline(in, line)) {
        stringstream ss(line);
        string type, name, w, h, q;
        if (!getline(ss, type, '|') || !getline(ss, name, '|') ||
            !getline(ss, w, '|') || !getline(ss, h, '|') || !getline(ss, q, '|'))
            continue;                               // skip bad line
        try {
            double watts = stod(w), hours = stod(h);
            int qty = stoi(q);
            if (type == "Bulb")      list.push_back(make_unique<Bulb>(name, watts, hours, qty));
            else if (type == "Fan")  list.push_back(make_unique<Fan>(name, watts, hours));
            else if (type == "AC")   list.push_back(make_unique<AC>(name, watts, hours));
        } catch (...) {
            continue;                               // skip corrupted line
        }
    }
}

// ---------- Menu actions ----------
void addAppliance(vector<unique_ptr<Appliance>>& list) {
    int type = readInt("Type (1 Bulb, 2 Fan, 3 AC): ", 1, 3);
    string name = readName("Appliance name: ");
    double watts = readDouble("Power in watts (1-5000): ", 1, 5000);
    double hours = readDouble("Hours used per day (0.1-24): ", 0.1, 24);

    if (type == 1) {
        int qty = readInt("Quantity (1-50): ", 1, 50);
        list.push_back(make_unique<Bulb>(name, watts, hours, qty));
    } else if (type == 2) {
        list.push_back(make_unique<Fan>(name, watts, hours));
    } else {
        list.push_back(make_unique<AC>(name, watts, hours));
    }
    cout << "Appliance added.\n";
}

void viewAll(const vector<unique_ptr<Appliance>>& list) {
    if (list.empty()) {
        cout << "No appliances added yet.\n";
        return;
    }
    int no = 1;
    for (const auto& a : list) a->display(no++);   // runtime polymorphism
}

void showTotal(const vector<unique_ptr<Appliance>>& list) {
    if (list.empty()) {
        cout << "No appliances added yet.\n";
        return;
    }
    int days = readInt("Number of days (1-31): ", 1, 31);
    double perDay = 0;
    for (const auto& a : list) perDay += a->calculateUnits();
    double units = perDay * days;

    cout << fixed << setprecision(2)
         << "\n----- BILL SUMMARY -----\n"
         << "Units per day : " << perDay << " kWh\n"
         << "Days          : " << days << "\n"
         << "Total units   : " << units << " kWh\n"
         << "Rate (assumed): Rs " << RATE_PER_UNIT << " per unit\n"
         << "Estimated bill: Rs " << units * RATE_PER_UNIT << "\n"
         << "------------------------\n";
}

int main() {
    vector<unique_ptr<Appliance>> appliances;
    loadFromFile(appliances);

    while (true) {
        cout << "\n=== ELECTRICITY BILL ESTIMATOR ===\n"
             << "1 Add appliance\n"
             << "2 View all appliances\n"
             << "3 Total units and estimated bill\n"
             << "4 Save and exit\n";
        int choice = readInt("Choice: ", 1, 4);

        switch (choice) {
            case 1: addAppliance(appliances); break;
            case 2: viewAll(appliances); break;
            case 3: showTotal(appliances); break;
            case 4:
                saveToFile(appliances);
                cout << "Saved. Goodbye!\n";
                return 0;
        }
    }
}
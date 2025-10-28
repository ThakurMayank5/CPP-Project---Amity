#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

class Flight
{
    friend class DomesticFlight;
    friend class InternationalFlight;

protected:
    int flightNumber;
    string flightName;
    string source;
    string destination;
    int seats;
    double baseFare;
    string type; // "Domestic" or "International"

public:
    // default constructor
    Flight()
        : flightNumber(0), flightName(""), source(""), destination(""), seats(0), baseFare(0.0), type("") {}

    Flight(int fno, string name, string src, string dest, int s, double fare, string t)
        : flightNumber(fno), flightName(name), source(src), destination(dest), seats(s), baseFare(fare), type(t)
    {
    }

    void StoreFlightDetails(int fno, string name, string src, string dest, int s, double fare, string t)
    {
        flightNumber = fno;
        flightName = name;
        source = src;
        destination = dest;
        seats = s;
        baseFare = fare;
        type = t;
    }

    void display() const
    {
        cout << "Flight Number: " << flightNumber << endl;
        cout << "Flight Name: " << flightName << endl;
        cout << "Source: " << source << endl;
        cout << "Destination: " << destination << endl;
        cout << "Seats: " << seats << endl;
        cout << "Base Fare: " << baseFare << endl;
        cout << "Fare with Taxes: " << (type == "Domestic" ? baseFare * 1.05 : baseFare * 1.20) << endl;
        cout << "Type: " << type << endl;
    }

    string getType()
    {
        return type;
    }
};

class DomesticFlight : public Flight
{
public:
    DomesticFlight(int fno, string name, string src, string dest, int s, double fare)
        : Flight(fno, name, src, dest, s, fare, "Domestic") {}

    double calculateFare()
    {
        return baseFare * 1.05; // 5% tax
    }

    static DomesticFlight convertToDomesticFlight(Flight flight)
    {
        return DomesticFlight(flight.flightNumber, flight.flightName, flight.source, flight.destination, flight.seats, flight.baseFare);
    }
    friend void saveFlightToFile(DomesticFlight *df);
};

class InternationalFlight : public Flight
{
public:
    InternationalFlight(int fno, string name, string src, string dest, int s, double fare)
        : Flight(fno, name, src, dest, s, fare, "International") {}

    double calculateFare()
    {
        return baseFare * 1.20; // 20% extra charges
    }
    static InternationalFlight convertToInternationalFlight(Flight flight)
    {
        return InternationalFlight(flight.flightNumber, flight.flightName, flight.source, flight.destination, flight.seats, flight.baseFare);
    }
    friend void saveFlightToFile(InternationalFlight *inf);
};

void saveFlightToFile(DomesticFlight *df)
{
    fstream fileout("flights.txt", ios::app);
    if (!fileout)
    {
        cerr << "Error opening file for writing!" << endl;
        return;
    }
    fileout << df->flightNumber << " " << df->flightName << " " << df->source << " " << df->destination << " "
            << df->seats << " " << df->baseFare << " " << df->type << endl;
    fileout.close();
}

void saveFlightToFile(InternationalFlight *inf)
{
    fstream fileout("flights.txt", ios::app);
    if (!fileout)
    {
        cerr << "Error opening file for writing!" << endl;
        return;
    }
    fileout << inf->flightNumber << " " << inf->flightName << " " << inf->source << " " << inf->destination << " "
            << inf->seats << " " << inf->baseFare << " " << inf->type << endl;
    fileout.close();
}

void saveFlightToFile(Flight *f)
{
    string type = f->getType();
    if (type != "Domestic" && type != "International")
    {
        cerr << "Invalid flight type!" << endl;
        return;
    }

    if (type == "Domestic")
    {
        DomesticFlight df = DomesticFlight::convertToDomesticFlight(*f);
        if (df.calculateFare() <= 0)
        {
            cerr << "Invalid fare for domestic flight!" << endl;
            return;
        }
        saveFlightToFile(&df);
    }
    else if (type == "International")
    {
        InternationalFlight inf = InternationalFlight::convertToInternationalFlight(*f);
        if (inf.calculateFare() <= 0)
        {
            cerr << "Invalid fare for international flight!" << endl;
            return;
        }
        saveFlightToFile(&inf);
    }
}

vector<Flight> readFlightsFromFile()
{
    fstream filein("flights.txt", ios::in);

    if (!filein)
    {
        return {};
    }

    if (filein.peek() == ifstream::traits_type::eof())
    {
        return {};
    }

    vector<Flight> flightList;

    while (!filein.eof())
    {
        int fno;
        string name, src, dest, type;
        int seats;
        double fare;

        filein >> fno >> name >> src >> dest >> seats >> fare >> type;

        if (type == "Domestic")
        {
            DomesticFlight flight(fno, name, src, dest, seats, fare);
            flightList.push_back(flight);
        }
        else if (type == "International")
        {
            InternationalFlight flight(fno, name, src, dest, seats, fare);
            flightList.push_back(flight);
        }
    }

    return flightList;
}

int main()
{
    while (true)
    {
        cout << "Menu:\n1. Display Flights\n2. Add Flight\n3. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (choice == 3)
            break;

        switch (choice)
        {
        case 1:
        {
            cout << "Displaying all flights:" << endl;
            const vector<Flight> flights = readFlightsFromFile();
            if (flights.empty())
            {
                cout << "No flights available." << endl;
            }
            else
            {
                for (const Flight &flight : flights)
                {
                    flight.display();
                    cout << "------------------------" << endl;
                }
            }
            break;
        }
        case 2:
        {
            cout << "Adding a new flight:" << endl;
            int fno, seats;
            double fare;
            string fname, src, dest, type;

            cout << "Enter Flight Number: ";
            cin >> fno;
            cout << "Enter Flight Name: ";
            cin >> fname;
            cout << "Enter Source: ";
            cin >> src;
            cout << "Enter Destination: ";
            cin >> dest;
            cout << "Enter Seats: ";
            cin >> seats;
            cout << "Enter Base Fare: ";
            cin >> fare;
            cout << "Enter Type (Domestic/International): ";

            while (true)
            {
                cin >> type;
                if (type == "Domestic" || type == "International")
                {
                    break;
                }
                cout << "Invalid type. Please enter Domestic or International: ";
            }

            Flight newFlight(fno, fname, src, dest, seats, fare, type);
            saveFlightToFile(&newFlight);

            break;
        }
        default:
            cout << "Invalid choice" << endl;
        }
    }

    return 0;
}
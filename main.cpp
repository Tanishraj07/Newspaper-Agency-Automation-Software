#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <iomanip>
#include <algorithm> // for std::find

using namespace std;

class Publication {
public:
    string name;
    double pricePerCopy;

    Publication(string n, double p) : name(n), pricePerCopy(p) {}
};

class Customer {
public:
    string name;
    string address;
    vector<Publication> subscriptions;
    double dues;
    bool hasOutstandingDue;
    bool deliveriesStopped;
    vector<pair<string, string>> stopRequests; // Pair of start date and end date for stopping deliveries

    Customer(string n, string addr) : name(n), address(addr), dues(0.0), hasOutstandingDue(false), deliveriesStopped(false) {}

    void addSubscription(const Publication& pub) {
        subscriptions.push_back(pub);
    }

    void removeSubscription(const Publication& pub) {
        auto it = remove_if(subscriptions.begin(), subscriptions.end(), [&pub](const Publication& p) {
            return p.name == pub.name;
        });
        subscriptions.erase(it, subscriptions.end());
    }

    void setDues(double amount) {
        dues = amount;
        hasOutstandingDue = dues > 0;
    }

    void makePayment(double amount) {
        dues -= amount;
        if (dues <= 0) {
            dues = 0;
            hasOutstandingDue = false;
        }
    }

    double getDues() const {
        return dues;
    }

    string getAddress() const {
        return address;
    }

    void requestStopDelivery(const string& startDate, const string& endDate) {
        stopRequests.push_back(make_pair(startDate, endDate));
        deliveriesStopped = true;
    }

    bool isDeliveryStopped(const string& currentDate) const {
        for (const auto& request : stopRequests) {
            if (currentDate >= request.first && currentDate <= request.second) {
                return true;
            }
        }
        return false;
    }

    void resumeDeliveries() {
        deliveriesStopped = false;
    }
};

class DeliveryPerson {
public:
    string name;
    double totalEarnings;

    DeliveryPerson(string n) : name(n), totalEarnings(0.0) {}

    void addEarnings(double amount) {
        totalEarnings += amount;
    }

    double getEarnings() const {
        return totalEarnings;
    }
};

class NewspaperAgency {
private:
    vector<Publication> publications;
    vector<Customer> customers;
    vector<DeliveryPerson> deliveryPersons;

    map<string, vector<pair<string, vector<Publication>>>> deliverySchedules; // deliveryPerson -> list of addresses with publications

public:
    void addPublication(const Publication& pub) {
        publications.push_back(pub);
    }

    void addCustomer(const Customer& customer) {
        customers.push_back(customer);
    }

    void addDeliveryPerson(const DeliveryPerson& dp) {
        deliveryPersons.push_back(dp);
    }

    void createDeliverySchedule() {
        // Example schedule creation, replace with actual logic
        for (const auto& dp : deliveryPersons) {
            vector<pair<string, vector<Publication>>> schedule;
            for (const auto& customer : customers) {
                if (!customer.deliveriesStopped) {
                    schedule.push_back(make_pair(customer.getAddress(), customer.subscriptions));
                }
            }
            deliverySchedules[dp.name] = schedule;
        }
    }

    void printDailyDelivery(const string& deliveryPersonName) {
        if (deliverySchedules.find(deliveryPersonName) != deliverySchedules.end()) {
            cout << "Delivery Schedule for " << deliveryPersonName << ":\n";
            for (const auto& delivery : deliverySchedules[deliveryPersonName]) {
                cout << "Deliver to: " << delivery.first << "\nPublications:\n";
                for (const auto& pub : delivery.second) {
                    cout << "  " << pub.name << " - $" << pub.pricePerCopy << endl;
                }
            }
        } else {
            cout << "No schedule found for " << deliveryPersonName << endl;
        }
    }

    void printMonthlyBill(const string& customerName) {
        for (auto& customer : customers) { // Note: Changed from const auto& to auto&
            if (customer.name == customerName) {
                double totalCost = 0;
                for (const auto& pub : customer.subscriptions) {
                    totalCost += pub.pricePerCopy;
                }
                customer.setDues(totalCost);
                cout << "Bill for " << customerName << ":\n";
                cout << "Total Cost: $" << totalCost << endl;
                break;
            }
        }
    }

    void printReceipts(const string& customerName, double amount) {
        for (auto& customer : customers) { // Note: Changed from const auto& to auto&
            if (customer.name == customerName) {
                customer.makePayment(amount);
                cout << "Receipt for " << customerName << ":\n";
                cout << "Amount Paid: $" << amount << endl;
                cout << "Remaining Dues: $" << customer.getDues() << endl;
                break;
            }
        }
    }

    void calculateDeliveryEarnings() {
        for (auto& dp : deliveryPersons) {
            double earnings = 0.0;
            for (const auto& delivery : deliverySchedules[dp.name]) {
                for (const auto& pub : delivery.second) {
                    earnings += pub.pricePerCopy * 0.025;
                }
            }
            dp.addEarnings(earnings);
            cout << "Earnings for " << dp.name << ": $" << dp.getEarnings() << endl;
        }
    }

    void processCustomerRequests() {
        for (auto& customer : customers) {
            // Implement logic to handle customer requests
            // Example: Check if delivery needs to be resumed or handled based on stop requests
        }
    }
};

int main() {
    NewspaperAgency agency;

    // Add publications
    agency.addPublication(Publication("Newspaper1", 1.5));
    agency.addPublication(Publication("Magazine1", 2.0));

    // Add customers
    Customer customer1("Alice", "123 Main St");
    customer1.addSubscription(Publication("Newspaper1", 1.5));
    agency.addCustomer(customer1);

    Customer customer2("Bob", "456 Elm St");
    customer2.addSubscription(Publication("Magazine1", 2.0));
    agency.addCustomer(customer2);

    // Add delivery persons
    DeliveryPerson dp1("John");
    DeliveryPerson dp2("Jane");
    agency.addDeliveryPerson(dp1);
    agency.addDeliveryPerson(dp2);

    // Create delivery schedules
    agency.createDeliverySchedule();

    // Example of customer requesting to stop delivery
    customer1.requestStopDelivery("2024-08-01", "2024-08-15");

    // Update delivery schedule after request
    agency.createDeliverySchedule();

    // Print delivery schedules
    agency.printDailyDelivery("John");

    // Print and handle billing
    agency.printMonthlyBill("Alice");
    agency.printReceipts("Alice", 1.5);

    // Calculate and print earnings
    agency.calculateDeliveryEarnings();

    return 0;
}

// Restaurant Management System - OOP mini project
// classes: MenuItem, OrderItem, Order, Table, Customer
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

int nextOrderId = 1001;
int nextMenuId = 1;
int nextCustomerId = 1;
const double CGST_RATE = 0.025; // 2.5%
const double SGST_RATE = 0.025; // 2.5%

class MenuItem {
private:
    int id;
    string name;
    string category;
    double price;
    bool available;
public:
    MenuItem(int id, const string &name, const string &category, double price) {
        this->id = id;
        this->name = name;
        this->category = category;
        this->price = price;
        available = true;
    }
    int getId() { return id; }
    string getName() { return name; }
    string getCategory() { return category; }
    double getPrice() { return price; }
    bool isAvailable() { return available; }
    void setAvailable(bool a) { available = a; }
    void setPrice(double p) { price = p; }
};

class Customer {
private:
    int id;
    string name;
    string phone;
    int totalOrders;
    double totalSpent;
public:
    Customer(int id, const string &name, const string &phone) {
        this->id = id;
        this->name = name;
        this->phone = phone;
        totalOrders = 0;
        totalSpent = 0.0;
    }
    int getId() { return id; }
    string getName() { return name; }
    string getPhone() { return phone; }
    int getTotalOrders() { return totalOrders; }
    double getTotalSpent() { return totalSpent; }
    void recordOrder(double amount) {
        totalOrders++;
        totalSpent += amount;
    }
    void showDetails() {
        cout << "id " << id << " - " << name << ", ph " << phone;
        cout << ", visits " << totalOrders << ", spent Rs" << fixed << setprecision(2) << totalSpent << endl;
    }
};

class OrderItem {
private:
    string itemName;
    double itemPrice;
    int quantity;
public:
    OrderItem(const string &name, double price, int qty) {
        itemName = name;
        itemPrice = price;
        quantity = qty;
    }
    string getName() { return itemName; }
    int getQty() { return quantity; }
    double getSubtotal() { return itemPrice * quantity; }
};

class Order {
private:
    int orderId;
    int tableNumber;
    string waiterName;
    int customerId; // -1 = walkin
    string customerName;
    string customerPhone;
    vector<OrderItem> items;
    string status;
    string paymentMode;
    bool paid;
public:
    Order(int id, int tableNum, const string &waiter) {
        orderId = id;
        tableNumber = tableNum;
        waiterName = waiter;
        customerId = -1;
        customerName = "Walk-in";
        customerPhone = "-";
        status = "Pending";
        paid = false;
    }
    void addItem(OrderItem item) { items.push_back(item); }
    void setStatus(string s) { status = s; }
    void setCustomer(int id, string n, string p) {
        customerId = id;
        customerName = n;
        customerPhone = p;
    }
    void markPaid(string mode) {
        paid = true;
        status = "Billed";
        paymentMode = mode;
    }
    int getOrderId() { return orderId; }
    int getTableNumber() { return tableNumber; }
    int getCustomerId() { return customerId; }
    string getStatus() { return status; }
    string getPaymentMode() { return paymentMode; }
    bool isPaid() { return paid; }
    bool isEmpty() { return items.size() == 0; }
    double getSubtotal() {
        double sub = 0;
        for (int i = 0; i < items.size(); i++) sub += items[i].getSubtotal();
        return sub;
    }
    double getCGST() { return getSubtotal() * CGST_RATE; }
    double getSGST() { return getSubtotal() * SGST_RATE; }
    double getGrandTotal() { return getSubtotal() + getCGST() + getSGST(); }

    void showBill() {
        cout << endl << "GRANDEUR RESTAURANT" << endl;
        cout << "Order #" << orderId << ", table " << tableNumber << endl;
        cout << "Customer: " << customerName;
        if (customerId != -1) cout << " (id " << customerId << ")";
        if (customerPhone != "-") cout << ", ph " << customerPhone;
        cout << endl << "Waiter: " << waiterName << ", status: " << status << endl;
        for (int i = 0; i < items.size(); i++) {
            cout << items[i].getName() << " x" << items[i].getQty()
                 << " = Rs" << fixed << setprecision(2) << items[i].getSubtotal() << endl;
        }
        cout << "Subtotal: Rs" << fixed << setprecision(2) << getSubtotal() << endl;
        cout << "CGST: Rs" << getCGST() << ", SGST: Rs" << getSGST() << endl;
        cout << "GRAND TOTAL: Rs" << getGrandTotal() << endl;
        if (paid) cout << "Paid via " << paymentMode << endl;
        else cout << "Not paid yet" << endl;
        cout << "thank you, visit again!" << endl << endl;
    }
};

class Table {
private:
    int tableNumber;
    int capacity;
    bool occupied;
    int currentOrderId;
public:
    Table(int num, int cap) {
        tableNumber = num;
        capacity = cap;
        occupied = false;
        currentOrderId = -1;
    }
    int getNumber() { return tableNumber; }
    int getCapacity() { return capacity; }
    bool isOccupied() { return occupied; }
    int getCurrentOrderId() { return currentOrderId; }
    void occupy(int orderId) { occupied = true; currentOrderId = orderId; }
    void free() { occupied = false; currentOrderId = -1; }
};

vector<MenuItem> menu;
vector<Order> orders;
vector<Table> tables;
vector<Customer> customers;

int findTableIndex(int tableNum) {
    for (int i = 0; i < tables.size(); i++)
        if (tables[i].getNumber() == tableNum) return i;
    return -1;
}
int findOrderIndex(int orderId) {
    for (int i = 0; i < orders.size(); i++)
        if (orders[i].getOrderId() == orderId) return i;
    return -1;
}
int findCustomerByPhone(string phone) {
    for (int i = 0; i < customers.size(); i++)
        if (customers[i].getPhone() == phone) return i;
    return -1;
}
int findCustomerById(int id) {
    for (int i = 0; i < customers.size(); i++)
        if (customers[i].getId() == id) return i;
    return -1;
}

void viewMenu() {
    string categories[4] = { "Starter", "Main", "Beverage", "Dessert" };
    cout << endl << "MENU" << endl;
    for (int c = 0; c < 4; c++) {
        bool headerDone = false;
        for (int i = 0; i < menu.size(); i++) {
            if (menu[i].getCategory() == categories[c] && menu[i].isAvailable()) {
                if (!headerDone) { cout << categories[c] << "s:" << endl; headerDone = true; }
                cout << menu[i].getId() << ") " << menu[i].getName() << " - Rs" << menu[i].getPrice() << endl;
            }
        }
    }
    cout << endl;
}

string selectPaymentMode() {
    cout << "how did they pay? 1) UPI  2) Cash  3) Other" << endl;
    int m;
    cin >> m;
    if (m == 1) return "UPI";
    else if (m == 2) return "Cash";
    else return "Other";
}

void showHome();
void waiterPanel();
void managerPanel();
void customerPanel();
void initData();

int main() {
    initData();
    showHome();
    return 0;
}

void initData() {
    menu.push_back(MenuItem(nextMenuId++, "Spring Rolls", "Starter", 120.0));
    menu.push_back(MenuItem(nextMenuId++, "Paneer Tikka", "Starter", 180.0));
    menu.push_back(MenuItem(nextMenuId++, "Chicken Soup", "Starter", 150.0));
    menu.push_back(MenuItem(nextMenuId++, "Butter Chicken", "Main", 320.0));
    menu.push_back(MenuItem(nextMenuId++, "Dal Makhani", "Main", 240.0));
    menu.push_back(MenuItem(nextMenuId++, "Veg Biryani", "Main", 220.0));
    menu.push_back(MenuItem(nextMenuId++, "Grilled Salmon", "Main", 480.0));
    menu.push_back(MenuItem(nextMenuId++, "Mango Lassi", "Beverage", 90.0));
    menu.push_back(MenuItem(nextMenuId++, "Masala Chai", "Beverage", 50.0));
    menu.push_back(MenuItem(nextMenuId++, "Fresh Lime Soda", "Beverage", 70.0));
    menu.push_back(MenuItem(nextMenuId++, "Gulab Jamun", "Dessert", 110.0));
    menu.push_back(MenuItem(nextMenuId++, "Ice Cream", "Dessert", 130.0));
    for (int i = 1; i <= 6; i++) {
        int cap;
        if (i <= 2) cap = 2;
        else if (i <= 4) cap = 4;
        else cap = 6;
        tables.push_back(Table(i, cap));
    }
}

void showHome() {
    while (true) {
        cout << endl << "GRANDEUR RESTAURANT" << endl;
        cout << "1. View Menu" << endl;
        cout << "2. Customer Portal" << endl;
        cout << "3. Waiter Login" << endl;
        cout << "4. Manager Login" << endl;
        cout << "0. Exit" << endl;
        cout << "choice: ";
        int choice;
        cin >> choice;
        if (choice == 1) viewMenu();
        else if (choice == 2) customerPanel();
        else if (choice == 3) waiterPanel();
        else if (choice == 4) managerPanel();
        else if (choice == 0) { cout << "bye! visit again" << endl; return; }
        else cout << "invalid choice, try again" << endl;
    }
}

void customerPanel() {
    int choice;
    cout << endl << "customer portal" << endl;
    cout << "1. Register" << endl;
    cout << "2. View my details/history" << endl;
    cout << "choice: ";
    cin >> choice;
    if (choice == 1) {
        string name, phone;
        cout << "your name: ";
        cin >> name;
        cout << "phone no: ";
        cin >> phone;
        if (findCustomerByPhone(phone) != -1) { cout << "this phone no is already registered!" << endl; return; }
        int newId = nextCustomerId++;
        customers.push_back(Customer(newId, name, phone));
        cout << "registered! welcome " << name << ", your customer id is " << newId << endl;
    } else if (choice == 2) {
        string phone;
        cout << "phone no: ";
        cin >> phone;
        int cIdx = findCustomerByPhone(phone);
        if (cIdx == -1) { cout << "not found, please register first" << endl; return; }
        customers[cIdx].showDetails();
        bool anyOrders = false;
        cout << "your orders:" << endl;
        for (int i = 0; i < orders.size(); i++) {
            if (orders[i].getCustomerId() == customers[cIdx].getId()) {
                cout << "order #" << orders[i].getOrderId() << ", table " << orders[i].getTableNumber()
                     << ", status " << orders[i].getStatus()
                     << ", total Rs" << fixed << setprecision(2) << orders[i].getGrandTotal() << endl;
                anyOrders = true;
            }
        }
        if (!anyOrders) cout << "no orders found" << endl;
    } else {
        cout << "invalid choice" << endl;
    }
}

void waiterPanel() {
    string waiterName, password;
    cout << "waiter name: ";
    cin >> waiterName;
    cout << "password: ";
    cin >> password;
    if (password != "waiter123") { cout << "wrong password!" << endl; return; }

    char choice;
    while (true) {
        cout << endl << "waiter panel (" << waiterName << ")" << endl;
        cout << "T - view tables, N - new order, A - add item, U - update status, B - bill, E - exit" << endl;
        cout << "choice: ";
        cin >> choice;

        if (choice == 'T' || choice == 't') {
            for (int i = 0; i < tables.size(); i++) {
                cout << "table " << tables[i].getNumber() << " (" << tables[i].getCapacity() << " seater) - ";
                if (tables[i].isOccupied()) cout << "occupied, order #" << tables[i].getCurrentOrderId() << endl;
                else cout << "free" << endl;
            }

        } else if (choice == 'N' || choice == 'n') {
            int tableNum;
            cout << "table number: ";
            cin >> tableNum;
            int tIdx = findTableIndex(tableNum);
            if (tIdx == -1) { cout << "no such table" << endl; continue; }
            if (tables[tIdx].isOccupied()) { cout << "table already occupied!" << endl; continue; }
            Order newOrder(nextOrderId, tableNum, waiterName);

            char regChoice;
            cout << "customer already registered? (y/n): ";
            cin >> regChoice;
            if (regChoice == 'Y' || regChoice == 'y') {
                string phone;
                cout << "their phone no: ";
                cin >> phone;
                int cIdx = findCustomerByPhone(phone);
                if (cIdx == -1) cout << "not found, treating as walk in" << endl;
                else {
                    newOrder.setCustomer(customers[cIdx].getId(), customers[cIdx].getName(), customers[cIdx].getPhone());
                    cout << "welcome back " << customers[cIdx].getName() << endl;
                }
            }

            viewMenu();
            cout << "add items, enter 0 when done" << endl;
            while (true) {
                int itemId;
                cout << "item id: ";
                cin >> itemId;
                if (itemId == 0) break;
                bool found = false;
                for (int i = 0; i < menu.size(); i++) {
                    if (menu[i].getId() == itemId && menu[i].isAvailable()) {
                        int qty;
                        cout << "qty for " << menu[i].getName() << ": ";
                        cin >> qty;
                        newOrder.addItem(OrderItem(menu[i].getName(), menu[i].getPrice(), qty));
                        cout << "added " << menu[i].getName() << " x" << qty << endl;
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "item not found / unavailable" << endl;
            }
            if (newOrder.isEmpty()) { cout << "no items were added so cancelling this order" << endl; continue; }
            tables[tIdx].occupy(nextOrderId);
            orders.push_back(newOrder);
            cout << "order #" << nextOrderId << " created for table " << tableNum << endl;
            nextOrderId++;

        } else if (choice == 'A' || choice == 'a') {
            int orderId;
            cout << "order id: ";
            cin >> orderId;
            int oIdx = findOrderIndex(orderId);
            if (oIdx == -1) { cout << "order not found" << endl; continue; }
            if (orders[oIdx].getStatus() == "Billed") { cout << "this order is already billed" << endl; continue; }
            viewMenu();
            while (true) {
                int itemId;
                cout << "item id (0 to stop): ";
                cin >> itemId;
                if (itemId == 0) break;
                bool found = false;
                for (int i = 0; i < menu.size(); i++) {
                    if (menu[i].getId() == itemId && menu[i].isAvailable()) {
                        int qty;
                        cout << "qty: ";
                        cin >> qty;
                        orders[oIdx].addItem(OrderItem(menu[i].getName(), menu[i].getPrice(), qty));
                        cout << menu[i].getName() << " added" << endl;
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "item not found" << endl;
            }

        } else if (choice == 'U' || choice == 'u') {
            int orderId;
            cout << "order id: ";
            cin >> orderId;
            int oIdx = findOrderIndex(orderId);
            if (oIdx == -1) { cout << "order not found" << endl; continue; }
            if (orders[oIdx].isPaid()) { cout << "already billed, cant change status" << endl; continue; }
            cout << "current status: " << orders[oIdx].getStatus() << endl;
            cout << "1 Pending  2 Preparing  3 Served, new status: ";
            int s;
            cin >> s;
            string statuses[4] = { "", "Pending", "Preparing", "Served" };
            if (s >= 1 && s <= 3) { orders[oIdx].setStatus(statuses[s]); cout << "status set to " << statuses[s] << endl; }
            else cout << "invalid choice" << endl;

        } else if (choice == 'B' || choice == 'b') {
            int orderId;
            cout << "order id: ";
            cin >> orderId;
            int oIdx = findOrderIndex(orderId);
            if (oIdx == -1) { cout << "order not found" << endl; continue; }
            orders[oIdx].showBill();
            if (!orders[oIdx].isPaid()) {
                cout << "mark as paid? (y/n): ";
                char c;
                cin >> c;
                if (c == 'Y' || c == 'y') {
                    string mode = selectPaymentMode();
                    int tIdx = findTableIndex(orders[oIdx].getTableNumber());
                    orders[oIdx].markPaid(mode);
                    if (tIdx != -1) tables[tIdx].free();
                    int cIdx = findCustomerById(orders[oIdx].getCustomerId());
                    if (cIdx != -1) customers[cIdx].recordOrder(orders[oIdx].getGrandTotal());
                    cout << "payment done via " << mode << ", table " << orders[oIdx].getTableNumber() << " freed" << endl;
                    orders[oIdx].showBill();
                }
            }

        } else if (choice == 'E' || choice == 'e') {
            break;
        } else {
            cout << "invalid choice" << endl;
        }
    }
}

void managerPanel() {
    string username, password;
    cout << "admin username: ";
    cin >> username;
    if (username != "ADMIN") { cout << "wrong username" << endl; return; }
    cout << "admin password: ";
    cin >> password;
    if (password != "ADMIN123") { cout << "wrong password" << endl; return; }

    int choice;
    while (true) {
        cout << endl << "manager panel" << endl;
        cout << "1 menu(all)  2 add item  3 toggle avail  4 update price  5 all orders  6 tables  7 revenue  8 customers  0 exit" << endl;
        cout << "choice: ";
        cin >> choice;

        if (choice == 1) {
            viewMenu();
            bool anyUnavail = false;
            for (int i = 0; i < menu.size(); i++) {
                if (!menu[i].isAvailable()) {
                    if (!anyUnavail) { cout << "unavailable items:" << endl; anyUnavail = true; }
                    cout << menu[i].getId() << ") " << menu[i].getName() << " (" << menu[i].getCategory() << ")" << endl;
                }
            }
            if (!anyUnavail) cout << "everything is available rn" << endl;

        } else if (choice == 2) {
            string name, cat;
            double price;
            cout << "item name: ";
            cin >> name;
            cout << "category (Starter/Main/Beverage/Dessert): ";
            cin >> cat;
            cout << "price: ";
            cin >> price;
            menu.push_back(MenuItem(nextMenuId++, name, cat, price));
            cout << "added to menu" << endl;

        } else if (choice == 3) {
            viewMenu();
            int id;
            cout << "item id to toggle: ";
            cin >> id;
            bool found = false;
            for (int i = 0; i < menu.size(); i++) {
                if (menu[i].getId() == id) {
                    menu[i].setAvailable(!menu[i].isAvailable());
                    cout << menu[i].getName() << " is now " << (menu[i].isAvailable() ? "available" : "unavailable") << endl;
                    found = true;
                    break;
                }
            }
            if (!found) cout << "item not found" << endl;

        } else if (choice == 4) {
            viewMenu();
            int id;
            double newPrice;
            cout << "item id: ";
            cin >> id;
            cout << "new price: ";
            cin >> newPrice;
            bool found = false;
            for (int i = 0; i < menu.size(); i++) {
                if (menu[i].getId() == id) {
                    menu[i].setPrice(newPrice);
                    cout << "price updated for " << menu[i].getName() << endl;
                    found = true;
                    break;
                }
            }
            if (!found) cout << "item not found" << endl;

        } else if (choice == 5) {
            if (orders.size() == 0) { cout << "no orders yet" << endl; continue; }
            for (int i = 0; i < orders.size(); i++) orders[i].showBill();

        } else if (choice == 6) {
            for (int i = 0; i < tables.size(); i++) {
                cout << "table " << tables[i].getNumber() << ", cap " << tables[i].getCapacity() << " - ";
                if (tables[i].isOccupied()) cout << "occupied (order #" << tables[i].getCurrentOrderId() << ")" << endl;
                else cout << "free" << endl;
            }

        } else if (choice == 7) {
            double revenue = 0, cashRev = 0, upiRev = 0, otherRev = 0;
            int billedCount = 0;
            for (int i = 0; i < orders.size(); i++) {
                if (orders[i].isPaid()) {
                    double amt = orders[i].getGrandTotal();
                    revenue += amt;
                    billedCount++;
                    if (orders[i].getPaymentMode() == "Cash") cashRev += amt;
                    else if (orders[i].getPaymentMode() == "UPI") upiRev += amt;
                    else otherRev += amt;
                }
            }
            cout << "billed: " << billedCount << ", pending: " << (orders.size() - billedCount) << endl;
            cout << "cash Rs" << fixed << setprecision(2) << cashRev << ", upi Rs" << upiRev << ", other Rs" << otherRev << endl;
            cout << "TOTAL revenue: Rs" << revenue << endl;

        } else if (choice == 8) {
            if (customers.size() == 0) { cout << "no customers yet" << endl; continue; }
            for (int i = 0; i < customers.size(); i++) customers[i].showDetails();

        } else if (choice == 0) {
            break;
        } else {
            cout << "invalid choice" << endl;
        }
    }
}

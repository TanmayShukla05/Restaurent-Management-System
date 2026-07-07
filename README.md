# Grandeur Restaurant — Restaurant Management System

A simple console-based Restaurant Management System built in C++ to practice Object-Oriented Programming concepts. The project simulates how a small restaurant manages customers, tables, orders, billing, and different user roles through a menu-driven interface.

## Features

- Register customers with their name and phone number. Every customer is assigned a unique Customer ID automatically.
- Customers can later search their details and previous orders using their registered phone number.
- Manage 6 restaurant tables with different seating capacities and track whether each table is free or occupied.
- Waiters can create orders for tables, optionally link them to registered customers, and add multiple menu items with quantities.
- Order status can be updated from **Pending** → **Preparing** → **Served** → **Billed**.
- Bills are generated automatically with:
  - Subtotal
  - CGST (2.5%)
  - SGST (2.5%)
  - Grand Total
- Supports payment through **UPI**, **Cash**, or **Other**.
- After payment, the table becomes available again, and the customer's visit count and total spending are updated.
- Manager panel allows:
  - Adding and updating menu items
  - Changing item availability
  - Viewing all orders
  - Checking table status
  - Viewing daily revenue by payment mode
  - Viewing registered customer details

## Classes Used

| Class | Purpose |
|-------|---------|
| `MenuItem` | Stores information about each menu item |
| `OrderItem` | Represents one item in an order with its quantity |
| `Order` | Handles ordered items, bill calculation, GST and payment details |
| `Table` | Stores table capacity, availability and current order |
| `Customer` | Stores customer information, visit count and total spending |

The project mainly focuses on applying OOP concepts such as classes, objects, encapsulation, and modular design.

## Compilation

Linux/macOS

```bash
g++ -std=c++17 restaurant_management.cpp -o restaurant_management
./restaurant_management
```

Windows (MinGW)

```bash
g++ -std=c++17 restaurant_management.cpp -o restaurant_management.exe
restaurant_management.exe
```

## Login Credentials

### Manager
- Username: `ADMIN`
- Password: `ADMIN123`

### Waiter
- Username: Any name
- Password: `waiter123`

## Example Usage

1. Register a customer.
2. Log in as a waiter and assign a table.
3. Add menu items to the order.
4. Generate the bill and complete the payment.
5. Log in as the manager to check revenue, orders, table status, or customer records.

## Future Improvements

- Save data to files or a database so it isn't lost after closing the program.
- Add discounts, coupons, or loyalty points.
- Allow order cancellation and item removal.
- Generate printable receipts.
- Add table reservation support.

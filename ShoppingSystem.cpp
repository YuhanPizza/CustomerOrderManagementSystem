// ***********************
// Name: Lorenz Alvin Tubo
// ***********************

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <occi.h>

#include <string>
#include <sstream>
#include <cstring>

#include <iomanip>

struct ShoppingCart {
    int product_id;
    double price;
    int quantity;
};


using oracle::occi::Environment;
using oracle::occi::Connection;

using namespace oracle::occi;
using namespace std;
using std::setw;

int mainMenu();
int subMenu(); 
void customerService(Connection* conn, int customerId);
void displayOrderStatus(Connection* conn, int orderId, int customerId); // you write this function
void cancelOrder(Connection* conn, int orderId, int customerId); // you write this function
void createEnvironement(Environment* env);
void openConnection(Environment* env, Connection* conn, string user, string pass, string constr);
void closeConnection(Connection* conn, Environment* env);
void teminateEnvironement(Environment* env);
int customerLogin(Connection* conn, int customerId);
double findProduct(Connection* conn, int product_id);
int addToCart(Connection* conn, struct ShoppingCart cart[]);
void displayProducts(struct ShoppingCart cart[], int productCount);
int checkout(Connection* conn, struct ShoppingCart cart[], int customerId, int productCount);


int main(void)
{
    int option;
    /* OCCI Variables */
    Environment* env = nullptr;
    Connection* conn = nullptr;
    //Statement* stmt = nullptr;
    //ResultSet* rs = nullptr;

    /* Used Variables */
    string str;
    string user = "dbs311_231nhh33";
    string pass = "18416633";
    string constr = "myoracle12c.senecacollege.ca:1521/oracle12c";


    try {
        // create environment and Open the connection
        env = Environment::createEnvironment(Environment::DEFAULT);
        conn = env->createConnection(user, pass, constr);
        cout<<"Connected!" << endl;

        int customerId = 0;

        do {
            
            option = mainMenu();
            switch (option) {

            case 1:

                cout << "Enter the customer ID: ";
                cin >> customerId;
               
                if (customerLogin(conn, customerId) == 1) {
                    //call customerService()
                    customerService(conn, customerId);
                }
                else {
                    cout << "customer does not exist." << endl;
                }
                break;
            case 0:
                cout << "Good bye!..." << endl;
                break;
                

            }
        } while (option != 0);
        env->terminateConnection(conn);
        Environment::terminateEnvironment(env);

    }

    catch (SQLException& sqlExcp) {
        cout << "error";
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }

    return 0;
}

int mainMenu() {
    int option = 1;
    do {
        // diplay the menu options
        cout << "******************** Main Menu ********************" << endl;
        cout << "1)	Login" << endl;
        cout << "0)	Exit" << endl;
        // read an option value

        if (option < 0 || option > 1) {

            cout << "You entered a wrong value. Enter an option (0-1): ";
        }
        else {
            cout << "Enter an option (0-1): ";

        }

        cin >> option;

    } while (option < 0 || option > 1);

    return option;
}

int subMenu() {
    int opt = 1;

    do {
        // diplay the menu options
        cout << "******************** Customer Service Menu ********************" << endl;
        cout << "1) Place an order" << endl;
        cout << "2) Check an order status" << endl;
        cout << "3) Cancel an order" << endl;
        cout << "0) Exit" << endl;
        // read an option value

        if (opt < 0 || opt > 3) {

            cout << "You entered a wrong value. Enter an option (0-1): ";
        }
        else {
            cout << "Enter an option (0-3): ";

        }

        cin >> opt;

    } while (opt < 0 || opt > 3);

    return opt;
}

void customerService(Connection* conn, int customerId) {
    struct ShoppingCart cart[5];
    int checkedout = 0;
    int productCount;
    int orderId;
    int opt;

    do {

        opt = subMenu();
        switch (opt) {

        case 1:

            cout << ">-------- Place an order ---------<";
            struct ShoppingCart cart[5];
            productCount = addToCart(conn, cart);
            displayProducts(cart, productCount);
            checkedout = checkout(conn, cart, customerId, productCount);
            if (checkedout) {
               cout << "The order is successfully completed." << endl;
            }
            else {
               cout << "The order is cancelled." << endl;
            }
            break;
        case 2:
            cout << ">-------- Check the order status --------<" << endl;
            cout << "Enter an order ID: ";
            cin >> orderId;
            displayOrderStatus(conn, orderId, customerId);
            break;
        case 3:
            
            cout << ">-------- Cancel an Order --------<" << endl;
            cout << "Enter an order ID: ";
            cin >> orderId;
            cancelOrder(conn, orderId, customerId);
            break;
        case 0:
            cout << "Back to main menu!..." << endl;
            break;

        }
    } while (opt != 0);

}

//Complete this function
void displayOrderStatus(Connection* conn, int orderId, int customerId) {
    string status;
    double total_price;
    Statement* stmt = conn->createStatement();
    stmt->setSQL("SELECT STATUS, TOTAL_PRICE FROM ORDERS WHERE ORDER_ID = :orderId AND CUSTOMER_ID = :customerId");
    stmt->setInt(1, orderId);
    stmt->setInt(2, customerId);
    ResultSet* rs = stmt->executeQuery();
    if (rs->next()) {
        status = rs->getString(1);
        total_price = rs->getDouble(2);
        cout << "Order ID: " << orderId << endl;
        cout << "Status: " << status << endl;
        cout << "Total Price: $" << fixed << setprecision(2) << total_price << endl;
    }
    else {
        cout << "The order does not exist." << endl;
    }
    conn->terminateStatement(stmt);
}

//Complete this function
void cancelOrder(Connection* conn, int orderId, int customerId) {
    Statement* stmt = conn->createStatement();
    stmt->setSQL("UPDATE ORDERS SET STATUS = 'CANCELLED' WHERE ORDER_ID = :orderId AND CUSTOMER_ID = :customerId");
    stmt->setInt(1, orderId);
    stmt->setInt(2, customerId);
    int rows = stmt->executeUpdate();
    if (rows == 1) {
        cout << "The order has been cancelled." << endl;
    }
    else {
        cout << "The order does not exist or cannot be cancelled." << endl;
    }
    conn->terminateStatement(stmt);
}

void createEnvironement(Environment* env) {
    try {
        env = Environment::createEnvironment(Environment::DEFAULT);
        cout << "environment created" << endl;
    }
    catch (SQLException& sqlExcp) {
        cout << "error";
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }

}

void teminateEnvironement(Environment* env) {
    Environment::terminateEnvironment(env);
}

void openConnection(Environment* env, Connection* conn, string user, string pass, string constr) {
    try {
        conn = env->createConnection(user, pass, constr);
    }
    catch (SQLException& sqlExcp) {
        cout << "error";
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }
}

void closeConnection(Connection* conn, Environment* env) {
    env->terminateConnection(conn);
}

int customerLogin(Connection* conn, int customerId) {

    Statement* stmt = nullptr;
    int found = 0;
    stmt = conn->createStatement("BEGIN find_customer(:1, :2); END;");
    stmt->setInt(1, customerId);
    stmt->registerOutParam(2, Type::OCCIINT, sizeof(found));
    stmt->executeUpdate();
    found = stmt->getInt(2);
    conn->terminateStatement(stmt);

    return found;

}

int addToCart(Connection* conn, struct ShoppingCart cart[]) {
    int product_id = 0;
    int productCount = 0;
    int addMore = 1;
    double price = 0;

    cout << "-------------- Add Products to Cart --------------" << endl;
    for (int i = 0; i < 5 && addMore == 1; i++) {
        do {
            cout << "Enter the product ID: ";
            cin >> cart[i].product_id;

            // call a stored procedure/function to check if the product exists
            //store the returning value in the price variable
            price = findProduct(conn, cart[i].product_id);

            /*if the price is zero, the product does not exist
              if the price is greater than zero the product display the following 
              output:
              Product Price:*/
            if (price != 0) {
                cout << "Product Price: " << price << endl;
                cart[i].price = price;
            }
            else {
                cout << "The product does not exists. Try again..." << endl;
            }


        } while (price == 0);

        cout << "Enter the product Quantity: ";
        cin >> cart[i].quantity;

        productCount++;

        cout << "Enter 1 to add more products or 0 to checkout: ";
        cin >> addMore;

    }

    return productCount;

}

double findProduct(Connection* conn, int productId) {
    Statement* stmt = nullptr;
    double found = 0;
    stmt = conn->createStatement("BEGIN find_product(:1, :2); END;");
    stmt->setInt(1, productId);
    stmt->registerOutParam(2, Type::OCCIDOUBLE, sizeof(found));
    stmt->executeUpdate();
    found = stmt->getDouble(2);
    conn->terminateStatement(stmt);

    return found;
}

void displayProducts(struct ShoppingCart cart[], int productCount) {

    double total = 0;
    cout << "------- Ordered Products ---------" << endl;
    for (int i = 0; i < productCount; i++) {
        cout << "---Item " << i + 1 << endl;
        cout << "Product ID: " << cart[i].product_id << endl;
        cout << "Price: " << cart[i].price << endl;
        cout << "Quantity: " << cart[i].quantity << endl;
        total = total + cart[i].price * cart[i].quantity;
    }

    cout << "----------------------------------" << endl;
    cout << "Total: " << total << endl;

}

int checkout(Connection* conn, struct ShoppingCart cart[], int customerId, int productCount) {
    char confirm = ' ';
    int exit = 1;
    do {
        cout << "Would you like to checkout? (Y/y or N/n) ";
        cin >> confirm;
        if (confirm == 'Y' || confirm == 'y') {
            exit = 0;
        }
        else if (confirm == 'N' || confirm == 'n') {
            exit = 1;
        }
        else {
            cout << "Wrong input. Try again..." << endl;
        }
    } while (confirm != 'N' && confirm != 'n' && confirm != 'Y' && confirm != 'y');

    if (!exit) {
        Statement* stmt = nullptr;
        int order_id = 0;
        stmt = conn->createStatement("BEGIN add_order(:1, :2); END;");
        stmt->setInt(1, customerId);
        stmt->registerOutParam(2, Type::OCCIDOUBLE, sizeof(order_id));
        stmt->executeUpdate();
        order_id = stmt->getDouble(2);

        // Add items
        for (int i = 0; i < productCount; i++) {
            stmt->setSQL("BEGIN add_order_item(:1, :2, :3, :4, :5); END;");
            stmt->setInt(1, order_id);
            stmt->setInt(2, i + 1);
            stmt->setInt(3, cart[i].product_id);
            stmt->setInt(4, cart[i].quantity);
            stmt->setDouble(5, cart[i].price);
            stmt->executeUpdate();
        }

        conn->commit();
        conn->terminateStatement(stmt);

    }

    return 1;
}


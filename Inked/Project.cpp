/*
    Author : Mia Murray
    Date   : April 15th 2021
    Title  : CSCI 370 - Final Project

    "Inked" is a simple database application program for a Tattoo Parlor Booking System
    that features an easy to use command line interface. Inked allows an employee to log in with
    their employee number on the command line, and the Inked experience from there will revolve
    around that employee. This booking system has been designed with the use case that the tattoo
    artists own the shop, and they do not have a receptionist. Because of this, they need an
    easy to use application that allows them to do most of the heavy lifting in their day to 
    day operations. 
    
    The shop has a consistent hourly rate of $150, and there is a 24 hour cancellation policy
    where a customer can cancel at any time, but within 24 hours of their appointment time they
    will have to pay a cancellation fee of $150 (1 hour of services). The shop is open every day
    from 9am to 5pm.

    The functionalities include:
    -Keep a record of all customers of the shop, including ones who have cancelled appointments
    -Book new appointments for customers, and keep track of their cancel date
    -Add new customers, must be done before making a new appointment with a new customer
    -Input tattoo info once completed, and generate a cost for the customer (lengthx$150)
    -View all upcoming appointments (remember, centric to the logged in employee)
    -View any potential touch up dates for tattoos (not appointments, seperate, free, informal)
    -View number of tattoos a customer has had at the shop (in customer look up)
    -View number of tattoos an employee has given (in employee + shop metrics)
    -View overall total revenue employee, and shop in metrics

    NOTE*
    -This application is designed for a single artist owned shop, it cannot handle multiple
    shop locations due to potential database conflicts.
    -I am assuming that this application has a seperate view for administrative tasks
    like adding/removing employees and setting the shop rate
    --> This project is for only one business use case, so only the normal employee view is implemented
    -It is assumed that the employee themself must make their own appointments, hense
    once logged in, employees can only book/access their own appointments.
    -->This is ok for the business, because this product is designed with a small business in mind,
    and small businesses quite often know other employee ID's (Like restaurants, POS log in).
    -->That being said, they are able to access any information they need about a customer by
    looking them up, so they can see their upcoming appointments with any artist that way
    -Appointments cannot be booked past 2022
    -It is assumed that an employee will check if a customer exists before enrolling them
    --> duplicate names are allowed because they occur in real life
*/
#include <iostream>
#include <occi.h>
#include <termios.h>
#include <unistd.h>
#include <string.h> //library for strlen()
#include <string>
#include <sstream> //library for converting string to int
#include <limits>
#include "Project.hpp"
using namespace std;
using namespace oracle::occi;

// GLOBAL VARIABLES
int emp_ID = 0;
string emp_name = "";

int main(int argc, char *argv[])
{
    // VARIABLES
    string sqlString = "";
    const string connectString = "sunfire.csci.viu.ca";

    // Get the employee ID provided as a command line argument
    // If 
    if(argc != 2)
    {
        cerr << "Error: Please enter an employee ID as a command line argument." << endl;
        return 0;
    }
    // convert string to int to match database
    string temp = argv[1];
    stringstream convert(temp);
    convert >> emp_ID;

    // this would be hardcoded in the customers distribution, but for the purposes of keeping
    // my password private, I have prompts so that you may enter your credentials
    cout << "Enter Oracle sqlplus Username & Password          \n\n";
    cout <<endl;

    string username;
    cout << "Username: ";
    cin >> username;
    cout << endl;
    cout << "Password: ";
    string password = readPassword(); // want to note this isn't necessary for the use case, as above
    cout << endl;

    clearscreen();

    try
    {
        Environment *env = Environment::createEnvironment();
        Connection *conn = env->createConnection(username, password, connectString);

        // Checking emp_ID is valid:
        sqlString = "SELECT emp_name FROM Artist WHERE emp_ID = :1";
        Statement *stmtEmp = conn->createStatement(sqlString);
        stmtEmp->setInt(1, emp_ID); //replacing variable with cardNo
        ResultSet *rsEmp = stmtEmp->executeQuery();
        //  If no results, employee doesn't exist in the system
        // only need to show first tuple as emp_ID is unique, and a match only returns one tuple
        rsEmp->next();
        if (rsEmp->status() == 0)
        {
            cerr << "Error: No artist associated with that employee ID.\n";
            cerr << "Inked shutting down... Try again.\n";
            cout << endl;
            return 0;
        }else
        {
            emp_name = rsEmp->getString(1);
        }
        
        char end;
        while (end != 'Q') //while the employee hasn't logged out yet
        {
            end = menu(conn, env);
        }
        stmtEmp->closeResultSet(rsEmp);
        conn->terminateStatement(stmtEmp);
        env->terminateConnection(conn);
        Environment::terminateEnvironment(env);
    }catch (SQLException  &e)
    {
        cout << e.what();
        return 0;
    }
    return 0;
}

char menu(Connection *conn, Environment *env)
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    clearscreen();
    cout << "------------------------------HELLO " << emp_name
         << "---------------------------------" << endl;
    cout << endl;
    cout << "                                Welcome to Inked\n";
    cout << endl;

    char ch;
    bool valid = false;
    int app_ID = 0;
    int cust_ID = 0;
    int tattoo_ID = 0;
    float tattoo_cost = 0;
    char prompt;

    cout << "------------------Please select one of the following options---------------\n";
    cout << endl;
    cout << "                     [ Enter 'N' : New Appointment            ]\n";
    cout << "                     [ Enter 'A' : Add New Customer           ]\n";
    cout << "                     [ Enter 'V' : View Upcoming Appointments ]\n";
    // finished tattoo, because artist will enter tattoo info and calculate cost right after
    cout << "                     [ Enter 'F' : Finished Tattoo            ]\n";
    cout << "                     [ Enter 'C' : Customer Look Up           ]\n";
    cout << "                     [ Enter 'M' : Employee and Shop Metrics  ]\n";
    cout << "                     [ Enter 'Q' : Log Out                    ]\n";
    while (valid == false)
    {
        cin >> ch;
        ch = toupper(ch);
        switch (ch)
        {
            case 'N':
                app_ID = gen_app_ID(conn);
                make_appointment(conn, env, app_ID);
                valid = true;
                return 'N';
            case 'A':
                cust_ID = gen_cust_ID(conn);
                add_customer(conn, cust_ID);
                valid = true;
                return 'A';
            case 'C':
                get_customer(conn);
                valid = true;
                return 'C';
            case 'F':
                tattoo_ID = gen_tattoo_ID(conn);
                tattoo_cost = new_tattoo(conn, env, tattoo_ID);
                clearscreen();
                if (tattoo_cost != 0) cout << "Charge Customer: " << tattoo_cost << endl;
                do 
                {
                    cout << '\n' << "Press a key to continue...";
                } while (cin.get() != '\n');
                valid = true;
                return 'F';
            case 'V':
                upcoming_appointments(conn, env);
                valid = true;
                return 'V';
            case 'M':
                cout << "Employee and Shop Metrics\n";
                generate_metrics(conn);
                valid = true;
                return 'M';
            case 'Q':
                clearscreen();
                cout << "Logging out " << emp_name << "...\n" << endl;
                return 'Q';
            default:
                cout << "Invalid command, please try again\n";
                valid = false;
                break;
        }
    }
}

string readPassword()
{
    struct termios settings;
    tcgetattr( STDIN_FILENO, &settings );
    settings.c_lflag = (settings.c_lflag & ~(ECHO));
    tcsetattr( STDIN_FILENO, TCSANOW, &settings );

    string password = "";
    cin >> password;
    settings.c_lflag = (settings.c_lflag | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &settings );
    return password;
}

int gen_app_ID(Connection *conn)
{
    int app_ID = 0;
    string temp = "";
    string sqlString = "SELECT max(app_ID) FROM Appointment";
    Statement *stmtID = conn->createStatement(sqlString);
    ResultSet *rsID = stmtID->executeQuery();
    //  If no results, no appointments are in system yet, return 1 for app_ID
    rsID->next();
    if (rsID->status() == 0)
    {
        stmtID->closeResultSet(rsID);
        conn->terminateStatement(stmtID);
        return 1;
    }else
    {
        temp = rsID->getString(1);
        stringstream convert(temp);
        convert >> app_ID;
        app_ID++; //increment app_ID by 1
        stmtID->closeResultSet(rsID);
        conn->terminateStatement(stmtID);
        return app_ID;
    }
}

int gen_cust_ID(Connection *conn)
{
    int cust_ID = 0;
    string temp = "";
    string sqlString = "SELECT max(cust_ID) FROM Customer";
    Statement *stmtID = conn->createStatement(sqlString);
    ResultSet *rsID = stmtID->executeQuery();
    //  If no results, no customers are in system yet, return 1 for cust_ID
    rsID->next();
    if (rsID->status() == 0)
    {
        stmtID->closeResultSet(rsID);
        conn->terminateStatement(stmtID);
        return 1;
    }else
    {
        temp = rsID->getString(1);
        stringstream convert(temp);
        convert >> cust_ID;
        cust_ID++; //increment cust_ID by 1
        stmtID->closeResultSet(rsID);
        conn->terminateStatement(stmtID);
        return cust_ID;
    }
}

int gen_tattoo_ID(Connection *conn)
{
    int tattoo_ID = 0;
    string temp = "";
    string sqlString = "SELECT max(tattoo_ID) FROM Tattoo";
    Statement *stmtID = conn->createStatement(sqlString);
    ResultSet *rsID = stmtID->executeQuery();
    //  If no results, no tattoos are in system yet, return 1 for tattoo_ID
    rsID->next();
    if (rsID->status() == 0)
    {
        stmtID->closeResultSet(rsID);
        conn->terminateStatement(stmtID);
        return 1;
    }else
    {
        temp = rsID->getString(1);
        stringstream convert(temp);
        convert >> tattoo_ID;
        tattoo_ID++; //increment tattoo_ID by 1
        stmtID->closeResultSet(rsID);
        conn->terminateStatement(stmtID);
        return tattoo_ID;
    }
}

void make_appointment(Connection *conn, Environment *env, int app_ID) 
{   // all new appointments have status = upcoming
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    clearscreen();

    string cust_name = "";
    string temp = "";
    int cust_ID = 0;
    //below is clunky, but cant find a way to access Date member attributes
    int cancelmonth = 0;
    int cancelday = 0;
    int year = 0;
    int hour = 0;
    int minute = 0;

    // no need to check app_ID
    string sqlString = "INSERT INTO Appointment(app_ID, emp_ID, cust_ID, app_date, cancel_date, app_status) VALUES "
                       "(:1, :2, :3, :4, :5, 'upcoming')";
    Statement *stmtApp = conn->createStatement(sqlString);
    stmtApp->setInt(1, app_ID);
    stmtApp->setInt(2, emp_ID);

    // GETTING CUSTOMER NAME
    cout << "Please enter the firstname and lastname of the customer: ";
    getline(cin, cust_name);
    // Checking name matches cust_ID:
    string sqlString2 = "SELECT cust_ID FROM Customer WHERE cust_name = :1";
    Statement *stmtCust = conn->createStatement(sqlString2);
    stmtCust->setString(1, cust_name); //replacing variable with customer name
    ResultSet *rsCust = stmtCust->executeQuery();
    //  If no results, customer doesn't exist in the system
    // Prompt employee to check name and send them back to menu (they can select 'N' again)
    rsCust->next();
    if (rsCust->status() == 0)
    {
        cerr << "Error: No customer associated with that name.\n";
        cerr << "Please check the spelling of the name and try again from Menu.\n";
        cerr << "If the customer is new, you must select Add Customer first to enroll them.\n";
        cout << endl;
        return;
    }else
    {
        temp = rsCust->getString(1);
        stringstream convert(temp);
        convert >> cust_ID;
    }
    stmtApp->setInt(3, cust_ID);


    // SETTING DATE, AND SETTING CANCELLATION DATE
    Date edate = get_date(env);
    stmtApp->setDate(4, edate);
    edate = edate.addDays(-1);

    stmtApp->setDate(5, edate);

    int insertStatus = stmtApp->executeUpdate();
    if (insertStatus == 1)
    {
        clearscreen();
        cout << "Appointment booked!" << endl;
        conn->commit(); //commit insert
    }else
    {
        cerr << "Error: Something went wrong :/\n";
    }
    do 
    {
        cout << '\n' << "Press a key to continue...";
    } while (cin.get() != '\n');
    conn->terminateStatement(stmtApp);
    stmtCust->closeResultSet(rsCust);
    conn->terminateStatement(stmtCust);
    return;
}

void get_customer(Connection *conn)
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    clearscreen();

    string cust_name = "";
    string temp = "";
    int cust_ID = 0;
    string email = "";
    string phone = "";
    int tattoo_count = 0;

    // GETTING CUSTOMER ID
    cout << "Please enter the firstname and lastname of the customer: ";
    getline(cin, cust_name);
    // Checking name matches cust_ID:
    string sqlString = "SELECT cust_ID FROM Customer WHERE cust_name = :1";
    Statement *stmtCust = conn->createStatement(sqlString);
    stmtCust->setString(1, cust_name); //replacing variable with customer name
    ResultSet *rsCust = stmtCust->executeQuery();
    //  If no results, customer doesn't exist in the system
    // Prompt employee to check name and send them back to menu (they can select 'N' again)
    rsCust->next();
    if (rsCust->status() == 0)
    {
        cerr << "Error: No customer associated with that name.\n";
        cerr << "Please check the spelling of the name and try again from Menu.\n";
        cerr << "If the customer is new, you must select Add Customer first to enroll them.\n";
        cout << endl;
        return;
    }else
    {
        temp = rsCust->getString(1);
        stringstream convert(temp);
        convert >> cust_ID;
    }
    stmtCust->closeResultSet(rsCust);

    sqlString = "SELECT cust_ID, cust_name, email, phone, amount_owing FROM Customer WHERE cust_ID = :1";
    stmtCust = conn->createStatement(sqlString);
    stmtCust->setInt(1, cust_ID);
    rsCust = stmtCust->executeQuery();
    rsCust->next();
    clearscreen();
    cout << "CUSTOMER INFO:\n";
    cout << "-------------------------------------------------------------------------" << endl;
    cout << "   Customer ID: " << rsCust->getString(1) << endl;
    cout << "     Full Name: " << rsCust->getString(2) << endl;
    cout << "         Email: " << rsCust->getString(3) << endl;
    cout << "         Phone: " << rsCust->getString(4) << endl;
    cout << "  Amount Owing: " << rsCust->getString(5) << endl;

    sqlString = "SELECT count(tattoo_ID) FROM Tattoo INNER JOIN Customer "
                "ON customer.cust_ID = tattoo.cust_ID WHERE cust_name = :1";
    Statement *stmtCount = conn->createStatement(sqlString);
    stmtCount->setString(1, cust_name);
    ResultSet *rsCount = stmtCount->executeQuery();
    rsCount->next();
    temp = rsCount->getString(1);
    stringstream convert(temp);
    convert >> tattoo_count;

    cout << "       Tattoos: " << tattoo_count << endl;

    stmtCust->closeResultSet(rsCount);
    conn->terminateStatement(stmtCount);
    stmtCust->closeResultSet(rsCust);
    conn->terminateStatement(stmtCust);
    return;
}

void add_customer(Connection *conn, int cust_ID)
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    clearscreen();

    string cust_name = "";
    string email = "";
    string phone = "";

    string sqlString = "INSERT INTO Customer(cust_ID, cust_name, email, phone, amount_owing) VALUES "
                       "(:1, :2, :3, :4, 0)"; //amount owing starts at 0
    Statement *stmtinsert = conn->createStatement(sqlString);
    stmtinsert->setInt(1, cust_ID);

    cout << "Please enter the customer name in format firstname lastname: ";
    getline(cin, cust_name); // we are assuming if the name is a duplicate it's intentional
    stmtinsert->setString(2, cust_name);

    cout << "Please enter the customer email: ";
    cin >> email;
    stmtinsert->setString(3, email);

    cout << "Please enter the customer phone: ";
    cin >> phone;
    stmtinsert->setString(4, phone);

    int insertStatus = stmtinsert->executeUpdate();
    if (insertStatus == 1)
    {
        conn->commit();
    }else
    {
        cerr << "Error: Something went wrong :/\n";
    }
    conn->terminateStatement(stmtinsert);
    return;
}

float new_tattoo(Connection *conn, Environment *env, int tattoo_ID)
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    clearscreen();

    string temp = "";
    string cust_name = "";
    int cust_ID = 0;
    string email = "";
    string phone = "";
    float length = 0;

    string sqlString = "INSERT INTO Tattoo(tattoo_ID, emp_ID, cust_ID, tattoo_date, tattoo_length, cost, touchup_date) VALUES "
                       "(:1, :2, :3, :4, :5, :6, :7)";
    Statement *stmtinsert = conn->createStatement(sqlString);
    stmtinsert->setInt(1, tattoo_ID);
    stmtinsert->setInt(2, emp_ID);

    // GETTING CUSTOMER ID
    cout << "Please enter the firstname and lastname of the customer: ";
    getline(cin, cust_name);
    // Checking name matches cust_ID:
    string sqlString2 = "SELECT cust_ID FROM Customer WHERE cust_name = :1";
    Statement *stmtCust = conn->createStatement(sqlString2);
    stmtCust->setString(1, cust_name); //replacing variable with customer name
    ResultSet *rsCust = stmtCust->executeQuery();
    //  If no results, customer doesn't exist in the system
    // Prompt employee to check name and send them back to menu (they can select 'N' again)
    rsCust->next();
    if (rsCust->status() == 0)
    {
        cerr << "Error: No customer associated with that name.\n";
        cerr << "Please check the spelling of the name and try again from Menu.\n";
        cerr << "If the customer is new, you must select Add Customer first to enroll them.\n";
        cout << endl;
        return 0;
    }else
    {
        temp = rsCust->getString(1);
        stringstream convert(temp);
        convert >> cust_ID;
    }
    stmtCust->closeResultSet(rsCust);
    conn->terminateStatement(stmtCust);
    stmtinsert->setInt(3, cust_ID);

    // SETTING DATE
    Date edate = get_date(env);
    stmtinsert->setDate(4, edate);

    cout << "Please enter the tattoo length in hours (i.e 1.5, 2.25): ";
    cin >> length;
    stmtinsert->setInt(5, length);

    float cost = length*150;
    stmtinsert->setInt(6, cost);

    cout << "Does this tattoo require a touchup? (Y/N): ";
    char touchup;
    cin >> touchup;
    touchup = toupper(touchup);
    while (touchup != 'N' && touchup != 'Y')
    {
        cin >> touchup;
        touchup = toupper(touchup);
    }
    if (touchup == 'Y')
    {
        cout << "Okay! Gathering touch up date..." << endl;
        Date tdate = get_date(env);
        stmtinsert->setDate(7, tdate);
    }else
    {
        //didnt realise date in oracle is not nullable, so going to enter the tattoo completion
        //date for touchup, so artist doesnt get confused
        stmtinsert->setDate(7, edate);
    }
    int insertStatus = stmtinsert->executeUpdate();

    unsigned int t = 0;
    int f = 0;
    unsigned int month = 0;
    unsigned int day = 0;
    edate.getDate(f, month, day, t, t, t);
    if (insertStatus == 1)
    {
        // WE NEED TO UPDATE THE APPOINTMENT ASSOCIATED WITH COMPLETED STATUS
        sqlString = "UPDATE Appointment SET app_status = 'complete' "
                   "WHERE EXTRACT(MONTH FROM app_date) = :1 AND EXTRACT(DAY FROM app_date) = :2 "
                   "AND cust_ID = :3"; // a customer wont have two appointments in the same day
        Statement *stmtupdate = conn->createStatement(sqlString);
        stmtupdate->setInt(1, month);
        stmtupdate->setInt(2, day);
        stmtupdate->setInt(3, cust_ID);
        int updateStatus = stmtupdate->executeUpdate();
        if (updateStatus == 1)
        {
            conn->commit();
        }else
        {
            cerr << "Error: Something went wrong :/\n";
        }
        conn->terminateStatement(stmtupdate);
        conn->commit();
    }else
    {
        cerr << "Error: Something went wrong :/\n";
    }
    conn->terminateStatement(stmtinsert);
    return cost;
}

void upcoming_appointments(Connection *conn, Environment *env)
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    clearscreen();

    string temp = ""; // for result set
    string sqlString;
    bool flag = false;

    cout << "-------------------------APPOINTMENTS + TOUCHUPS-------------------------------\n";
    cout << endl;

    // GETTING UPCOMING APPOINTMENTS CENTRIC TO THE ARTIST LOGGED IN
    sqlString = "SELECT cust_name, app_date, cancel_date FROM Appointment A "
                "JOIN Customer C ON C.cust_ID = A.cust_ID "
                "WHERE app_date > sysdate AND emp_ID = :1 AND app_status = 'upcoming'";
    Statement *stmt1 = conn->createStatement(sqlString);
    stmt1->setInt(1, emp_ID); // replacing variable with emp_ID
    ResultSet *rs1 = stmt1->executeQuery();
    //  If no results, there are no upcoming appointments for the artist
    while (rs1->next())
    {
        if (flag == false) cout << emp_name << " your upcoming appointments are: "
                                                "    CUSTOMER "
                "        APPOINTMENT      CANCELLATION FEE DUE AFTER" << endl;
        flag = true;
        cout << "                                                "
             << rs1->getString(1) << "  "
             << rs1->getString(2) << "        "
             << rs1->getString(3) << endl;
    }
    stmt1->closeResultSet(rs1);
    conn->terminateStatement(stmt1);

    // GETTING UPCOMING TOUCHUPS CENTRIC TO THE ARTIST LOGGED IN
    flag = false;
    sqlString = "SELECT cust_name, touchup_date FROM Tattoo T "
                "JOIN Customer C ON C.cust_ID = T.cust_ID " 
                "WHERE touchup_date > sysdate AND emp_ID = :1";
    Statement *stmt2 = conn->createStatement(sqlString);
    stmt2->setInt(1, emp_ID); // replacing variable with emp_ID
    ResultSet *rs2 = stmt2->executeQuery();
    //  If no results, there are no upcoming touchups for the artist
    while (rs2->next())
    {
        if (flag == false) cout << emp_name << " your upcoming touch ups are: "
                                               "       CUSTOMER         TOUCH UP DATE " << endl;
        flag = true;
        cout << "                                                "
             << rs2->getString(1) << "  "
             << rs2->getString(2) << endl;
    }
    stmt2->closeResultSet(rs2);
    conn->terminateStatement(stmt2);
    do 
    {
        cout << '\n' << "Press a key to continue...";
    } while (cin.get() != '\n');
    return;
}

void generate_metrics(Connection *conn)
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    clearscreen();

    string cust_name = "";
    string temp = "";
    int cust_ID = 0;
    string email = "";
    string phone = "";
    int tattoo_count = 0;

    cout << "------------------------------HELLO " << emp_name
         << "---------------------------------" << endl;
    cout << endl;

    // GETTING SHOP METRICS
    string sqlString = "SELECT count(tattoo_ID) FROM Tattoo"; //total tattoos completed
    Statement *stmt = conn->createStatement(sqlString);
    ResultSet *rs = stmt->executeQuery();
    rs->next();
    cout << "                                Shop Metrics\n";
    cout << endl;
    cout << "         Total Tattoos Given: " << rs->getString(1) << endl;
    stmt->closeResultSet(rs);
    conn->terminateStatement(stmt);

    sqlString = "SELECT count(cust_ID) FROM Appointment WHERE app_status = 'upcoming'";
    stmt = conn->createStatement(sqlString);
    rs = stmt->executeQuery();
    rs->next();
    cout << " Total Upcoming Appointments: " << rs->getString(1) << endl;
    stmt->closeResultSet(rs);
    conn->terminateStatement(stmt);

    sqlString = "SELECT count(cust_ID) FROM Appointment WHERE app_status = 'cancelled'";
    stmt = conn->createStatement(sqlString);
    rs = stmt->executeQuery();
    rs->next();
    cout << "Total Cancelled Appointments: " << rs->getString(1) << endl;
    stmt->closeResultSet(rs);
    conn->terminateStatement(stmt);

    sqlString = "SELECT sum(cost) FROM Tattoo";
    stmt = conn->createStatement(sqlString);
    rs = stmt->executeQuery();
    rs->next();
    cout << "               Total Revenue: " << rs->getString(1) << endl;
    stmt->closeResultSet(rs);
    conn->terminateStatement(stmt);

    // GETTING ARTIST METRICS
    cout << endl;
    cout << "                               Artist Metrics\n";
    cout << endl;
    sqlString = "SELECT count(tattoo_ID) FROM Tattoo where emp_ID = :1";
    stmt = conn->createStatement(sqlString);
    stmt->setInt(1, emp_ID);
    rs = stmt->executeQuery();
    rs->next();
    cout << "         Total Tattoos Given: " << rs->getString(1) << endl;
    stmt->closeResultSet(rs);
    conn->terminateStatement(stmt);

    sqlString = "SELECT count(cust_ID) FROM Appointment WHERE app_status = 'upcoming' AND emp_ID = :1";
    stmt = conn->createStatement(sqlString);
    stmt->setInt(1, emp_ID);
    rs = stmt->executeQuery();
    rs->next();
    cout << " Total Upcoming Appointments: " << rs->getString(1) << endl;
    stmt->closeResultSet(rs);
    conn->terminateStatement(stmt);

    sqlString = "SELECT count(cust_ID) FROM Appointment WHERE app_status = 'cancelled' AND emp_ID = :1";
    stmt = conn->createStatement(sqlString);
    stmt->setInt(1, emp_ID);
    rs = stmt->executeQuery();
    rs->next();
    cout << "Total Cancelled Appointments: " << rs->getString(1) << endl;
    stmt->closeResultSet(rs);
    conn->terminateStatement(stmt);

    sqlString = "SELECT sum(cost) FROM Tattoo WHERE emp_ID = :1";
    stmt = conn->createStatement(sqlString);
    stmt->setInt(1, emp_ID);
    rs = stmt->executeQuery();
    rs->next();
    cout << "               Total Revenue: " << rs->getString(1) << endl;
    stmt->closeResultSet(rs);
    conn->terminateStatement(stmt);
    do 
    {
        cout << '\n' << "Press a key to continue...";
    } while (cin.get() != '\n');
    return;
}

Date get_date(Environment *env)
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    clearscreen();
    //below is clunky, but cant find a way to access Date member attributes
    int month = 0;
    int day = 0;
    int year = 0;
    int hour = 0;
    int minute = 0;

    cout << "Please enter the month in format MM: ";
    cin >> month;
    while (month < 1 || month > 12)
    {
        cerr << "Invalid month, try again (MM): ";
        cin >> month;
    }

    cout << "Please enter the day in format DD: ";
    cin >> day;
    while (day < 1 || day > 31)
    {
        cerr << "Invalid day, try again (DD): ";
        cin >> day;
    }

    cout << "Please enter the year in format YYYY: ";
    cin >> year;
    while (year < 2021 || year > 2022)
    {
        cerr << "Invalid year, try again (YYYY): ";
        cin >> year;
    }

    cout << "Please enter the hour in format 24: ";
    cin >> hour;
    while (hour < 9 || hour > 17) // shop is open from 9 to 5 every day
    {
        cerr << "Invalid time, try again (24): ";
        cin >> hour;
    }

    cout << "Please enter the appointment minutes in format MI: ";
    cin >> minute;
    while (minute < 0 || minute > 59)
    {
        cerr << "Invalid time, try again (MI): ";
        cin >> minute;
    }
    Date edate(env, year, month, day, hour, minute, 0);
    return edate;
}

void clearscreen()
{
    cout << string(100, '\n');
}


/* Header file for 370 Project */
using namespace std;
using namespace oracle::occi;

string readPassword(); // not in Inked, just for project
void clearscreen(); // used to improve UI
Date get_date(Environment *env); // user input for date is needed in many areas

char menu(Connection *conn, Environment *env); // runs until return value is Q -> quit

int gen_app_ID(Connection *conn); // checks database for max(app_ID), increments by one
int gen_cust_ID(Connection *conn); // checks database for max(cust_ID), increments by one
int gen_tattoo_ID(Connection *conn); // checks database for max(tattoo_ID), increments by one

void make_appointment(Connection *conn, Environment *env, int app_ID); // app_ID is generated outside function, is unique
void get_customer(Connection *conn); // employee needs to look up by name, not ID
void add_customer(Connection *conn, int cust_ID); // adds a new customer into the system
float new_tattoo(Connection *conn, Environment *env, int tattoo_ID); // tattoo_ID is generated outside function, is unique
void upcoming_appointments(Connection *conn, Environment *env);
void generate_metrics(Connection *conn);




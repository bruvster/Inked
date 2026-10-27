CREATE TABLE Customer(
    cust_ID int not null primary key,
    cust_name varchar(20) not null,
    email varchar(25),
    phone varchar(12),
    amount_owing number(7,2)
);

CREATE TABLE Artist(
    emp_ID int not null primary key,
    emp_name varchar(15)
);

CREATE TABLE Appointment(
    app_ID int not null primary key,
    emp_ID int not null,
    cust_ID int not null,
    app_date date,
    cancel_date date,
    app_status varchar(10),
    FOREIGN KEY(emp_ID) REFERENCES Artist,
    FOREIGN KEY(cust_ID) REFERENCES Customer
);

CREATE TABLE Tattoo(
    tattoo_ID int not null primary key,
    emp_ID int not null,
    cust_ID int not null,
    tattoo_date date, /* might differ from appointment date, best to keep seperate */ 
    tattoo_length number(7,2) not null, 
    cost number(7,2) not null,
    touchup_date date,
    FOREIGN KEY(emp_ID) REFERENCES Artist,
    FOREIGN KEY(cust_ID) REFERENCES Customer
);
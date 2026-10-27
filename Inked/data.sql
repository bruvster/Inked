INSERT INTO Customer(cust_ID, cust_name, email, phone, amount_owing) VALUES
    (1, 'Karma Lee', 'karma@dmail.com', '2501267452', 0);

INSERT INTO Customer(cust_ID, cust_name, email, phone, amount_owing) VALUES
    (2, 'Richard Smith', 'richboy@dmail.com', '2502356734', 0);

INSERT INTO Customer(cust_ID, cust_name, email, phone, amount_owing) VALUES
    (3, 'Jesus Gonzales', 'crystalsidings@dmail.com', '2509762946', 0);

INSERT INTO Customer(cust_ID, cust_name, email, phone, amount_owing) VALUES
    (4, 'Silas Maltesen', 'silees@dmail.com', '2508261143', 0);

INSERT INTO Customer(cust_ID, cust_name, email, phone, amount_owing) VALUES
    (5, 'Mia Emilie', 'mimi@dmail.com', '2502440033', 0);

INSERT INTO Customer(cust_ID, cust_name, email, phone, amount_owing) VALUES
    (6, 'Natalie Nguyen', 'sunsetlover@dmail.com', '2500927394', 0);

INSERT INTO Customer(cust_ID, cust_name, email, phone, amount_owing) VALUES
    (7, 'Abdul Malik', 'dreambig@dmail.com', '2500193756', 150.00);

INSERT INTO Customer(cust_ID, cust_name, email, phone, amount_owing) VALUES
    (8, 'Karma Lee', 'karma@dmail.com', '2501111111', 0);

INSERT INTO Artist(emp_ID, emp_name) VALUES
    (1, 'Jules LeFlame');

INSERT INTO Artist(emp_ID, emp_name) VALUES
    (2, 'Winston Black');

INSERT INTO Appointment(app_ID, emp_ID, cust_ID, app_date, cancel_date, app_status) VALUES
    (1, 1, 1, TO_DATE('04/01/2021 09:00:00', 'MM/DD/YYYY HH24:MI:SS'), TO_DATE('03/31/2021 09:00:00', 'MM/DD/YYYY HH24:MI:SS'), 'complete');

INSERT INTO Appointment(app_ID, emp_ID, cust_ID, app_date, cancel_date, app_status) VALUES
    (2, 2, 6, TO_DATE('05/01/2021 12:00:00', 'MM/DD/YYYY HH24:MI:SS'), TO_DATE('04/30/2021 12:00:00', 'MM/DD/YYYY HH24:MI:SS'), 'complete');

INSERT INTO Appointment(app_ID, emp_ID, cust_ID, app_date, cancel_date, app_status) VALUES
    (3, 1, 4, TO_DATE('05/06/2021 11:00:00', 'MM/DD/YYYY HH24:MI:SS'), TO_DATE('05/05/2021 11:00:00', 'MM/DD/YYYY HH24:MI:SS'), 'complete');

INSERT INTO Appointment(app_ID, emp_ID, cust_ID, app_date, cancel_date, app_status) VALUES
    (4, 2, 7, TO_DATE('05/02/2021 11:00:00', 'MM/DD/YYYY HH24:MI:SS'), TO_DATE('05/01/2021 11:00:00', 'MM/DD/YYYY HH24:MI:SS'), 'cancelled');

INSERT INTO Appointment(app_ID, emp_ID, cust_ID, app_date, cancel_date, app_status) VALUES
    (5, 1, 2, TO_DATE('07/02/2021 13:00:00', 'MM/DD/YYYY HH24:MI:SS'), TO_DATE('07/01/2021 13:00:00', 'MM/DD/YYYY HH24:MI:SS'), 'upcoming');

INSERT INTO Appointment(app_ID, emp_ID, cust_ID, app_date, cancel_date, app_status) VALUES
    (6, 2, 3, TO_DATE('10/07/2021 14:00:00', 'MM/DD/YYYY HH24:MI:SS'), TO_DATE('10/06/2021 14:00:00', 'MM/DD/YYYY HH24:MI:SS'), 'upcoming');

INSERT INTO Appointment(app_ID, emp_ID, cust_ID, app_date, cancel_date, app_status) VALUES
    (7, 2, 5, TO_DATE('10/08/2021 15:00:00', 'MM/DD/YYYY HH24:MI:SS'), TO_DATE('10/07/2021 15:00:00', 'MM/DD/YYYY HH24:MI:SS'), 'upcoming');

INSERT INTO Tattoo(tattoo_ID, emp_ID, cust_ID, tattoo_date, tattoo_length, cost, touchup_date) VALUES
    (1, 1, 1, TO_DATE('05/01/2021 12:30:00', 'MM/DD/YYYY HH24:MI:SS'), 3.5, 525.00, null);

INSERT INTO Tattoo(tattoo_ID, emp_ID, cust_ID, tattoo_date, tattoo_length, cost, touchup_date) VALUES
    (2, 2, 6, TO_DATE('05/01/2021 14:00:00', 'MM/DD/YYYY HH24:MI:SS'), 2, 300.00, null);

INSERT INTO Tattoo(tattoo_ID, emp_ID, cust_ID, tattoo_date, tattoo_length, cost, touchup_date) VALUES
    (3, 1, 4, TO_DATE('05/06/2021 12:00:00', 'MM/DD/YYYY HH24:MI:SS'), 1, 150.00, TO_DATE('06/06/2021 12:00:00', 'MM/DD/YYYY HH24:MI:SS'));

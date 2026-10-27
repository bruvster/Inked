# Inked
 
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

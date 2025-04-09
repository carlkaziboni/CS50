# Suggestions
#### Video Demo:  <https://youtu.be/ZAPsmDCv_OQ>
#### Description:
This is a project where students, parents, teachers or anyone submit suggestions to the student council and the suggestions are responded to by the current Student Council members or staff that are involved. This webapp allows people to view if their suggestions were approved or unapproved by the committee.

### Technologies used:
- Flask
- Bootstrap 4 and Bootstrap 5
- Flask-login
- Sqlite3
- Werkzeug

### Static

Site.css is used as the stylesheet for 'guests.html', which is used as a template for all the pages that do not reuqire a user logged in. Style.css is used to stylesheet for 'admin.html', which is used as a template for all the pages that require a logged in user.

### Templates and their respective functions

+ Home.html
  * index() - Where suggestions are entered guests

+ Completed.html
  * completed() - View Completed Suggestions

+ Progress.html
  * inprogress() - View suggestions currently under progress and how they will be done
  
+ Unapproved.html
  * unapproved() - View unapproved suggestions and their reasons

+ Suggestions.html - login is required
  * suggestions() - view suggestions submitted
  * addapproved() - Add suggestion to approved and move to progress
  * addunapproved() - Add suggestion to unapproved

+ Inprogress.html - login is required
  * progress() -View suggestions in progress
  * delay() - Add delay to progress suggestions
  * complete() - Move suggestion to completed

+ Minutes.html - login is required
  * minutes() - Add minutes to memory from meetings

+ Minuteshist.html - login is required
  * minuteshistory() - View history of minutes

+ Accounts.html - admin login is required
  * accounts() - View password of non-hashed passwords. View and create normal and admin accounts
  * delete() - Delete account

+ Login.html
  * login() - Where users logged in

+ Changepass.html
  * changepassword() - Allows user to changepassword

### Additional Functions

* Admin_required(f) - Wraps function that only allows logged in users with admin status into the route

### Suggestions.db

* Accounts - Manage users
  + Contains ID (Primary Key), Name, Username (Unique), Password, PasswordType, AccountType
* Approved - Approved suggestions
  + Contains ID (Primary Key), Suggestion, Method, DateCompleted, Delay, Suggestionref (Foreign Key references Suggestions(ID))
* Completed - Completed suggestions
  + Contains ID (Primary Key), Date, Suggestion, Method
* Suggestions - Collects suggestions
  + Contains ID (Primary Key), Forename, Surname, Form, Gender, Suggestion, Status
* Minutes - Stores minutes
  + Contains ID (Primary Key), Minute, Date
* Unapproved - Stores unapproved suggestions
  + Unapproved ID (Primary Key), Suggestion, Reason, Date, Suggestionref (Foreign Key references Suggestions(ID))

from flask import Flask, render_template, redirect, request, abort, session
import sqlite3, datetime, secrets, string
from flask_login import LoginManager, login_user, logout_user, login_required
from werkzeug.security import check_password_hash, generate_password_hash
from functools import wraps

login_manager = LoginManager()

login_manager.login_view = '/login'

app = Flask(__name__)
login_manager.init_app(app)

app.config['SECRET_KEY'] = '192b9bdd22ab9ed4d12e236c78afcb9a393ec15f71bbf5dc987d54727823bcbf'

class User:
    def __init__(self, id):
        self.id = id
    
    def is_authenticated(self):
        return True
    
    def is_active(self):
        return True
    
    def is_anonymous(self):
        return False
    
    def get_id(self):
        return self.id

@login_manager.user_loader
def load_user(user_id):
    return User(user_id)

def admin_required(f):
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get('admin') is None:
            return redirect('/suggestions')
        return f(*args, **kwargs)
    return decorated_function

@app.route("/", methods =['GET', 'POST'])
def index():
    if request.method == 'GET':
        return render_template('home.html')
    elif request.method == 'POST':
        forename = request.form.get('name')
        surname = request.form.get('surname')
        formclass = request.form.get('identification')
        gender = request.form.get('gender')
        suggestiontext = request.form.get('suggestiontext')
        with sqlite3.connect('suggestions.db') as con:
            cur = con.cursor()
            cur.execute('INSERT INTO suggestions (Forename, Surname, Form, Gender, Suggestion) VALUES (?, ?, ?, ?, ?)', (forename, surname, formclass, gender, suggestiontext))
            con.commit()

        return render_template('home.html')
    return abort(400)

@app.route("/completed")
def completed():
    with sqlite3.connect('suggestions.db') as con:
        cur = con.cursor()
        completedval = cur.execute('SELECT Date, Suggestion, Method FROM Completed').fetchall()
    return render_template('completed.html', completedval = completedval)

@app.route("/inprogress")
def inprogress():
    with sqlite3.connect('suggestions.db') as con:
        cur = con.cursor()
        inprogress = cur.execute('SELECT Suggestion, Method, DateCompleted, Delay FROM Approved').fetchall()
    return render_template('progress.html', inprogress = inprogress)

@app.route('/unapproved')
def unapproved():
    with sqlite3.connect('suggestions.db') as con:
        cur = con.cursor()
        unapprovedlist = cur.execute('SELECT Suggestion, Reason, Date FROM Unapproved')
    return render_template('unapproved.html', unapprovedlist = unapprovedlist)

@app.route('/suggestions', methods=['GET'])
@login_required
def suggestions():
    if request.method == 'GET':
        with sqlite3.connect('suggestions.db') as con:
            cur = con.cursor()
            suggestionval = cur.execute('SELECT suggestion, ID FROM suggestions WHERE Status = "none"').fetchall()
        return render_template('suggestions.html', suggestionval = suggestionval)
    return abort(400)

@app.route('/addapproved', methods=['POST'])
@login_required
def addapproved():
    id = request.form.get('suggestionid')
    date = request.form.get('date')
    method = request.form.get('MOC')
    with sqlite3.connect('suggestions.db') as con:
        cur = con.cursor()
        suggestion = cur.execute('SELECT suggestion FROM suggestions WHERE ID = ?', (id,)).fetchall()
        cur.execute('INSERT INTO Approved (Suggestion, Method, DateCompleted, Suggestionref) VALUES (?, ?, ?, ?)', (suggestion[0][0], method, date, id))
        cur.execute('UPDATE Suggestions SET Status = "Approved" WHERE ID = ?', (id,))
    return redirect("/suggestions")

@app.route('/addunapproved', methods=['POST'])
@login_required
def addunapproved():
    id = request.form.get('suggestionid')
    reason = request.form.get('reason')
    day = datetime.date.today()
    with sqlite3.connect('suggestions.db') as con:
        cur = con.cursor()
        suggestion = cur.execute('SELECT suggestion FROM suggestions WHERE ID = ?', (id,)).fetchall()
        cur.execute('INSERT INTO Unapproved (Suggestion, Reason, Suggestionref, Date) VALUES (? ,? ,?, ?)',(suggestion[0][0], reason, id, day))
        cur.execute('UPDATE Suggestions SET Status = "Unapproved" WHERE ID = ?', (id,))
    return redirect("/suggestions")

@app.route('/progress', methods=['GET'])
@login_required
def progress():
    with sqlite3.connect('suggestions.db') as con:
        cur = con.cursor()
        approved = cur.execute('SELECT ID, Suggestion, Method, Datecompleted, Delay FROM Approved').fetchall()
    return render_template('inprogress.html', approved = approved)

@app.route('/delay', methods=['POST'])
@login_required
def delay():
    id = request.form.get('id')
    delayval = request.form.get('delay-num')
    with sqlite3.connect('suggestions.db') as con:
        cur = con.cursor()
        cur.execute('UPDATE Approved SET Delay = ? WHERE ID = ?', (delayval, id))
    return redirect('/progress')

@app.route('/complete', methods=['POST'])
@login_required
def complete():
    present = datetime.date.today()
    suggestionid = request.form.get('suggestionid')
    with sqlite3.connect('suggestions.db') as con:
        cur = con.cursor()
        newvals = cur.execute('SELECT Suggestion, Method FROM Approved WHERE ID = ?', (suggestionid,)).fetchall()
        cur.execute('INSERT INTO Completed (Date, Suggestion, Method) VALUES (?, ?, ?)', (present, newvals[0][0], newvals[0][1]))
        cur.execute('DELETE FROM Approved WHERE ID = ?', (suggestionid,))
    return redirect('/progress')

@app.route('/minutes', methods=['GET', 'POST'])
@login_required
def minutes():
    if request.method == 'GET':
        return render_template('minutes.html')
    elif request.method == 'POST':
        day = datetime.date.today()
        minute = request.form.get('minute')
        with sqlite3.connect('suggestions.db') as con:
            cur = con.cursor()
            cur.execute('INSERT INTO Minutes (Minute, Date) VALUES (?, ?)', (minute, day))
        return redirect('/minutes')
    return abort(400)

@app.route('/minuteshistory', methods=['GET'])
@login_required
def minuteshistory():
    with sqlite3.connect('suggestions.db') as con:
        cur = con.cursor()
        minute = cur.execute('SELECT Minute, Date, ID FROM Minutes').fetchall()
    return render_template('minuteshist.html', minute=minute)

@app.route('/accounts', methods = ['GET', 'POST'])
@login_required
@admin_required
def accounts():
    if request.method == 'GET':
        with sqlite3.connect('suggestions.db') as con:
            cur = con.cursor()
            users = cur.execute('SELECT ID, Name, AccountType, PasswordType, Password FROM Accounts').fetchall()
        return render_template('accounts.html', users=users)
    elif request.method == 'POST':
        name = request.form.get('name')
        username = request.form.get('username')
        accounttype = request.form.get('accounttype')
        passwordtype = 'UNHASHED'
        password = ''.join(secrets.choice(string.ascii_letters + string.punctuation) for i in range(8))
        with sqlite3.connect('suggestions.db') as con:
            cur = con.cursor()
            usernamecheck = cur.execute('SELECT * FROM Accounts WHERE Username = ?', (username,)).fetchone()
            if usernamecheck != None:
                return abort(400)
            cur.execute('INSERT INTO Accounts (Name, Username, Password, Accounttype, PasswordType) VALUES (?, ?, ?, ?, ?)', (name, username, password, accounttype, passwordtype))
        return redirect('/accounts')
    
@app.route('/delete', methods = ['POST'])
@login_required
def delete():
    deleteid = request.form.get('deleteid')
    with sqlite3.connect('suggestions.db') as con:
        cur = con.cursor()
        cur.execute('DELETE FROM Accounts WHERE ID = ?', (deleteid))
    return redirect('/accounts')

@app.route('/login', methods=['POST', 'GET'])
def login():
    if request.method == 'POST':
        if session.get('_user_id') is not None:
            return redirect('/logout')
        username = request.form.get('username')
        password = request.form.get('password')
        with sqlite3.connect('suggestions.db') as con:
            cur = con.cursor()
            verify = cur.execute('SELECT ID, Username, Password, PasswordType, AccountType FROM Accounts WHERE Username = ?', (username,)).fetchall()
            if len(verify) == 0:
                return redirect('/login')
            if verify[0][3] == 'UNHASHED':
                if password == verify[0][2]:
                    user_id = load_user(str(verify[0][0]))
                    if verify[0][4] == 'admin':
                        session['admin'] = True
                    login_user(user_id)
                else:
                    return abort(400)
            elif verify[0][3] == 'HASHED':
                    if check_password_hash(verify[0][2], password):
                        user_id = load_user(str(verify[0][0]))
                        if verify[0][4] == 'admin':
                            session['admin'] = True
                        login_user(user_id)
                    else:
                        return abort(400)
            return redirect('/suggestions')
    if session.get('_user_id') is not None:
        return redirect('/logout')
    return render_template('login.html')

@app.route('/changepassword', methods=['GET', 'POST'])
@login_required
def changepassword():
    if request.method == 'POST':
        password = request.form.get('password')
        passwordconfirm = request.form.get('passwordconfirm')
        if password != passwordconfirm:
            redirect('/changepassword')
        passwordhash = generate_password_hash(password, method = 'pbkdf2:sha256', salt_length=16)
        ID = session.get("_user_id")
        with sqlite3.connect('suggestions.db') as con:
            cur = con.cursor()
            cur.execute('UPDATE Accounts SET Password = ?, PasswordType = ? WHERE ID = ?', (passwordhash, 'HASHED', ID))
        return redirect('/suggestions')
    return render_template('changepass.html')

@app.route('/logout')
@login_required
def logout():
    logout_user()
    return redirect('/')
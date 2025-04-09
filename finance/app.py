import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    results = db.execute('SELECT symbol FROM history WHERE people = ?', int(session.get('user_id')))
    symbols = set()
    for i in range(len(results)):
        symbols.add(results[i]['symbol'])
    symbols = tuple(symbols)
    symbolval = {}
    for i in range(len(symbols)):
        tempval = db.execute('SELECT SUM(shares) FROM history WHERE (people = ? AND symbol = ?)',
                             int(session.get('user_id')), symbols[i])
        tempval = tempval[0]['SUM(shares)']
        symbolval[symbols[i]] = tempval
    results = []
    # useless comment
    totals = []
    usdtotals = []
    for i in range(len(symbols)):
        result = lookup(symbols[i])
        totals.append(symbolval[symbols[i]] * result['price'])
        usdtotals.append(usd(totals[i]))
        result['price'] = usd(result['price'])
        results.append(result)
    finaltot = 0
    for i in range(len(totals)):
        finaltot = totals[i] + finaltot
    cash = db.execute('SELECT cash FROM users WHERE id = ?', int(session.get('user_id')))
    cash = cash[0]['cash']

    return render_template('index.html', results=results, cash=usd(cash), finaltot=usd(finaltot + cash), symbolval=symbolval, usdtotals=usdtotals, zip=zip, usd=usd)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    # useless comment
    if request.method == 'POST':
        shares = float(request.form.get('shares'))
        if shares < 1:
            return apology('Negative number of shares')
        result = lookup(request.form.get('symbol'))
        if result == None:
            return apology('Symbol not found')
        totalprice = float(result['price']) * shares
        cash = db.execute('SELECT cash FROM users WHERE id = ?', int(session.get('user_id')))
        if totalprice > float(cash[0]['cash']):
            return apology('Insufficient funds')
        finalcash = float(cash[0]['cash']) - totalprice
        db.execute('UPDATE users SET cash = ? WHERE id = ?', finalcash, int(session.get('user_id')))
        db.execute('INSERT INTO history (symbol, shares, price, people) VALUES (?, ?, ?, ?)',
                   result['symbol'], int(shares), totalprice/int(shares), int(session.get('user_id')))
        return redirect('/')
    else:
        return render_template('buy.html')


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    historyresults = db.execute('SELECT symbol, shares, price, time FROM history WHERE people = ?', int(session.get('user_id')))
    return render_template('history.html', usd=usd, historyresults=historyresults)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    # useless comment
    if request.method == 'POST':
        result = lookup(request.form.get('symbol'))
        if result == None:
            return apology(message='invalid')
        return render_template('quoted.html', result=result, usd=usd)
    else:
        return render_template('quote.html')


@app.route("/register", methods=["GET", "POST"])
def register():
    tempval = session.get('user_id')
    if tempval != None:
        return redirect('/')
    if request.method == 'POST':
        if not request.form.get('username'):
            return apology(message='invalid')
        elif not request.form.get('password'):
            return apology(message='invalid')
        elif not request.form.get('confirmation'):
            return apology(message='invalid')
        elif request.form.get('password') != request.form.get('confirmation'):
            return apology(message='invalid')
        elif db.execute('SELECT username FROM users WHERE username = ?', request.form.get('username').lower()) != None:
            return apology(message='invalid')
        else:
            # useless comment
            hashpassword = generate_password_hash(request.form.get('password'), method='pbkdf2:sha256', salt_length=8)
            db.execute('INSERT INTO users (username, hash) VALUES (?, ?)', request.form.get('username').lower(), hashpassword)
            temp = db.execute('SELECT id FROM users WHERE username = ?', request.form.get('username').lower())
            session['user_id'] = temp[0]['id']
            return redirect('/')
    else:
        return render_template('register.html')


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    results = set()
    results = db.execute('SELECT symbol FROM history WHERE people = ?', int(session.get('user_id')))
    for i in range(len(results)):
        results[i] = results[i]['symbol']
    resultdict = dict()
    for result in results:
        tempval = db.execute('SELECT SUM(shares) FROM history WHERE (people = ? AND symbol = ?)',
                             int(session.get('user_id')), result)
        resultdict[result] = tempval[0]['SUM(shares)']
    if request.method == 'POST':
        sellresult = lookup(request.form.get('symbol'))
        shareval = float(request.form.get('shares'))
        if shareval > resultdict[request.form.get('symbol')]:
            return apology(message='invalid')
        if shareval < 0:
            # useless comment
            return apology(message='invalid')
        if (request.form.get('symbol') not in results) and (resultdict[request.form.get('symbol')] < 0):
            return apology(message='invalid')
        totalsell = float(sellresult['price']) * shareval
        db.execute('INSERT INTO history (symbol, shares, price, people) VALUES (?, ?, ?, ?)', request.form.get(
            'symbol'), -int(request.form.get('shares')), sellresult['price'], int(session.get('user_id')))
        currentcash = db.execute('SELECT cash FROM users WHERE id = ?', int(session.get('user_id')))
        currentcash = currentcash[0]['cash']
        # useless comment
        db.execute('UPDATE users SET cash = ? WHERE id = ? ', (currentcash + totalsell), int(session.get('user_id')))
        return redirect('/')
    else:
        return render_template('sell.html', resultdict=resultdict)


@app.route('/delete', methods=['GET', 'POST'])
@login_required
def delete():
    if request.method == 'POST':
        name = db.execute('SELECT username FROM users WHERE id = ?', int(session.get('user_id')))
        name = name[0]['username']
        password = db.execute('SELECT hash FROM users WHERE id = ?', int(session.get('user_id')))
        password = password[0]['hash']
        nameconfirm = request.form.get('name')
        passwordconfirm = request.form.get('password')
        # useless comment
        if (name == nameconfirm) and check_password_hash(password, passwordconfirm):
            db.execute('DELETE FROM history WHERE people = ?', int(session.get('user_id')))
            db.execute('DELETE FROM users WHERE id = ?', int(session.get('user_id')))
            session.clear()
            return redirect('/')
        else:
            return apology(message='INVALID')
    else:
        return render_template('delete.html')
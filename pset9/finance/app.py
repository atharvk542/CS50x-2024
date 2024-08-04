import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
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
    currentId = session["user_id"]
    stocks = db.execute(
        "SELECT symbol, shares, price, holdingPrice FROM purchases WHERE user_id = ?", currentId)
    totalPrice = 0.0

    for row in stocks:
        l = lookup(row["symbol"])
        row["price"] = l["price"]
        row["holdingPrice"] = l["price"] * row["shares"]
        totalPrice += row["holdingPrice"]

    cash = float(db.execute("SELECT cash FROM users WHERE id = ?", currentId)[0]["cash"])
    grandTotal = totalPrice + cash
    return render_template("index.html", totalPrice=totalPrice, cash=cash, grandTotal=grandTotal, stocks=stocks)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")

    elif request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if symbol == "":
            return apology("Symbol is blank")

        elif lookup(symbol) == None:
            return apology("Symbol does not exist")

        if shares == "":
            return apology("# of shares does not exist")

        try:
            shares = float(shares)
        except ValueError:
            return apology("# of shares is not a number")

        if not shares > 0 or shares != int(shares):
            return apology("# of shares is not a positive integer")

        shares = int(shares)

        currentId = session["user_id"]
        price = lookup(symbol)["price"]
        userCash = float(db.execute("SELECT cash FROM users WHERE id = ?", currentId)[0]["cash"])

        if (shares * price) > userCash:
            return apology("You do not have enough money for the purchase.")

        db.execute("UPDATE users SET cash = ? WHERE id = ?",
                   userCash - shares * price, session["user_id"])
        db.execute("INSERT INTO purchases (user_id, symbol, shares, price, timestamp, holdingPrice) VALUES (?, ?, ?, ?, datetime('now'), ?)",
                   currentId, symbol, shares, price, shares * price)
        db.execute("INSERT INTO trades (id, symbol, price, shares, type) VALUES (?, ?, ?, ?, ?)",
                   currentId, symbol, price, shares, "BUY")

        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute("SELECT * FROM trades WHERE id = ?", session["user_id"])
    return render_template("history.html", transactions=transactions)


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
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
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
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    elif request.method == "POST":
        symbol = request.form.get("symbol")

        if symbol == "":
            return apology("Must enter a symbol")

        try:
            price = lookup(symbol)["price"]
        except TypeError:
            return apology("Stock does not exist")

        return render_template("quoted.html", symbol=symbol, price=price)
    else:
        return apology("How did you get here")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        name = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if name == "" or password == "" or confirmation == "":
            return apology("One or more of the inputs is blank.")

        if not password == confirmation:
            return apology("Passwords do not match. Please make sure the confirmation equals the password.")

        try:
            hashcode = generate_password_hash(password)
            db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", name, hashcode)
        except ValueError:
            return apology("Name is taken, please try another username.")

        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    currentId = session["user_id"]
    stocks = db.execute(
        "SELECT symbol, shares, price, holdingPrice FROM purchases WHERE user_id = ?", currentId)

    if request.method == "GET":

        return render_template("sell.html", stocks=stocks)

    elif request.method == "POST":

        symbol = request.form.get("symbol")
        shares = float(request.form.get("shares"))
        symbolRow = db.execute(
            "SELECT * FROM purchases WHERE user_id = ? AND symbol = ?", currentId, symbol)[0]

        if symbol == "":
            return apology("Did not select a stock")

        userShares = db.execute(
            "SELECT SUM(shares) AS total FROM purchases WHERE user_id = ? AND symbol = ?", currentId, symbol)[0]["total"]
        if userShares <= 0:
            return apology("Do not own any of requested stock")

        if shares > userShares:
            return apology("Invalid sell amount, do not own enough shares")

        if not shares > 0 or not shares == int(shares):
            return apology("Shares is not a positive integer")

        shares = int(shares)

        price = symbolRow["price"]

        db.execute("UPDATE purchases SET shares = ?, holdingPrice = ? WHERE user_id = ? AND symbol = ?",
                   userShares - shares, price * (userShares - shares), currentId, symbol)
        db.execute("INSERT INTO trades (id, symbol, price, shares, type) VALUES (?, ?, ?, ?, ?)",
                   currentId, symbol, price, shares, "SELL")

        oldCash = float(db.execute("SELECT cash FROM users WHERE id = ?", currentId)[0]["cash"])
        holdingPrice = db.execute(
            "SELECT holdingPrice FROM purchases WHERE user_id = ? AND symbol = ?", currentId, symbol)[0]["holdingPrice"]

        newShares = db.execute("SELECT SUM(shares) AS total FROM purchases WHERE user_id = ? AND symbol = ?",
                               currentId, symbol)[0]["total"]

        if newShares <= 0:
            db.execute("DELETE FROM purchases WHERE user_id = ? AND symbol = ?", currentId, symbol)

        db.execute("UPDATE users SET cash = ? WHERE id = ?", oldCash + holdingPrice, currentId)

        return redirect("/")

    else:
        return apology("Inappropriate http request")


@app.route("/balance", methods=["GET", "POST"])
@login_required
def balance():
    if request.method == "POST":
        amount = request.form.get("amount")

        if amount == "":
            return apology("Please enter a balance")

        amount = float(amount)

        if amount <= 0:
            return apology("Amount to add to balance must be positive")

        oldCash = float(db.execute("SELECT cash FROM users WHERE id = ?",
                        session["user_id"])[0]["cash"])
        db.execute("UPDATE users SET cash = ? WHERE id = ?", oldCash + amount, session["user_id"])

        return redirect("/")

    elif request.method == "GET":
        return render_template("balance.html")

    else:
        return apology("idk how you got here")

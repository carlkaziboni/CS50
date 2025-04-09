-- Keep a log of any SQL queries you execute as you solve the mystery.

--Read crime scene report
SELECT description
    FROM crime_scene_reports
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND street = "Humphrey Street";

--Get interview transcripts from witnesses
SELECT transcript
    FROM interviews
    WHERE transcript
    LIKE "%bakery%"
    AND year = 2021
    AND month = 7
    AND day = 28;

--Get names of people who left bakery within 10 mins of theft
SELECT name
    FROM people
    WHERE license_plate IN
    (SELECT license_plate
        FROM bakery_security_logs
        WHERE year = 2021
        AND month = 7
        AND day = 28
        AND hour = 10
        AND minute > 15
        AND minute < (15 + 10)
        AND activity = "exit");

--Get names of people who used the ATM Legget Street
SELECT name
    FROM people
    WHERE id IN
    (SELECT person_id
        FROM bank_accounts
        WHERE account_number IN
        (SELECT account_number
            FROM atm_transactions
            WHERE year = 2021
            AND month = 7
            AND day = 28
            AND atm_location = "Leggett Street"
            AND transaction_type = "withdraw"));

--Get names of people who called for less than a minute
SELECT name
    FROM people
    WHERE phone_number IN
    (SELECT caller
        FROM phone_calls
        WHERE year = 2021
        AND month = 7
        AND day = 28
        AND duration < 60);

-- Get earliest flight FROM Fiftyville on July 29
SELECT id
    FROM flights
    WHERE year = 2021
    AND month = 7
    AND day = 29
    AND origin_airport_id IN
    (SELECT id
        FROM airports
        WHERE city = "Fiftyville")
    ORDER BY hour asc, minute asc
    LIMIT 1;

-- Get names of all passenegers on flight 36
SELECT name
    FROM people
    WHERE passport_number IN
    (SELECT passport_number
        FROM passengers
        WHERE flight_id = 36);

--Get name of receiver of call from Bruce
SELECT id
    FROM phone_calls
    WHERE caller IN
    (SELECT phone_number
        FROM people
        WHERE name = "Bruce")
    AND year = 2021
    AND day = 28
    AND month = 7
    AND duration < 60;

SELECT name
    FROM people
    WHERE phone_number IN
    (SELECT receiver
        FROM phone_calls
        WHERE id = 233);
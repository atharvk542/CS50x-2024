-- Keep a log of any SQL queries you execute as you solve the mystery.

--This query was used to find when and where the CS50 duck theft occurred and led to the interviews detailing the theft.
SELECT * FROM crime_scene_reports WHERE street = 'Humphrey Street';

--This query revealed the interviews from the day of the crime, and the three interviews mentioning the bakery said that the theft happened within
--10 minutes of the theft occuring, (so within 10 minutes of 10:15), that the thief was at the ATM withdrawing money on Leggett Street before they did the theft,
-- and that they called someone for under a minute after leaving the bakery and booked the earliest flight out of fiftyville the next day, and his accomplice was on the phone.
SELECT * FROM interviews WHERE month = 7 AND day = 28;

--This query gets the people who were who were at the bakery on July 28th and selects them if they were
--also at the atm on Leggett Street on the same day, withdrawing money before 10 am.
SELECT DISTINCT name, people.license_plate FROM people JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate WHERE
people.license_plate IN
(SELECT license_plate FROM people WHERE id IN
(SELECT person_id FROM bank_accounts WHERE account_number IN
(SELECT account_number FROM atm_transactions WHERE month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND hour <= 10 AND
transaction_type = 'withdraw')));

--Finds the people that are on the earliest flight departing the next day from Fiftyville
SELECT name FROM people WHERE passport_number IN
(SELECT passport_number FROM passengers WHERE flight_id IN
(SELECT id FROM flights WHERE month = 7 AND day = 29 AND origin_airport_id = 8 ORDER BY hour ASC, minute ASC));

--List of people who made a phone call on July 28th and also were on the earliest flight departing from fiftyville on the 29th
SELECT name FROM people WHERE phone_number IN
(SELECT caller FROM phone_calls WHERE month = 7 AND day = 28) AND passport_number IN
(SELECT passport_number FROM passengers WHERE flight_id IN
(SELECT id FROM flights WHERE month = 7 AND day = 29 AND origin_airport_id = 8 ORDER BY hour ASC, minute ASC));


--Combines every single query above into one huge query; the only person who could've fit the description is Bruce.
SELECT name FROM people WHERE phone_number IN
(SELECT caller FROM phone_calls WHERE month = 7 AND day = 28 AND duration < 60) AND passport_number IN
(SELECT passport_number FROM passengers WHERE flight_id IN
(SELECT id FROM flights WHERE month = 7 AND day = 29 AND origin_airport_id = 8 AND hour = 8)) AND people.license_plate IN
(SELECT license_plate FROM people WHERE id IN
(SELECT person_id FROM bank_accounts WHERE account_number IN
(SELECT account_number FROM atm_transactions WHERE month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND
transaction_type = 'withdraw'))) AND people.license_plate IN
(SELECT bakery_security_logs.license_plate FROM bakery_security_logs WHERE day = 28 AND month = 7 AND hour = 10 AND
minute >= 15 AND minute <= 25);

--Used to find Bruce's phone number to identify his accomplice on the other end of the phone call
SELECT phone_number, passport_number from people WHERE name = 'Bruce';

--Finds the name of the accomplice by finding the only person Bruce called on July 28th for under a minute.
SELECT name FROM people WHERE phone_number =
(SELECT receiver FROM phone_calls WHERE caller = "(367) 555-5533" AND day = 28 AND month = 7 AND duration < 60);

--Finds the destination of the flight Bruce took on the 29th as the earliest flight out of fiftyville
SELECT city FROM airports WHERE id IN
(SELECT destination_airport_id FROM flights WHERE id IN
(SELECT flight_id FROM passengers WHERE passport_number = 5773159633));

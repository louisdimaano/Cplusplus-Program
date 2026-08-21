## Personal Expense Tracker — C++

## Overview

A C++ console application that helps users track and analyze their personal spending. Users can log expenses with an amount, date, category, and description, then view summaries including total spent and a breakdown by category. All data is saved to a local file so records persist between sessions.

## The Problem It Solves

Most expense tracking tools require internet access, accounts, or subscriptions. This application provides a simple, offline solution for organizing and analyzing day-to-day spending without any external dependencies.

## Features

Add expenses with amount, date, category, and description
View all recorded expenses
View spending summaries by category and total
Persistent data storage using a local file (expenses.dat)
Technical Highlights

The program uses two core classes with clear, separated responsibilities. The Expense class handles individual transactions and their serialization to and from a text format using a pipe delimiter. The ExpenseTracker class manages the full list of expenses, file I/O, and the user menu. This separation keeps main() minimal and makes the logic easy to follow and extend.

File read/write logic was designed with a defined line format using a | separator and helper functions ToLine() and FromLine() to ensure reliable serialization even for descriptions containing spaces.

## Technologies Used

C++ · Object-Oriented Programming · std::vector · std::map · File I/O · std::stringstream · Menu-driven console interface

## What I Learned

Designing classes with clear public and private responsibilities
Building reliable file serialization with custom delimiters
Using std::vector and std::map for record management
Writing maintainable code with single-responsibility functions and descriptive naming

## Future Improvements

Stronger input validation for dates, categories, and menu choices
CSV or JSON file format for better data portability
Optional encryption for financial data privacy
Search and filtering by date range or category

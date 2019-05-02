# EasyQtSql
EasyQtSql is a lightweight header-only C++11 library for quick and easy SQL querying with QtSql classes.

Features:
* Header only C++11 library
* Easy to use - just include header files
* Small size, simple code
* SQL execution with less C++ code compared to QtSql library
* Easier DB error handling
* Easier parameters binding & result reading
* RAII transactions
* Not an ORM library - you can write, optimize & execute SQL queries as you want
* MIT License

[EasyQtSql doxygen Documentation](https://kramolnic.github.io/EasyQtSql/)

## Getting Started
### Prerequisites
You need to install:
* C++11 compiler
* Qt5 library
* Qt Creator 4.7.2 - recommended (for automated test running)
* QtSql module with the required SQL database drivers compiled / installed

The library was compiled and tested with:
* MS Visual C++ Compiler 14.0 / Qt5.6 and Qt5.7
* gcc version 7.3.0 (Ubuntu 7.3.0-27ubuntu1~18.04) / Qt5.9

### Quick Start Guide

Please read [EasyQtSql doxygen Documentation](https://kramolnic.github.io/EasyQtSql/)

## Running the tests
Tests are implemented with [QtTest](http://doc.qt.io/archives/qt-5.7/qttest-index.html) module. To run the tests you can use Qt Creator. 

## Built With

* [Qt](https://www.qt.io/) - Qt | Cross-platform software development for embedded &amp; desktop
* [QtSql](http://doc.qt.io/archives/qt-5.7/qtsql-index.html) - Qt SQL is an essential module which provides support for SQL databases

## Authors

* **kramolnic** - *Initial work* - [https://github.com/kramolnic](https://github.com/kramolnic)

## License

This project is licensed under the [MIT License](https://github.com/kramolnic/EasyQtSql/blob/master/EasyQtSql/License.txt).


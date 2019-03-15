# ReadsProfiler


ReadsProfiler is a client-server app which provides to users access to an online library of free to download books.The users can search,download and rate certain books.The app is capable of recommending books to the users,based on the information it collects on them(searches,downloads,ratings of all users) using the k-means clustering technique.

## Getting Started

These instructions will get your copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

What things you need to install the software and how to install them.

gcc and g++
```
sudo apt-get install build-essential
```

QT version 4 or greater.
```
wget http://download.qt.io/official_releases/qt/5.7/5.7.0/qt-opensource-linux-x64-5.7.0.run
chmod +x qt-opensource-linux-x64-5.7.0.run
./qt-opensource-linux-x64-5.7.0.run
```
OpenGL
```
sudo apt-get install mesa-common-dev
```
Sqlite
```
sudo apt-get install sqlite3 libsqlite3-dev
```

### Compiling

#### Server

Change the path to the server directory

```
cd path-to-project-folder/server
```

And 

```
./chmod +x ./compile_server
./compile_server
```

#### Client
Change the path to the client directory

```
cd path-to-project-folder/client
```

Call qmake on the .pro file

```
qmake client.pro
```
Call make

```
make
```
## Executing the app's components

### Server 
Change the path to the server directory

```
cd path-to-project-folder/server
```
Execute server with one argument,representing the port address used for the connection with the client.
```
./server port
```
### Client
 Change the path to the client directory

```
cd path-to-project-folder/client
```

Execute with the following arguments:address and port  representing the address of the server and the port you want to use.  

```
./client address port
```
*Note:In order for the client-server app to work it is required to set the ports and the address properly.*

## Built With

* [SQLite](https://www.sqlite.org/index.html) -for the database
* [Qt](https://www.qt.io/) -  for the client's interface
* C\C++ 

## Authors

* **Silviu Andrei Mariuta** 




# Password Manager CLI in C++

A secure password manager command-line application written in C++.

## Prerequisites

- CMake 3.14 or higher
- A C++17 compatible compiler
- OpenSSL library
- nlohmann_json (via package manager or GitHub)

## Build

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Usage

Initialize a new database:

```bash
./pwdmgr init --db path/to/db.dat
```

Add an entry:

```bash
./pwdmgr add --db path/to/db.dat --name example.com --username user --password pass
```

List entries:

```bash
./pwdmgr list --db path/to/db.dat
```

Get an entry:

```bash
./pwdmgr get --db path/to/db.dat --name example.com
```

Remove an entry:

```bash
./pwdmgr remove --db path/to/db.dat --name example.com
```

Update an entry:

```bash
./pwdmgr update --db path/to/db.dat --name example.com --username newuser --password newpass
```

The CLI will prompt for a master password. 
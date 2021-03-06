Reign of Kings Server Browser
=============================

The Reign of Kings Server Browser lists all public game servers for the game [Reign of Kings](http://reignofkings.net/). It uses the same source as the game itself.

Currently only very basic functionality is provided for Windows, but the software should also build and run on other operating systems.

Screenshot
----------

![Reign Of Kings Server Browser on Windows 8.1](https://secure.mueller-martin.net/cloud/index.php/s/60d96ca2b311281fda9203c0ea512652/download)

Developer Information
---------------------

### Building

#### Dependencies

- Qt 5


#### Windows

For building the ROK Server Browser for Windows following components are required:

- MSYS
- MinGW-w64
- statically linked Qt 5

If all requirements are met, simply run:

```
qmake
```

and then

```
make
```

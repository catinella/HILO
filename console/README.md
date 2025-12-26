![Project's banner](../images/HILO_banner.png)

## 1.0 Files

| Files/Dirs | Description                                  |
|------------|----------------------------------------------|
| build.pro  | Rules to build the whole project files       |
| [conf.pri] | Building process configuration file          |
| inputDevs  | Virtual input devices (DUT point of view)    |
| mainApp    | The graphic console application              |
| outputDevs | Virtual output devices                       |
| sharedComps| Graphic components used by the other modules |

## 2.0 Description
This folder hosts the HILO's GUI. When you use this HILO in interactive mode, this graphical interface will allow you to set your
test environment with many virtual tools (buttons, led-stripes, switches, keypads, displays... and so on) and verify the DUT's
behavior controlling its input and output in all configurations you have planned.

### 2.1 QT
In order to provide a multiplatform and very flexible GUI I have developed this module using the QT framework. In fact, QT allows
me to modify and create new widgets in a very powerful way.

### 2.2 QT code building
To provide a multiplaform building chain I have use the official QT's tool **QMake6**. It requires a configuration file (file.pro)
to produce the platform dependent build-chain files (eg. Makefile for Unix/Linux). Then, you can compile the code with your usual
tool-chain, directly. For example, if you are using a POSIX systems, you can run the following command:

	qmake6 *.pro && make all

[!] Use this approach for all sub-modules' tests too.

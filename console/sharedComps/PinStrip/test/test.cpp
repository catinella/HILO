/*------------------------------------------------------------------------------------------------------------------------------
//                                                    _   _ ___ _     ___  
//                                                   | | | |_ _| |   / _ \
//                                                   | |_| || || |  | | | |
//                                                   |  _  || || |__| |_| |
//                                                   |_| |_|___|_____\___/ 
//                                                    Hardware in the loop
//                                              (https://github.com/catinella/HILO)
//
//
//
//
// Filename: test.cpp
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
//	This is a test for the PinStripe widget class. The test execution will show a PinStripe widget composed by many pins and a
//	set of buttons where everyone of them is connected to a single pin. In the test, every button reppresents a virtual tool
//	(eg. a button, a switch...) and the PinStripe would be the DUT's pins. So, you can connect every DUT's pin with any tool's
//	pin, then push the button and verify the DUT's pin value changing.
//	
//	In order to connect a couple of pins, click with the mouse's right button on the two pins and you will see a graphic line
//	from one to another.
//
//	To move the PinStripe around keep the left button pressed on the object and move the mouse to the new destination, then
//	release the button
//
//	Pins connection diagram:
//	========================
//
//		USER            PinWidget    ToolWidget   PinStripe     pinConnection   connectionOverlay
//		  |                 |             |           |               |                 |             
//		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
//		  |                 |             |           |               |                 |        //
//		  |    right click  |             |           |               |                 |        // Pin selection
//		  +================>|             |           |               |                 |        //
//		  |                 |             |           |               |   rightClicked()|        //
//		  |                 +---------------------------------------------------------->|        //
//		  |                 |             |           |               |addItem()        |        //
//		  |                 |             |           |               |<~~~~~~~~~~~~~~~~+        //
//		  |                 |             |           |               |                 |        //
//		  |                 |             |           |               |                 +---+    //
//		  |                 |             |           |               |     isComplete()|   |    //
//		  |                 |             |           |               |                 |<--+    //
//		  |                 |             |           |               |                 +---+    //
//		  |                 |             |           |               |   (link drawing)|   |    // Pins linking...
//		  |                 |             |           |               |                 |<--+    //
//		  |                 |             |           |               |                 |        //
//		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
//		  |                 |             |           |               |                 |        //
//
//	Pin's value changing diagram:
//	=============================
//
//		USER            PinWidget    ToolWidget   PinStripe     pinConnection   connectionOverlay
//		  |                 |             |           |               |                 |             
//		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
//		  |                 |             |           |               |                 |        //
//		  |                 |  btn pressed|           |               |                 |        // Virtual tool activation
//		  +==============================>|           |               |                 |        //    (Graphic button)
//		  |                 |setValue()   |           |               |                 |        //
//		  |                 |<~~~~~~~~~~~~+           |               |                 |        //
//		  |                 |             |           |               |   valueChanged()|        //
//		  |                 +---------------------------------------------------------->|        //
//		  |                 |setValue(*)  |           |               |                 |        // [!] propagation stopped
//		  |                 +<~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|        //     to avoid loop
//		  |                 |             |           |               |                 |        //
//		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		  |                 |             |           |               |                 |        //
//		  |                 |             |           |               |                 |        //  * : propagate = false
//
//	PinStrip drag-and-drop functionality diagram:
//	=============================================
//
//		USER            PinStripe          TestWidget     connectionOverlay
//		  |                    |                |                |     
//		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		  |                    |                |                |     
//		  |   left-butt(press) |                |                |     
//		  +===================>|                |                |
//		  |                    +---+            |                |     
//		  |                    |   |flag        |                |     
//		  |                    |   |setting     |                |     
//		  |                    +<--+            |                |     
//		  |                    |                |                |     
//		  |    left-butt(hold) |                |                |     
//		  +===================>|       dragging |                |
//		  |                    +--------------->|                |     
//		  |                    | moveTo()       |                |     
//		  |                    |<~~~~~~~~~~~~~~~+     paintNow() |     
//		  |                    |                +~~~~~~~~~~~~~~~>|     
//		  |                    |                |                |     
//		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		  |                    |                |                |     
//		  | left-butt(release) |                |                |     
//		  +===================>|                |                |
//		  |                    +---+            |                |     
//		  |                    |   |flag        |                |     
//		  |                    |   |resetting   |                |     
//		  |                    +<--+            |                |     
//		  |                    |                |                |     
//		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		  |                    |                |                |     
//
//
//		+--------+----------------------+
//		| Symbol | Description          |
//		+--------+----------------------+
//		|   ===  | user activity        |
//		|   ---  | QT signal            |
//		|   ~~~  | Fuction/method call  |
//		+--------+----------------------+
//
//
//
// License:  LGPL ver 3.0
//
// 		This script is a free software; you can redistribute it and/or modify it under the terms	of the GNU
// 		Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License,
// 		or (at your option) any later version. 
//
//		For further details please read the full LGPL text file [https://www.gnu.org/licenses/lgpl-3.0.txt].
// 		You should have received a copy of the GNU General Public License along with this file; 
// 		if not, write to the 
//
//			Free Software Foundation, Inc.,
//			59 Temple Place, Suite 330,
//			Boston, MA  02111-1307  USA
//
//                                                                                                               cols=128 tab=6
------------------------------------------------------------------------------------------------------------------------------*/

#include <QApplication>
#include "TestWidget.h"


int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	TestWidget w;
	w.show();

	return app.exec();
}


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
//		USER            PinStripe           test.cpp      connectionOverlay
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
#include <QWidget>
#include <QVBoxLayout>
#include "PinStrip.h"
#include "ConnectionOverlay.h"
#include "ToolWidget.h"

#define TEST_NUMOFPINS      8
#define TEST_DISPLAY_WIDTH  640
#define TEST_DISPLAY_HEIGHT 480

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	
	auto    canvas        = new QWidget();
	auto    canvasLayout  = new QVBoxLayout(canvas);
	auto    toolsLayout   = new QHBoxLayout();
	auto    dutStrip      = new PinStrip(PWDG_DUTSIDE, 8, canvas);
	auto    overlay       = new ConnectionOverlay(canvas);
	QVector<ToolWidget*> tools;

	{
		canvas->setAutoFillBackground(true);
		QPalette pal = canvas->palette();
		pal.setColor(QPalette::Window, QColor(102, 178, 255));  // grigio scuro
		canvas->setPalette(pal);
	}
	
	canvasLayout->setContentsMargins(10, 10, 10, 10);
	toolsLayout->setContentsMargins(0, 0, 0, 0);

	// 1) Tools creation and adjustment in the horizontal-layout (toolsLayout)
	for (uint8_t i = 0; i < 8; ++i) {
		ToolWidget *tool = new ToolWidget(i, canvas);
		toolsLayout->addWidget(tool);
		tools.append(tool);
	}

	// 2) Canvas layout
 	canvasLayout->addLayout(toolsLayout);
	toolsLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

	// 3) DUT's strip geometry
	dutStrip->adjustSize();
	dutStrip->move((TEST_DISPLAY_WIDTH - dutStrip->width())/2, (TEST_DISPLAY_HEIGHT/2));
	dutStrip->show();
	dutStrip->raise();


	// 4) Adding Overlay to canvas
	overlay->setGeometry(canvas->rect());
	overlay->raise();
	overlay->show();

	// 5) DUT's pin registration
	for (int i = 0; i < 8; ++i) {
		overlay->registerTerminal(QString("dut.%1").arg(i), dutStrip->getPin(i));
	}

	// 6) Tool's pin registration
	for (int i = 0; i < tools.size(); ++i) {
		overlay->registerTerminal(QString("tool.%1").arg(i), tools[i]->pinStrip()->getPin(0));
	}

	// 7) Canvas resizing
	canvas->setMinimumSize(TEST_DISPLAY_WIDTH, TEST_DISPLAY_HEIGHT);

	QObject::connect(dutStrip, &PinStrip::dragging, [dutStrip, canvas, overlay](const QPoint &delta) {
		QPoint p = dutStrip->position() + delta;

		const int maxX = canvas->width()  - dutStrip->width();
		const int maxY = canvas->height() - dutStrip->height();

		p.setX(std::clamp(p.x(), 0, maxX));
		p.setY(std::clamp(p.y(), 0, maxY));

		dutStrip->moveTo(p);

		overlay->paintNow();
	});
	
	canvas->show();

	return(app.exec());
}


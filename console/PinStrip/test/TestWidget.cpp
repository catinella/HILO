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
// Filename: TestWidget.cpp
//
// Author:   Silvano Catinella <catinella@yahoo.com>
//
// Description:
//		This class provides the object that implement the whole test.
//
//		+===================== mainLayout =====================+
//		|                                                      |
//		|  +================= canvasLayout =================+  |
//		|  |                                                |  |
//		|  |  +============== toolLayout ================+  |  |
//		|  |  |  +-------+  +-------+         +-------+  |  |  |
//		|  |  |  | Tool0 |  | Tool1 |  ////// | Tool7 |  |  |  |
//		|  |  |  +-------+  +-------+         +-------+  |  |  |
//		|  |  |     *           *                 *      |  |  |
//		|  |  +==========================================+  |  |
//		|  |                                                |  |
//		|  |  +-------------- DUT's stripe --------------+  |  |
//		|  |  |   *    *    *    *    *    *    *    *   |  |  |
//		|  |  |   0    1    2    3    4    5    6    7   |  |  |
//		|  |  +------------------------------------------+  |  |
//		|  |                                                |  |
//		|  +================================================+  |
//		|                                                      |
//		+======================================================+
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
#include "TestWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

TestWidget::TestWidget (QWidget *parent):QWidget (parent) {
	//
	// Description:
	//	It is the object constructor and implements all the test's logic
	//	See the the schema on the file top to get how the objects are arranged on the application's window
	//
	m_canvas = new QWidget (this);

	auto *canvasLayout = new QVBoxLayout (m_canvas);
	auto *toolsLayout = new QHBoxLayout ();

	canvasLayout->setContentsMargins (0, 0, 0, 0);
	toolsLayout->setContentsMargins (0, 0, 0, 0);

	// 1) DUT's strip creation
	m_dutStrip = new PinStrip (PWDG_DUTSIDE, 8, m_canvas);

	// 2) Tools creation and adjustment in the horizontal-layout (toolsLayout)
	for (uint8_t i = 0; i < 8; ++i) {
		ToolWidget *tool = new ToolWidget (i, m_canvas);
		toolsLayout->addWidget (tool);
		m_tools.append (tool);

		// IMPORTANT: It brings bit from tool to DUT's strip
		connect (tool->pinStrip (), &PinStrip::valuesChanged, this,[this, i] (uint8_t value) {
			   m_dutStrip->setValue (i, (((1u << i) & value) != 0u) ? 1 : 0);}
		);
	}

	// 3) Canvas layout
	canvasLayout->addLayout (toolsLayout);
	canvasLayout->addWidget (m_dutStrip);

	// 4) Adding Overlay to canvas
	m_overlay = new ConnectionOverlay (m_canvas);
	m_overlay->setGeometry (m_canvas->rect ());
	m_overlay->raise ();
	m_overlay->show ();

	// 5) DUT's pin registration
	for (int i = 0; i < 8; ++i) {
		m_overlay->registerTerminal(QString("dut.%1").arg(i), m_dutStrip->getPin(i));
	}

	// 6) Tool's pin registration
	for (int i = 0; i < m_tools.size (); ++i) {
		m_overlay->registerTerminal(QString("tool.%1").arg(i), m_tools[i]->pinStrip()->getPin(0));
	}

	// 7) Main layout: contiene solo il canvas
	auto *mainLayout = new QVBoxLayout (this);
	mainLayout->addWidget (m_canvas);
	setLayout (mainLayout);
}

void TestWidget::resizeEvent(QResizeEvent *e) {
	QWidget::resizeEvent(e);
	
	if (m_overlay && m_canvas) {
		m_overlay->setGeometry(m_canvas->rect());
		m_overlay->raise();
		m_overlay->update();
	}
}

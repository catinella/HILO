
#include "pinConnection.h"


//------------------------------------------------------------------------------------------------------------------------------
//                                               C O N S T R U C T O R S
//------------------------------------------------------------------------------------------------------------------------------
PinConnection::PinConnection (QString a, QString b, PinWidget *aw, PinWidget *bw): aId(a), bId(b), aW(aw), bW(bw) {
	if (a != b && aw != bw && aw != nullptr && bw != nullptr)
		normalize();
	else {
		// ERROR!
		aId = "!";
		bId = "!";
		aw = nullptr;
		bw = nullptr;
	}
}

PinConnection::PinConnection (QString a, PinWidget *aw): aId(a), aW(aw) {
	bId = "";
	bW = nullptr;
}

//------------------------------------------------------------------------------------------------------------------------------
//                                              P U B L I C   M E T H O D S
//------------------------------------------------------------------------------------------------------------------------------
bool PinConnection::addItem (const QString &x, PinWidget *xw) {
	//
	// Description:
	//	This method allows you to add a point to the link definition. But, it the definition is already completed, the
	//	method does not change the link and returns a fase value
	//
	bool out = false;
	if (aW == nullptr) {
		aId = x;
		aW = xw;
		out = true;
	} else if (bW == nullptr) {
		bId = x;
		bW = xw;
		out = true;
	} else {
		// ERROR!
	}

	// Links between pins on the same side, is not allowed
	if (aW != nullptr && bW != nullptr && aW->getSide() == bW->getSide()) {
		aW = nullptr;
		bW = nullptr;
	}
	return(out);
}


void PinConnection::getItems (QString &xID, QString &yID, PinWidget *&xWidget, PinWidget *&yWidget) const {
	xID = aId;
	yID = bId;
	xWidget = aW.data();
	yWidget = bW.data();

	return;
}


bool PinConnection::operator== (const PinConnection & o) const noexcept {
	return(aId == o.aId && bId == o.bId);
}
	

bool PinConnection::isValid () const noexcept {
	//
	// Description
	//	It return true only if the A and B points/widgets both have been defined
	//
	return(aW != nullptr && bW != nullptr);
}
	

bool PinConnection::involves (const QString & pinId) const {
	//
	// Description
	//	It returns a true value only if one of the two defined ponts has the same id of the argument defined one
	//
	return(aId == pinId || bId == pinId);
}


bool PinConnection::involves (const PinWidget *w) const {
	//
	// Description
	//	It returns a true value only if one of the two defined points refers the same widget of the argument defined one
	//
	return(aW == w || bW == w);
}


//------------------------------------------------------------------------------------------------------------------------------
//                                               P R I V A T E   M E T H O D S
//------------------------------------------------------------------------------------------------------------------------------
void PinConnection::normalize() {
	//
	// Description:
	//	This is the main constructor.
	//	In order to ignore the link-direction in the comparing operations, this method orders the couple keys. In this way A->B
	//	will be considered equal to B->A
	//
	if (bId < aId) {		
		qSwap(aId, bId);
		qSwap(aW, bW);
	}
}

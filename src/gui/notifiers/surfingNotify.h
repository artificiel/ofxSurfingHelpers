
// This is a modified version by mobiusSurfing
// bc some errors on static definitions...
// but also for adding some features and esthetic's.

//
//     _____    ___     
//    /    /   /  /     ofxNotify
//   /  __/ * /  /__    (c) ponies & light, 2013, 2015. All rights reserved.
//  /__/     /_____/    poniesandlight.co.uk
//
//  ofxNotify.h
//  Created by tim on 23/01/2013.
//  
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once

#include "ofMain.h"

class surfingNotify
{

private:

	struct MessagesColored 
	{
		std::string message;
		ofColor colorBg;
		ofColor colorText;
	};
 
public:

	////--------------------------------------------------------------
	//void addNotification(string msg) {
	//	messages[ofGetElapsedTimeMicros()] = msg;
	//}

	//--------------------------------------------------------------
	void addNotification(string msg) {
		MessagesColored m;
		m.message = msg;
		m.colorText = colorText;
		m.colorBg = colorBg;

		messages[ofGetElapsedTimeMicros()] = m;
	}

	//--------------------------------------------------------------
	void addNotification(string msg, ofColor _colorText, ofColor _colorBg) {
		MessagesColored m;
		m.message = msg;
		m.colorText = _colorText;
		m.colorBg = _colorBg;
		messages[ofGetElapsedTimeMicros()] = m;
	}

	std::map<unsigned long long, MessagesColored> messages;
	//std::map<unsigned long long, std::string> messages;

	int timeMsgMs; // defaults to 2000ms
	std::ostringstream message;
	bool bPrinted = false;

	ofTrueTypeFont font;
	float fontSize;
	float pad0 = 5;//to border
	float pad1;
	float pad2;
	float rounded;//box round

	// Default colors
	ofColor colorBg = ofColor(0);
	ofColor colorText = ofColor(255);

	//--------------------------------------------------------------
	void refreshPads()
	{
		pad1 = 3 * fontSize;
		pad2 = fontSize * 0.5f;
	}

public:

	enum AlignNote_
	{
		AlignNote_RIGHT = 0,
		AlignNote_CENTER,
		AlignNote_LEFT,

		AlignNote_AMOUNT
	};

private:

	AlignNote_ alignNote = AlignNote_RIGHT;

public:

	//--------------------------------------------------------------
	void setAlignment(AlignNote_ align) {
		alignNote = align;
	}

	//--------------------------------------------------------------
	void setColorText(ofColor c) {
		colorText = c;
	}
	//--------------------------------------------------------------
	void setColorBg(ofColor c) {
		colorBg = c;
	}

public:

	//--------------------------------------------------------------
	surfingNotify()
		: bPrinted(false) {

		timeMsgMs = 2000;

		// draw help font
		fontSize = 15;
		font.load(OF_TTF_MONO, fontSize);

		refreshPads();
		rounded = 10.f;
	};

	virtual ~surfingNotify();

	//--------------------------------------------------------------
	void setup(string pathFont, int sizeFont, float round = 10.f)
	{
		fontSize = sizeFont;
		font.load(pathFont, sizeFont);

		rounded = round;
		refreshPads();
	}

	// Draw the notifier 
	void draw(bool bShouldDraw = true);
	
	void setMessagesLifeTime(int messageLifeTimeInMilliseconds_);

	//--------------------------------------------------------------
	void drawTextBoxed(string text, int x, int y, int alpha = 255, ofColor cText = 255, ofColor cBg = 0)
	{
		ofPushStyle();

		if (!font.isLoaded())
		{
			ofDrawBitmapStringHighlight(text, x, y);
		}
		else
		{
			// bbox
			ofSetColor(cBg, alpha);
			ofFill();
			ofRectangle _r(font.getStringBoundingBox(text, x, y));
			_r.setWidth(_r.getWidth() + pad1);
			_r.setHeight(_r.getHeight() + pad1);
			_r.setX(_r.getPosition().x - pad1 / 2.f);
			_r.setY(_r.getPosition().y - pad1 / 2.f);

			if (rounded <= 0) ofDrawRectangle(_r);
			else ofDrawRectRounded(_r, rounded);

			// text
			//float alphaDelayed = MAX(0, alpha - 10);//fix
			//ofSetColor(cText, alphaDelayed);
			ofSetColor(cText, alpha);
			ofNoFill();

			font.drawString(text, x, y);
		}

		ofPopStyle();
	}

private:

	//--------------------------------------------------------------
	float getWidthBBtextBoxed(string text) {
		return (font.getStringBoundingBox(text, 0, 0)).getWidth();
	}

	//--------------------------------------------------------------
	float getHeightBBtextBoxed(string text) {
		return (font.getStringBoundingBox(text, 0, 0)).getHeight();
	}

};

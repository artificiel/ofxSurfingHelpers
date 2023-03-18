#pragma once
#include "ofMain.h"

namespace ofxSurfingHelpers {

	inline void ofxDrawString(string s, float x, float y, float z, ofTrueTypeFont* font, ofCamera* camera) {
		if (font == nullptr) return;
		if (camera == nullptr) return;

		bool bWorldTo2D = false;

		if (bWorldTo2D)
		{
			//TODO: wrong position..is no centered ?
			// could be related to ofScale(100.f); on StageManager::draw() ?
			//glm::vec3 posScreen = camera->worldToScreen(glm::vec3(x / 100.f, y / 100.f, z / 100.f));

			ofRectangle r = ofGetCurrentViewport();
			glm::vec3 posScreen = camera->worldToScreen(glm::vec3(x, y, z), r);

			float _x = posScreen.x;
			float _y = posScreen.y;

			////debug
			//float _x = ofGetWidth() / 2;
			//float _y = ofGetHeight() / 2;

			font->drawString(s, _x, _y);
		}
		else
		{
			//TODO: not works

#ifdef USE_TTF_TO_3D
			ofxDrawStringTrueType(s, x, y, z, &font);
#else
			font->drawString(s, x, y); // draw fonts in 3d, not to viewer/screen perpendicual oriented..
#endif
		}


		//TODO:
		// Must be out of the camera!
		//ofPushMatrix();
		//ofTranslate(posScreen);
		//font->drawString(s, 0, 0);
		//ofPopMatrix();
	};

	// From libs\openFrameworks\graphics\of3dGraphics.cpp
	// Allows passing a font and the camera to customize style instead of using bitmap font
	inline void ofxDrawGridPlane(float stepSize, size_t numberOfSteps, bool labels, ofTrueTypeFont* font, ofCamera* camera, ofColor c1 = ofColor{ 96, 96, 96, 128 }, ofColor c2 = ofColor{ 255, 255, 255, 255 })
	{
		auto renderer = ofGetCurrentRenderer();
		ofPushStyle();

		float scale = stepSize * numberOfSteps;
		float lineWidth = renderer->getStyle().lineWidth;

		// Draw all the lines

		ofSetColor(c1);
		for (int iDimension = 0; iDimension < 2; iDimension++)
		{
			for (size_t i = 0; i <= numberOfSteps; i++)
			{
				float yz = i * stepSize;

				if (i == numberOfSteps || i == 0)
					renderer->setLineWidth(2); // central axis or cap line
				else if (i % 2 == 0) {
					renderer->setLineWidth(1.5); // major
				}
				else {
					renderer->setLineWidth(1); // minor
				}

				if (iDimension == 0) {
					renderer->drawLine(0, yz, -scale, 0, yz, scale);
					if (yz != 0) renderer->drawLine(0, -yz, -scale, 0, -yz, scale);
				}
				else {
					renderer->drawLine(0, -scale, yz, 0, scale, yz);
					if (yz != 0) renderer->drawLine(0, -scale, -yz, 0, scale, -yz);
				}
			}
		}
		renderer->setLineWidth(lineWidth);

		// Draw all the labels

		if (labels) {
			//draw numbers on axes
			if (font == nullptr || camera == nullptr) // bitmap
			{
				ofColor prevColor = renderer->getStyle().color;
				ofDrawBitmapMode mode = renderer->getStyle().drawBitmapMode;

				renderer->setColor(c2);
				renderer->setBitmapTextMode(OF_BITMAPMODE_MODEL_BILLBOARD);
				renderer->drawString(ofToString(0), 0, 0, 0);

				for (float i = 1; i <= numberOfSteps; i++)
				{
					float yz = i * stepSize;

					renderer->drawString(ofToString(yz), 0, yz, 0);
					renderer->drawString(ofToString(-yz), 0, -yz, 0);
					renderer->drawString(ofToString(yz), 0, 0, yz);
					renderer->drawString(ofToString(-yz), 0, 0, -yz);
				}

				renderer->setColor(prevColor);
				renderer->setBitmapTextMode(mode);
			}
			else // custom 
			{
				//TODO:
				// skip labels. we will draw using the out-of-the-camera approach!
				ofPopStyle();
				return;


				ofSetColor(c2);

				ofxDrawString(ofToString(0), 0, 0, 0, font, camera);

				for (float i = 1; i <= numberOfSteps; i++)
				{
					float yz = i * stepSize;
					ofxDrawString(ofToString(yz), 0, yz, 0, font, camera);
					ofxDrawString(ofToString(-yz), 0, -yz, 0, font, camera);
					ofxDrawString(ofToString(yz), 0, 0, yz, font, camera);
					ofxDrawString(ofToString(-yz), 0, 0, -yz, font, camera);
				}

				ofPopStyle();
			}
		}

		else ofPopStyle();
	};

	// Draws a grid of the three planes with labels
	inline void ofxDrawGrid(float stepSize, size_t numberOfSteps, bool labels, bool x, bool y, bool z, ofTrueTypeFont* font, ofCamera* camera, ofColor c1 = ofColor{ 96, 96, 96, 128 }, ofColor c2 = ofColor{ 255, 255, 255, 255 })
	{
		auto renderer = ofGetCurrentRenderer();

		//ofPushStyle();
		ofColor prevColor = ofGetStyle().color;

		ofSetColor(c1);

		// lines with each plane labels

		if (font == nullptr || camera == nullptr) { // bitmap
			if (x) {
				ofDrawGridPlane(stepSize, numberOfSteps, labels);
			}
			if (y) {
				glm::mat4 m = glm::rotate(glm::mat4(1.0), glm::half_pi<float>(), glm::vec3(0, 0, -1));
				renderer->pushMatrix();
				renderer->multMatrix(m);
				ofDrawGridPlane(stepSize, numberOfSteps, labels);
				renderer->popMatrix();
			}
			if (z) {
				glm::mat4 m = glm::rotate(glm::mat4(1.0), glm::half_pi<float>(), glm::vec3(0, 1, 0));
				renderer->pushMatrix();
				renderer->multMatrix(m);
				ofDrawGridPlane(stepSize, numberOfSteps, labels);
				renderer->popMatrix();
			}
		}
		else { // custom ttf

			if (x) {
				ofxDrawGridPlane(stepSize, numberOfSteps, labels, font, camera, c1, c2);
			}
			if (y) {
				glm::mat4 m = glm::rotate(glm::mat4(1.0), glm::half_pi<float>(), glm::vec3(0, 0, -1));
				ofPushMatrix();
				ofMultMatrix(m);
				ofxDrawGridPlane(stepSize, numberOfSteps, labels, font, camera, c1, c2);
				ofPopMatrix();
			}
			if (z) {
				glm::mat4 m = glm::rotate(glm::mat4(1.0), glm::half_pi<float>(), glm::vec3(0, 1, 0));
				ofPushMatrix();
				ofMultMatrix(m);
				ofxDrawGridPlane(stepSize, numberOfSteps, labels, font, camera, c1, c2);
				ofPopMatrix();
			}
		}

		if (labels) {
			if (font == nullptr || camera == nullptr)
			{ // bitmap
				ofDrawBitmapMode mode = ofGetStyle().drawBitmapMode;
				float labelPos = stepSize * (numberOfSteps + 0.5);

				ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
				ofDrawBitmapString("x", labelPos, 0, 0);
				ofDrawBitmapString("y", 0, labelPos, 0);
				ofDrawBitmapString("z", 0, 0, labelPos);

				ofSetDrawBitmapMode(mode);
			}
			else
			{ // custom ttf
				//TODO:
				// skip labels. we will draw using the out-of-the-camera approach!

				//ofPopStyle();
				ofSetColor(prevColor);
				return;


				float labelPos = stepSize * (numberOfSteps + 0.5);

				ofxDrawString("x", labelPos, 0, 0, font, camera);
				ofxDrawString("y", 0, labelPos, 0, font, camera);
				ofxDrawString("z", 0, 0, labelPos, font, camera);
			}
		}

		//ofPopStyle();
		ofSetColor(prevColor);
	};

	//----


	//TODO: WIP
//#define USE_TTF_TO_3D // -> Enable to go fixing..

#ifdef USE_TTF_TO_3D
	// Getting from of\libs\openFrameworks\gl\ofGLRenderer.cpp
	//void ofGLRenderer::drawString(string textString, float x, float y, float z)
	//----------------------------------------------------------
	void ofxDrawStringTrueType(string textString, float x, float y, float z, ofTrueTypeFont* font) const
	{
		//ofGLRenderer* mutThis = const_cast<ofGLRenderer*>(this);

		// ?

		auto g = ofGetGLRenderer();
		ofGLRenderer* mutThis = g;

		//auto g = ofGetCurrentRenderer();
		//'const_cast': cannot convert from 'std::shared_ptr<ofBaseRenderer>' to 'ofGLRenderer *' 
		//ofGLRenderer* mutThis = const_cast<ofGLRenderer*>(g);

		//-

		float sx = 0;
		float sy = 0;


		///////////////////////////
		// APPLY TRANSFORM / VIEW
		///////////////////////////
		//

		bool hasModelView = false;
		bool hasProjection = false;
		bool hasViewport = false;

		ofRectangle rViewport;

		//bool vflipped = isVFlipped();
		bool vflipped = false;

		/*
		//switch (currentStyle.drawBitmapMode) {

		//case OF_BITMAPMODE_SIMPLE:

		//	sx += x;
		//	sy += y;
		//	break;

		//case OF_BITMAPMODE_SCREEN:

		//	hasViewport = true;
		//	mutThis->pushView();

		//	rViewport = matrixStack.getFullSurfaceViewport();
		//	mutThis->viewport(rViewport);

		//	mutThis->matrixMode(OF_MATRIX_PROJECTION);
		//	mutThis->loadIdentityMatrix();
		//	mutThis->matrixMode(OF_MATRIX_MODELVIEW);
		//	mutThis->loadIdentityMatrix();

		//	mutThis->translate(-1, 1, 0);
		//	mutThis->scale(2 / rViewport.width, -2 / rViewport.height, 1);
		//	mutThis->translate(x, rViewport.height - y, 0);
		//	vflipped = false;
		//	break;

		//case OF_BITMAPMODE_VIEWPORT:

		//	rViewport = getCurrentViewport();

		//	hasProjection = true;
		//	mutThis->matrixMode(OF_MATRIX_PROJECTION);
		//	mutThis->pushMatrix();
		//	mutThis->loadIdentityMatrix();

		//	hasModelView = true;
		//	mutThis->matrixMode(OF_MATRIX_MODELVIEW);
		//	mutThis->pushMatrix();
		//	mutThis->loadIdentityMatrix();

		//	mutThis->translate(-1, 1, 0);
		//	mutThis->scale(2 / rViewport.width, -2 / rViewport.height, 1);
		//	mutThis->translate(x, rViewport.height - y, 0);
		//	vflipped = false;
		//	break;

		//case OF_BITMAPMODE_MODEL:

		//	hasModelView = true;
		//	mutThis->matrixMode(OF_MATRIX_MODELVIEW);
		//	mutThis->pushMatrix();

		//	mutThis->translate(x, y, z);
		//	break;
		*/

		//case OF_BITMAPMODE_MODEL_BILLBOARD:
		{
			//our aim here is to draw to screen
			//at the viewport position related
			//to the world position x,y,z

			// tig: we want to get the signed normalised screen coordinates (-1,+1) of our point (x,y,z)
			// that's projection * modelview * point in GLSL multiplication order
			// then doing the good old (v + 1.0) / 2. to get unsigned normalized screen (0,1) coordinates.
			// we then multiply x by width and y by height to get window coordinates.

			// previous implementations used gluProject, which made it incompatible with GLES (and the future)
			// https://developer.apple.com/library/mac/#documentation/Darwin/Reference/ManPages/man3/gluProject.3.html
			//
			// this could probably be backported to the GL2 Renderer =)

			//rViewport = getCurrentViewport();
			rViewport = ofGetCurrentViewport();

			glm::mat4 modelview = glm::mat4(1.0), projection = glm::mat4(1.0);
			glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(modelview));
			glGetFloatv(GL_PROJECTION_MATRIX, glm::value_ptr(projection));


			//glm::mat4 mat = matrixStack.getOrientationMatrixInverse() * projection * modelview;

			//glm::mat4 mat = matrixStack.getOrientationMatrixInverse() * projection * modelview;
			//ofMatrixStack matrixStack;??

			//TODO: not inverted//
			glm::mat4 _m = ofGetCurrentOrientationMatrix();

			glm::mat4 mat = _m * projection * modelview;;





			glm::vec4 dScreen4 = mat * glm::vec4(x, y, z, 1.0);
			glm::vec3 dScreen = glm::vec3(dScreen4) / dScreen4.w;
			dScreen += glm::vec3(1.0);
			dScreen *= 0.5;

			dScreen.x *= rViewport.width;
			dScreen.x += rViewport.x;

			dScreen.y *= rViewport.height;
			dScreen.y += rViewport.y;

			if (dScreen.z >= 1) {
				return;
			}


			hasProjection = true;
			mutThis->matrixMode(OF_MATRIX_PROJECTION);
			mutThis->pushMatrix();
			mutThis->loadIdentityMatrix();

			hasModelView = true;
			mutThis->matrixMode(OF_MATRIX_MODELVIEW);
			mutThis->pushMatrix();
			mutThis->loadIdentityMatrix();

			mutThis->translate(-1, -1, 0);

			mutThis->scale(2 / rViewport.width, 2 / rViewport.height, 1);

			mutThis->translate(dScreen.x, dScreen.y, 0);
		}
		//}
		//break;

		//default:
		//	break;
		//}

		// remember the current blend mode so that we can restore it at the end of this method.
		GLint blend_src, blend_dst;
		glGetIntegerv(GL_BLEND_SRC, &blend_src);
		glGetIntegerv(GL_BLEND_DST, &blend_dst);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#ifndef TARGET_OPENGLES
		// this temporarily enables alpha testing,
		// which discards pixels unless their alpha is 1.0f
		glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0);
#endif



		//// A. Draw mesh text 
		//ofMesh charMesh = bitmapFont.getMesh(textString, sx, sy, currentStyle.drawBitmapMode, vflipped);
		//mutThis->bind(bitmapFont.getTexture(), 0);
		//draw(charMesh, OF_MESH_FILL, false, true, false);
		//mutThis->unbind(bitmapFont.getTexture(), 0);


		// B. Draw mesh true type text 
		ofMesh charMesh = font->getStringMesh(textString, sx, sy, vflipped);
		g->draw(charMesh, OF_MESH_FILL);




#ifndef TARGET_OPENGLES
		glPopAttrib();
#endif
		// restore blendmode
		glBlendFunc(blend_src, blend_dst);

		if (hasModelView)
			mutThis->popMatrix();

		if (hasProjection)
		{
			mutThis->matrixMode(OF_MATRIX_PROJECTION);
			mutThis->popMatrix();
			mutThis->matrixMode(OF_MATRIX_MODELVIEW);
		}

		if (hasViewport)
			mutThis->popView();

	}
#endif


	//TODO:
	// Another approach is to draw the labels out of the camera,
	// using worldTo2D

	// Allows passing a font and the camera to customize style instead of using bitmap font
	inline void ofxDrawGridPlaneLabels(float stepSize, size_t numberOfSteps, ofTrueTypeFont* font, ofCamera* camera, ofColor c = ofColor{ 255, 255, 255, 255 }, float scale = -1.0f)
	{
		if (font == nullptr) return;
		if (camera == nullptr) return;

		//ofPushStyle();
		ofColor prevColor = ofGetStyle().color;

		ofSetColor(c);

		string s;
		glm::vec2 p;
		glm::vec2 bb;
		ofRectangle r = ofGetCurrentViewport();

		// Draw all the labels
		{
			// Draw numbers on axes
			{
				// point zero 
				s = ofToString(0);
				p = camera->worldToScreen(glm::vec3(0, 0, 0), r);
				bb = font->getStringBoundingBox(s, 0, 0).getTopRight();
				p -= glm::vec2(bb.x / 2, -bb.x / 2);
				font->drawString(s, p.x, p.y);

				for (float i = 1; i <= numberOfSteps; i++)
				{
					float yz = i * stepSize;

					float yz_ = yz;//for labels without scale fix!

					//renderer->drawString(ofToString(yz), 0, yz, 0);
					//renderer->drawString(ofToString(-yz), 0, -yz, 0);
					//renderer->drawString(ofToString(yz), 0, 0, yz);
					//renderer->drawString(ofToString(-yz), 0, 0, -yz);

					//TODO: workaround fix
					// how to fix without using billboard mode like as ofBitmapFont?
					if (scale != -1.0f) yz *= scale;

					// floor x and floor z
					
					// flipped
					//s = ofToString(yz);
					//p = camera->worldToScreen(glm::vec3(0, yz, 0), r);
					//bb = font->getStringBoundingBox(s, 0, 0).getTopRight();
					//p -= glm::vec2(bb.x / 2, -bb.x / 2);
					//font->drawString(s, p.x, p.y);
					//s = ofToString(-yz);
					//p = camera->worldToScreen(glm::vec3(0, -yz, 0), r);
					//bb = font->getStringBoundingBox(s, 0, 0).getTopRight();
					//p -= glm::vec2(bb.x / 2, -bb.x / 2);
					//font->drawString(s, p.x, p.y);

					//fix
					s = ofToString(yz_);
					p = camera->worldToScreen(glm::vec3(yz, 0, 0), r);
					bb = font->getStringBoundingBox(s, 0, 0).getTopRight();
					p -= glm::vec2(bb.x / 2, -bb.x / 2);
					font->drawString(s, p.x, p.y);
					s = ofToString(-yz_);
					p = camera->worldToScreen(glm::vec3(-yz, 0, 0), r);
					bb = font->getStringBoundingBox(s, 0, 0).getTopRight();
					p -= glm::vec2(bb.x / 2, -bb.x / 2);
					font->drawString(s, p.x, p.y);


					s = ofToString(yz_);
					p = camera->worldToScreen(glm::vec3(0, 0, yz), r);
					bb = font->getStringBoundingBox(s, 0, 0).getTopRight();
					p -= glm::vec2(bb.x / 2, -bb.x / 2);
					font->drawString(s, p.x, p.y);

					s = ofToString(-yz_);
					p = camera->worldToScreen(glm::vec3(0, 0, -yz), r);
					bb = font->getStringBoundingBox(s, 0, 0).getTopRight();
					p -= glm::vec2(bb.x / 2, -bb.x / 2);
					font->drawString(s, p.x, p.y);
				}
			}

			//--

			// Draw x, y, z labels
			{
				float labelPos = stepSize * (numberOfSteps + 0.5);
				float labelPos_ = labelPos;

				//TODO: workaround fix
				// how to fix without using billboard mode like as ofBitmapFont?
				if (scale != -1.0f) labelPos *= scale;

				s = "x";
				p = camera->worldToScreen(glm::vec3(labelPos, 0, 0), r);
				bb = font->getStringBoundingBox(s, 0, 0).getTopRight();
				p -= glm::vec2(bb.x / 2, -bb.x / 2);
				font->drawString(s, p.x, p.y);

				s = "y";
				p = camera->worldToScreen(glm::vec3(0, labelPos, 0), r);
				bb = font->getStringBoundingBox(s, 0, 0).getTopRight();
				p -= glm::vec2(bb.x / 2, -bb.x / 2);
				font->drawString(s, p.x, p.y);

				s = "z";
				p = camera->worldToScreen(glm::vec3(0, 0, labelPos), r);
				bb = font->getStringBoundingBox(s, 0, 0).getTopRight();
				p -= glm::vec2(bb.x / 2, -bb.x / 2);
				font->drawString(s, p.x, p.y);
			}
		}

		//ofPopStyle();
		ofSetColor(prevColor);
	};

	//--

	// 3D Scene Grids and Bg Helpers

	//--------------------------------------------------------------
	inline void SurfDrawBgGradient(ofColor c1 = ofColor{ 40, 40, 40 }, ofColor c2 = ofColor{ 0, 0, 0 })
	{
		ofBackgroundGradient(c1, c2, OF_GRADIENT_CIRCULAR);
	};

	//--------------------------------------------------------------
	inline void SurfDrawFloor(float size, float offsetHeight = 0, ofColor c1 = ofColor{ 96, 96, 96, 128 }, ofColor c2 = ofColor{ 64, 64, 64, 128 })
	{
		bool bFlipz = true;//make it floor

		ofPushMatrix();
		ofPushStyle();

		if (offsetHeight != 0) ofTranslate(0, offsetHeight, 0);

		if (bFlipz) ofRotate(90, 0, 0, 1);

		ofSetColor(c1);
		ofDrawGridPlane(size, 1, false);

		ofSetColor(c2);
		ofDrawGridPlane(size / 2.f, 2, false);

		ofPopStyle();
		ofPopMatrix();
	};

};
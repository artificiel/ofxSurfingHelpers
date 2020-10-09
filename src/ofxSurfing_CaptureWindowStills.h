#pragma once

#include "ofMain.h"

///----
///
#define INCLUDE_RECORDER
///
///----

#include "ofxTextureRecorder.h"
#include "ofxSurfingHelpers.h"

class CaptureWindow : public ofBaseApp
{

public:
	CaptureWindow() {
		cap_w = 1920;
		cap_h = 1080;
	};
	~CaptureWindow() {};

private:
	ofxTextureRecorder recorder;

	ofFbo cap_Fbo;
	ofFbo::Settings cap_Fbo_Settings;
	int cap_w, cap_h;

	string _pathFolderStills;
	string _pathFolderSnapshots;

	bool bRecPrepared = false;
	bool bRecording;
	bool bShowInfo = false;

public:
	//--------------------------------------------------------------
	void setVisibleInfo(bool b) {
		bShowInfo = b;
	}
	//--------------------------------------------------------------
	void setToggleVisibleInfo() {
		bShowInfo = !bShowInfo;
	}

private:
	//--------------------------------------------------------------
	float getRecordedDuration() {
		return (ofGetElapsedTimeMillis() - timeStart) / 1000.f;
	}

private:
	uint32_t timeStart;
	std::string textInfo;

public:
	//--------------------------------------------------------------
	void setup(std::string path = "captures/") {///call with the path folder if you want to customize
		_pathFolderStills = path + "Stills/";
		_pathFolderSnapshots = path + "Snapshots/";

		ofxSurfingHelpers::CheckFolder(_pathFolderStills);
		ofxSurfingHelpers::CheckFolder(_pathFolderSnapshots);

		cap_Fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
		ofxTextureRecorder::Settings settings(cap_Fbo.getTexture());
		settings.imageFormat = OF_IMAGE_FORMAT_JPEG;
		settings.numThreads = 12;
		settings.maxMemoryUsage = 9000000000;

		recorder.setPath(_pathFolderStills);
		recorder.setup(settings);

		cap_w = ofGetWidth();
		cap_h = ofGetHeight();

		buildAllocateFbo();

		buildInfo();
	}

public:
	//--------------------------------------------------------------
	void buildAllocateFbo() {//cap_w and cap_h must be updated

		cap_Fbo_Settings.internalformat = GL_RGB;
		cap_Fbo_Settings.width = cap_w;
		cap_Fbo_Settings.height = cap_h;
		cap_Fbo.allocate(cap_Fbo_Settings);
	}

public:
	//--------------------------------------------------------------
	void begin() {///call before draw the scene to record

		cap_Fbo.begin();
		ofClear(0, 255);
	}

	//--------------------------------------------------------------
	void end() {///call after draw the scene to record

		cap_Fbo.end();

		//-

		if (bRecPrepared)
		{
			if (bRecording && ofGetFrameNum() > 0)
			{
				recorder.save(cap_Fbo.getTexture());
			}
		}
	}

	//--------------------------------------------------------------
	void draw() {
		cap_Fbo.draw(0, 0);//drawing is required outside fbo
	}

	//--------------------------------------------------------------
	void drawInfo() {///draw the gui info if desired
		if (bShowInfo) {

			drawHelp();

			if (bRecPrepared || bRecording)
			{
				int y = ofGetHeight() - 200;
				int x = 20;

				//cap info
				string str;
				str = "SIZE " + ofToString(cap_w) + "x" + ofToString(cap_h);

				//draw red circle and info when recording
				ofPushStyle();

				//red rec circle
				if (bRecording)
				{
					ofFill();
					ofSetColor(ofColor::red);
					ofDrawCircle(ofPoint(x + 8, y), 8);
					ofNoFill();
					ofSetLineWidth(2.f);
					ofSetColor(ofColor::black);
					ofDrawCircle(ofPoint(x + 8, y), 8);
					y += 28;
				}
				else if (bRecPrepared)
				{
					if (ofGetFrameNum() % 60 < 20) {
						ofFill();
						ofSetColor(ofColor::red);
						ofDrawCircle(ofPoint(x + 8, y), 8);
					}
					ofNoFill();
					ofSetLineWidth(2.f);
					ofSetColor(ofColor::black);
					ofDrawCircle(ofPoint(x + 8, y), 8);
					y += 28;
				}

				//fps
				ofDrawBitmapStringHighlight("FPS " + ofToString(ofGetFrameRate(), 0), x, y);
				y += 20;

				//cap info
				ofDrawBitmapStringHighlight(str, x, y);
				y += 20;

				//refresh window size
				ofDrawBitmapStringHighlight("KEY F8: REFRESH WINDOW SIZE", x, y);
				y += 20;

				if (bRecording)
				{
					ofDrawBitmapStringHighlight("RECORD DURATION: " + ofToString(getRecordedDuration(), 1), x, y);
					y += 20;
					ofDrawBitmapStringHighlight("KEY U: STOP", x, y);
					y += 20;
				}
				else if (bRecPrepared)
				{
					ofDrawBitmapStringHighlight("RECORD MOUNTED. READY...", x, y);
					y += 20;
					ofDrawBitmapStringHighlight("KEY U: START  u: UNMOUNT", x, y);
					y += 20;
				}

				ofPopStyle();
			}
		}
	}

	//--------------------------------------------------------------
	void drawHelp() {
		// help info
		ofDrawBitmapStringHighlight(textInfo, 20, 50);

		if (bRecording)
		{
			if (ofGetFrameNum() % 60 == 0) {
				ofLogWarning(__FUNCTION__) << ofGetFrameRate();
				ofLogWarning(__FUNCTION__) << "texture copy: " << recorder.getAvgTimeTextureCopy();
				ofLogWarning(__FUNCTION__) << "gpu download: " << recorder.getAvgTimeGpuDownload();
				ofLogWarning(__FUNCTION__) << "image encoding: " << recorder.getAvgTimeEncode();
				ofLogWarning(__FUNCTION__) << "file save: " << recorder.getAvgTimeSave();
			}
		}
	}

	//--------------------------------------------------------------
	void keyPressed(ofKeyEventArgs &eventArgs) {///to received short keys control commands
		const int key = eventArgs.key;
		const int keycode = eventArgs.keycode;
		const int scancode = eventArgs.scancode;
		const uint32_t codepoint = eventArgs.codepoint;

		//-

		switch (key)
		{
			//toggle show info
		case 'h':
			setToggleVisibleInfo();
			break;

			//mount prepare record
		case 'u':
			bRecPrepared = !bRecPrepared;
			ofLogWarning(__FUNCTION__) << "Mount: " << (bRecPrepared ? "ON" : "OFF");
			break;

			//start recording
		case 'U':
		{
			if (bRecording)//do stop
			{
				ofLogWarning(__FUNCTION__) << "Stop Recording";

				bRecPrepared = false;
				bRecording = false;
			}
			else//do start
			{
				bRecording = true;
				timeStart = ofGetElapsedTimeMillis();
				ofLogWarning(__FUNCTION__) << "Start Recording into: " << _pathFolderStills;
			}
		}
		break;

		//take screenshot
		case OF_KEY_F11:
		{
			string _fileName = "snapshot_" + ofGetTimestampString() + ".png";
			string _pathFilename = ofToDataPath(_pathFolderSnapshots + _fileName, true);//bin/data

			ofImage img;
			img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
			bool b = img.save(_pathFilename);

			if (b) cout << __FUNCTION__ << " Saved screenshot successfully: " << _pathFilename << endl;
			else cout << __FUNCTION__ << " Error saving screenshot:" << _pathFilename << endl;
		}
		break;

		//set instagram size
		case 'i':
		{
			int w, h;
			w = 864;
			h = 1080 + 19;
			ofSetWindowShape(w, h);
			windowResized(w, h);

			cap_w = w;
			cap_h = h;
			buildAllocateFbo();
		}
		break;

		// refresh window size to update fbo settings
		case OF_KEY_F8:
			windowResized(ofGetWidth(), ofGetHeight());
			break;
		}
	}

	//--------------------------------------------------------------
	void windowResized(int w, int h) {///must be called to resize the fbo and video resolution
		cap_w = w;
		cap_h = h;
		buildAllocateFbo();
	}

private:
	//--------------------------------------------------------------
	void buildInfo() {///must be called after bitrate, framerate and size w/h are setted

		//build help info
		textInfo = "";
		textInfo += "HELP KEYS"; textInfo += "\n";
		textInfo += "h  : Show Help info"; textInfo += "\n";
		textInfo += "u  : Mount recorder"; textInfo += "\n";
		textInfo += "i  : Set optimal Instagram size"; textInfo += "\n";
		textInfo += "F8 : Refresh window size"; textInfo += "\n";
		textInfo += "U  : Start recording"; textInfo += "\n";
		textInfo += "F11: Capture screenshot"; textInfo += "\n";
	}
};
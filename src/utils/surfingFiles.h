#pragma once

#include "ofMain.h"

namespace ofxSurfingHelpers {

	// Taken from ofxKu
	//--------------------------------------------------------------------------------
	inline bool ofxKuFileExists(string fileName)
	{
		fileName = ofToDataPath(fileName);
		std::ifstream inp;
		inp.open(fileName.c_str(), std::ifstream::in);
		inp.close();
		return !inp.fail();
	}

	//-

	// Taken from ofxMyUtil 
	//--------------------------------------------------------------
	inline bool TextToFile(const string& Path, const stringstream& Args, bool Append = false)
	{
		//ofFile f(path, ofFile::ReadWrite);
		//if (!f.exists()) f.create();

		std::filebuf fb;
		if (Append)
			fb.open(Path, std::ios::app);
		else
			fb.open(Path, std::ios::out);

		if (!fb.is_open()) return false;

		std::ostream os(&fb);
		stringstream ss(Args.str());

		if (ss.fail()) return false;

		string temp;
		while (getline(ss, temp))
		{
			if (temp != "")
			{
				os << temp << endl;
			}
		}

		fb.close();
		return true;
	}

	//--------------------------------------------------------------
	inline bool TextToFile(const string& Path, const char *Args, bool Append = false)
	{
		stringstream ss; ss << Args;
		return TextToFile(Path, ss, Append);
	}

	//--------------------------------------------------------------
	inline bool TextToFile(const std::string& Path, std::string Args, bool Append = false)
	{
		stringstream ss; ss << Args;
		return TextToFile(Path, ss, Append);
	}

};// ofxSurfingHelpers
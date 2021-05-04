#pragma once

#include "ofMain.h"


#include <random>
#include <iostream>


namespace ofxSurfingHelpers {

	//------------------------------------------------------------------------------
	//animator functions taken from 
	//https://github.com/tcoppex/ofxFontSampler/blob/main/example/src/ofApp.cpp

	namespace {

		/* Return a linear value in range [0,1] every delay (in seconds). */
		float Tick(float delay) {
			return fmodf(ofGetElapsedTimeMillis() / 1000.0f, delay) / delay;
		}

		/* Return a linear value in range [0,1] every delay (in seconds),
		 with value equal 0 at boundaries, and 1 at delay / 2. */
		float Bounce(float delay) {
			return 0.5f*(1.0 + glm::sin(Tick(delay) * glm::two_pi<float>()));
		}

		/* Noise function used by the gradient scaling. */
		float Noise(const ofPoint &vertex) {
			return /*24.0f **/ ofNoise(0.005f*vertex + 0.5f*ofGetElapsedTimeMillis()*0.0002f);
		}

		static constexpr int32_t kCharsetSize = 26;

	}  // namespace

    //--

        inline float NextGaussian(const float center, const float standard_dev)
        {
            std::random_device rd;
            std::mt19937 mt(rd());
            std::normal_distribution<float> distribution(center, standard_dev);
            return distribution(mt);
        }
        inline float NextReal(const float lower, const float upper)
        {
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_real_distribution<float> distribution(lower, upper);
            return distribution(mt);
        }


	//

	//get a blink faded to use as alpha on gui button when "active-listening-mode" enabled
	//ie: blink when a new preset is editing
	//--------------------------------------------------------------
	inline float getFadeBlink(float min = 0.20, float max = 0.80, float freq = 0.15) {

		float a = ofMap(glm::sin(freq * ofGetFrameNum()), -1, 1, min, max);

		return a;
	}

	//-

	// simple smooth
	//--------------------------------------------------------------
	template <typename T>
	void ofxKuValueSmooth(T &value, T target, float smooth) {
		value += (target - value) * (1 - smooth);
	}

	//--------------------------------------------------------------
	template <typename T>
	void ofxKuValueSmoothDirected(T &value, T target, float smooth0, float smooth1) {
		float smooth = (target < value) ? smooth0 : smooth1;
		ofxKuValueSmooth(value, target, smooth);
	}
};


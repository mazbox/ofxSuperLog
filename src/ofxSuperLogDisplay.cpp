/**
 *  ofxSuperLogDisplayLoggerChannel.cpp
 *
 *  Created by Marek Bereza on 02/09/2013.
 */

#include "ofxSuperLogDisplay.h"

ofxSuperLogDisplay::ofxSuperLogDisplay() {
	enabled = false;
	minimized = true;
	MAX_NUM_LOG_LINES = DEFAULT_NUM_LOG_LINES;
	width = ofGetWidth()*2.f/5.f;;
	draggingWidth = false;
}

ofxSuperLogDisplay::~ofxSuperLogDisplay() {
}

void ofxSuperLogDisplay::setMaxNumLogLines(int maxNumLogLines) {
	MAX_NUM_LOG_LINES = maxNumLogLines;
}

void draw(ofEventArgs &e);

void mousePressed(ofMouseEventArgs &e);
void mouseMoved(ofMouseEventArgs &e);
void mouseDragged(ofMouseEventArgs &e);
void mouseReleased(ofMouseEventArgs &e);


void ofxSuperLogDisplay::setEnabled(bool enabled) {
	if(enabled==this->enabled) return;
	this->enabled = enabled;
	if(enabled) {
		ofAddListener(ofEvents().mousePressed, this, &ofxSuperLogDisplay::mousePressed);
		ofAddListener(ofEvents().mouseMoved, this, &ofxSuperLogDisplay::mouseMoved);
		ofAddListener(ofEvents().mouseDragged, this, &ofxSuperLogDisplay::mouseDragged);
		ofAddListener(ofEvents().mouseReleased, this, &ofxSuperLogDisplay::mouseReleased);
		ofAddListener(ofEvents().draw, this, &ofxSuperLogDisplay::draw);
	} else {
		ofRemoveListener(ofEvents().mousePressed, this, &ofxSuperLogDisplay::mousePressed);
		ofRemoveListener(ofEvents().mouseMoved, this, &ofxSuperLogDisplay::mouseMoved);
		ofRemoveListener(ofEvents().mouseDragged, this, &ofxSuperLogDisplay::mouseDragged);
		ofRemoveListener(ofEvents().mouseReleased, this, &ofxSuperLogDisplay::mouseReleased);
		ofRemoveListener(ofEvents().draw, this, &ofxSuperLogDisplay::draw);
	}
}

bool ofxSuperLogDisplay::isEnabled() {
	return enabled;
}

void ofxSuperLogDisplay::log(ofLogLevel level, const string & module, const string & message) {
	
	if(message.find('\n')==-1) {
		logLines.push_back(module + ": " + ofGetLogLevelName(level) + ": " + message);
	} else {
		vector<string> lines = ofSplitString(message,"\n");
		for(int i = 0; i < lines.size(); i++) {
			if(i==0) {
				logLines.push_back(module + ": " + ofGetLogLevelName(level) + ": " + lines[0]);
			} else {
				logLines.push_back("\t" + lines[i]);
			}
		}
		
	}
	while(logLines.size()>MAX_NUM_LOG_LINES) {
		logLines.pop_front();
	}

}


void ofxSuperLogDisplay::log(ofLogLevel logLevel, const string & module, const char* format, ...) {
	va_list args;
	va_start(args, format);
	log(logLevel, module, format, args);
	va_end(args);
}

void ofxSuperLogDisplay::log(ofLogLevel logLevel, const string & module, const char* format, va_list args) {
	log(logLevel, module, ofVAArgsToString(format,args));
}




void ofxSuperLogDisplay::draw(ofEventArgs &e) {

	if(minimized) {
		minimizedRect.set(ofGetWidth() -150, ofGetHeight() - 20, 150, 20);
		
		ofPushStyle();
		ofEnableAlphaBlending();
		glColor4f(0, 0, 0, 0.4);
		ofRect(minimizedRect);
		ofSetColor(255);
		ofDrawBitmapString("+ [ Log ] ", minimizedRect.x + 10, minimizedRect.getBottom() - 4);
		ofPopStyle();
	} else {
		float x = ofGetWidth() - width;

		ofPushStyle();
		ofEnableAlphaBlending();
		glColor4f(0, 0, 0, 0.3);
		ofRect(x, 0, width, ofGetHeight());

		ofSetColor(255);
		int pos = 0;
		for(int i = logLines.size() - 1; i >=0; i--) {
			float yy = ofGetHeight() - pos*20 - 5;
			if(yy<0) break;
			ofDrawBitmapString(logLines[i], x + 20, yy);
			pos++;
		}
		
		glColor4f(0, 0, 0.5, 0.3);
		ofRect(ofGetWidth() - width, 0, 20, ofGetHeight());
		ofSetColor(255);
		float yy = ofGetHeight()/2;
		ofLine(x+8, yy - 10, x+8, yy+10);
		ofLine(x+12, yy - 10, x+12, yy+10);
		ofDrawBitmapString("x", ofGetWidth() - width + 5, ofGetHeight() - 5);
		ofPopStyle();
	}
}

void ofxSuperLogDisplay::mousePressed(ofMouseEventArgs &e) {
	if(!minimized && ABS(e.x - (ofGetWidth() - width))<20) {
		draggingWidth = true;
		mouseDragged(e);
	}
}
void ofxSuperLogDisplay::mouseMoved(ofMouseEventArgs &e) {
}
void ofxSuperLogDisplay::mouseDragged(ofMouseEventArgs &e) {
	if(draggingWidth) {
		width = 10 + ofGetWidth() - e.x;
		width = MAX(10, width);
		width = MIN(ofGetWidth() - 10, width);
	}
}
	   
	   
void ofxSuperLogDisplay::mouseReleased(ofMouseEventArgs &e) {
	if(minimized) {
		if(minimizedRect.inside(e.x, e.y)) {
			minimized = false;
		}
	} else {
		if(e.y>ofGetHeight() - 20) {
			if(ABS(e.x - (ofGetWidth() - width))<20) {
				minimized = true;
			}
		}
	}
	draggingWidth = false;
}

void ofxSuperLogDisplay::setMinimized(bool minimized) {
	this->minimized  = minimized;
}

bool ofxSuperLogDisplay::isMinimized() {
	return minimized;
}

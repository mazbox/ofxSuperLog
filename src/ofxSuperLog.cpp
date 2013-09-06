/**
 *  ofxSuperLog.cpp
 *
 *  Created by Marek Bereza on 02/09/2013.
 */

#include "ofxSuperLog.h"
ofPtr<ofxSuperLog> ofxSuperLog::logger;
ofxSuperLog *ofxSuperLog::logPtr = NULL;


ofPtr<ofxSuperLog> &ofxSuperLog::getLogger(bool writeToConsole, bool drawToScreen, string logDirectory) {
	if(logPtr==NULL) {
		logPtr = new ofxSuperLog(writeToConsole, drawToScreen, logDirectory);
		logger = ofPtr<ofxSuperLog>(logPtr);

	}


	return logger;
}


ofxSuperLog::ofxSuperLog(bool writeToConsole, bool drawToScreen, string logDirectory) {

	this->loggingToFile = logDirectory!="";
	this->loggingToScreen = drawToScreen;
	this->loggingToConsole = writeToConsole;
	this->logDirectory = logDirectory;
	if(loggingToFile) {
		if(!ofFile(logDirectory).exists()) {
			ofDirectory dir(logDirectory);
			dir.create();
		}

		fileLogger.setFile(logDirectory + "/" + ofGetTimestampString() + ".log");
	}
	if(drawToScreen) {
		displayLogger.setEnabled(true);
	}
}

ofxSuperLog::~ofxSuperLog() {
}




void ofxSuperLog::archiveOldLogs(int numUncompressedToKeep, int numCompressedToKeep) {
    if(numUncompressedToKeep==-1) return;
    if(logDirectory=="") {
        ofLogError() << "Must specify a log directory - can't be \"\"";
        return;
    }
    ofDirectory dir;
    dir.allowExt("log");
    dir.listDir(logDirectory);
    dir.sort();
   // while(numUncompressedToKeep<dir.size())
   ofLogError() << "This isn't implemented yet!";

   /*
   // MUST use binary!
std::ofstream out("test.zip", std::ios::binary);
Compress c(out, true);
Poco::Path aFile("c:\\data\\hello.txt");
c.addFile(theFile, "hello.txt");
c.close(); // MUST be done to finalize the Zip file

*/
}


void ofxSuperLog::log(ofLogLevel level, const string & module, const string & message) {

	if(loggingToFile) fileLogger.log(level, module, message);
	if(loggingToConsole) consoleLogger.log(level, module, message);
	if(loggingToScreen) displayLogger.log(level, module, message);

}

void ofxSuperLog::log(ofLogLevel logLevel, const string & module, const char* format, ...) {
	va_list args;
	va_start(args, format);
	log(logLevel, module, format, args);
	va_end(args);
}


void ofxSuperLog::log(ofLogLevel logLevel, const string & module, const char* format, va_list args) {
	if(loggingToFile) fileLogger.log(logLevel, module, format, args);
	if(loggingToConsole) consoleLogger.log(logLevel, module, format, args);
	if(loggingToScreen) displayLogger.log(logLevel, module, format, args);

}

void ofxSuperLog::setMaxNumLogLines(int maxNumLogLines) {
	displayLogger.setMaxNumLogLines(maxNumLogLines);
}

bool ofxSuperLog::isScreenLoggingEnabled() {
	return displayLogger.isEnabled();
}
void ofxSuperLog::setScreenLoggingEnabled(bool enabled) {
	displayLogger.setEnabled(enabled);
}


void ofxSuperLog::setMaximized(bool maximized){
	displayLogger.setMinimized(!maximized);
}

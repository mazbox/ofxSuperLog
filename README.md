ofxSuperLog
===========
This enhances the ofLog system to allow you to have one log call that simultaneously writes to a log file (with a timestamped name, per-launch) and to the screen.

To use, in testApp::setup()

Call
	ofSetLoggerChannel(ofxSuperLog::getLogger(true, true, "logs"));

Where the first paramter is whether you want to write to the console, the second is whether you want to draw it to the screen, and the third, if you want to write to a file, what the folder is called.

If you specify a folder that doesn't exist it will create one for you.

There is some basic interaction with the screen based logging so you can minimize it. If you want to hide it completely you'd call this:

ofxSuperLog::getLogger()->setScreenLoggingEnabled(false);
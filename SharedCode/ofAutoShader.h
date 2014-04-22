#pragma once

class ofAutoShader : public ofShader {
public:
	void setup(string name) {
		this->name = name;
		ofEventArgs args;
		update(args);
		ofAddListener(ofEvents().update, this, &ofAutoShader::update);
	}
	
	void update(ofEventArgs &args) {	
		bool needsReload = false;
			
		string fragName = name + ".frag";
		ofFile fragFile(fragName);
		if(fragFile.exists()) {
			Poco::Timestamp fragTimestamp = fragFile.getPocoFile().getLastModified();
			if(fragTimestamp != lastFragTimestamp) {
				needsReload = true;
				lastFragTimestamp = fragTimestamp;
			}
		} else {
			fragName = "";
		}
		
		string vertName = name + ".vert";
		ofFile vertFile(vertName);
		if(vertFile.exists()) {
			Poco::Timestamp vertTimestamp = vertFile.getPocoFile().getLastModified();
			if(vertTimestamp != lastVertTimestamp) {
				needsReload = true;
				lastVertTimestamp = vertTimestamp;
			}
		} else {
			vertName = "";
		}
		
		if(needsReload) {
			ofLogVerbose("ofAutoShader") << "reloading shader at " << ofGetTimestampString("%H:%M:%S");
			ofShader::load(vertName, fragName);
		}
	}
private:
	string name;
	Poco::Timestamp lastFragTimestamp, lastVertTimestamp;
};

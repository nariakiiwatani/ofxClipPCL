#pragma once

#include "ofMain.h"
#include "ofxClipPCLGeometry.h"
#include "ofxImGui.h"
#include "ofxImGuizmo.h"

class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
private:
	std::shared_ptr<ofx::clippcl::Geometry> geom_;
	ofxImGui::Gui gui_;
	ImGuizmo::OPERATION op_;
	ImGuizmo::MODE mode_;

	ofCamera camera_;
	ofMesh mesh_;
};

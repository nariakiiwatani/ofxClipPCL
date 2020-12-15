#include "ofApp.h"

using namespace ofx::clippcl;
using namespace std;

//--------------------------------------------------------------
void ofApp::setup(){
	mesh_ = ofMesh::sphere(100, 64);
	auto clipper = make_shared<ofx::clippcl::ShapeGroupAny>();
	clipper->addShape<Plane>(glm::vec3{1,0,0}, 0);
	clipper->addShape<Plane>(glm::vec3{0,1,0}, 0);
	shape_ = clipper;
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofPushMatrix();
	ofTranslate(ofGetWindowSize()/2.f);
	if(ofGetMousePressed()) {
		shape_->getValid(mesh_).drawVertices();
	}
	else {
		mesh_.drawVertices();
	}
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

#include "ofApp.h"
#include "ofxClipPCL.h"
#include "ofxClipPCLShapes.h"
#include "shadergen/ofxClipPCLShaderGenerator.h"

using namespace ofx::clippcl;
using namespace std;

//--------------------------------------------------------------
void ofApp::setup(){
	mesh_ = ofMesh::sphere(100, 64);
	auto clipper = make_shared<ofx::clippcl::ClipperGroupAll>();
	clipper->addClipper<Plane>(glm::vec3{1,0,0}, 0);
	clipper->addClipper<Plane>(glm::vec3{0,1,0}, 0);
	std::cout << shader::Generator(*clipper).createMain() << std::endl;
	clipper_ = clipper;
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofPushMatrix();
	ofTranslate(ofGetWindowSize()/2.f);
	if(ofGetMousePressed()) {
		shader_.begin();
		mesh_.drawVertices();
		shader_.end();
		//		clipper_->getValid(mesh_).drawVertices();
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
	shader_ = shader::Generator(*clipper_).createShader();
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

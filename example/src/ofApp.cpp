#include "ofApp.h"
#include "ofxClipPCL.h"
#include "ofxClipPCLGeometry.h"
#include "shadergen/ofxClipPCLShaderGenerator.h"

using namespace ofx::clippcl;
using namespace std;

//--------------------------------------------------------------
void ofApp::setup(){
	mesh_ = ofMesh::sphere(100, 64);
	auto clipper = make_shared<Not<ClipperGroupAll>>();
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
	shader_.setupShaderFromSource(GL_VERTEX_SHADER, R"(
	#version 410

	layout (location = 0) in vec3 position;

	uniform mat4 modelViewProjectionMatrix;

	flat out int valid;

	bool ofxClipPCLMainFunc(vec3 position);
							  
	void main()
	{
		valid = int(ofxClipPCLMainFunc(position));
		gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);
	}
	)");
	
	shader_.setupShaderFromSource(GL_FRAGMENT_SHADER, R"(
	#version 410

	flat in int valid;
	out vec4 fragColor;

	void main() {
		fragColor = vec4(1.0, float(valid), 0.0, 1.0);
	}
	)");
	shader::Generator(*clipper_).attachToShader(shader_);
	shader_.linkProgram();
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

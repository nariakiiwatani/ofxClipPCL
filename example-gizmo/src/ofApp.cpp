#include "ofApp.h"

using namespace std;
using namespace ofx::clippcl;

//--------------------------------------------------------------
void ofApp::setup(){
	camera_.setPosition({0,50,100});
	camera_.lookAt({0,0,0});
	geom_ = make_shared<Plane>(glm::vec4{0,1,0,0});
	mesh_ = ofMesh::sphere(10,32);
	gui_.setup();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	camera_.begin();
	geom_->getValid(mesh_).drawVertices();
	camera_.end();
	gui_.begin();
	ImGuizmo::BeginFrame();
	auto mat = geom_->getWorldMatrix();
	if(ImGuizmo::Manipulate(camera_, mat, op_, mode_)) {
		geom_->setWorldMatrix(mat);
	}
	gui_.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key) {
		case 'w': op_ = ImGuizmo::TRANSLATE; break;
		case 'e': op_ = ImGuizmo::SCALE; break;
		case 'r': op_ = ImGuizmo::ROTATE; break;
		case ' ': mode_ = mode_==ImGuizmo::LOCAL?ImGuizmo::WORLD:ImGuizmo::LOCAL; break;
	}

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

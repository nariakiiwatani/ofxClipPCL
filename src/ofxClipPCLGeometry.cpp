#include "ofxClipPCLGeometry.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>
#include "of3dGraphics.h"

using namespace ofx::clippcl;
using namespace std;

glm::mat4 Geometry::getWorldMatrix() const {
	return node_.getGlobalTransformMatrix();
}
void Geometry::setWorldMatrix(const glm::mat4 &mat) {
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(mat, scale, rotation, translation, skew, perspective);
	
	node_.setGlobalPosition(translation);
	node_.setGlobalOrientation(rotation);
	node_.setScale(scale/(node_.getParent()?node_.getParent()->getGlobalScale():glm::vec3{1,1,1}));
	applyMatrix(node_.getGlobalTransformMatrix());
}

void Geometry::refreshMatrix(const glm::mat4 &mat) {
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(mat, scale, rotation, translation, skew, perspective);
	
	node_.setPosition(translation);
	node_.setOrientation(rotation);
	node_.setScale(scale);
}

void Plane::applyMatrix(const glm::mat4 &mat)
{
	args_ = glm::inverse(glm::transpose(mat))*glm::vec4{0,0,1,0};
}

glm::mat4 Plane::buildMatrix() const
{
	ofNode node;
	node.lookAt(-glm::vec3(args_));
	node.dolly(-args_[3]);
	return node.getLocalTransformMatrix();
}

std::string Plane::getShaderCodeFuncName() const
{
	return "ofxClipPCLFuncPlane";
}
std::vector<std::string> Plane::getArgsForShaderFuncDeclare(const std::string &src_arg) const
{
	return {src_arg, "vec3 normal", "float distance"};
}
std::string Plane::getShaderCodeFuncImpl(const std::string &default_src_arg) const
{
	return "return dot(" + default_src_arg + ", normal) > distance;";
}
std::vector<std::string> Plane::getArgsForShaderFunc(const std::string &src_arg) const
{
	return {src_arg, glm::to_string(glm::vec3(args_)), ofToString(-args_[3])};
}
bool Plane::isValid(const glm::vec3 &point) const {
	return glm::dot(point, glm::vec3(args_)) > -args_[3];
}
void Plane::draw() const
{
	node_.transformGL();
	ofDrawPlane(0,0,0,1,1);
	node_.restoreTransformGL();
}


void Box::applyMatrix(const glm::mat4 &mat)
{
	inv_mat_ = glm::inverse(mat);
}

void Box::draw() const
{
	node_.transformGL();
	ofDrawBox(2);
	node_.restoreTransformGL();
}
bool Box::isValid(const glm::vec3 &point) const
{
	auto p = glm::abs(inv_mat_*glm::vec4(point, 1));
	return std::max(p.x, std::max(p.y, p.z)) < 1;
}

std::string Box::getShaderCodeFuncName() const
{
	return "ofxClipPCLFuncBox";
}
std::vector<std::string> Box::getArgsForShaderFuncDeclare(const std::string &src_arg) const
{
	return {src_arg, "mat4 inv_mat"};
}
std::string Box::getShaderCodeFuncImpl(const std::string &default_src_arg) const
{
	return R"(
	vec4 p = abs(inv_mat*vec4()" + default_src_arg + R"(,1));
	return max(p.x, max(p.y, p.z)) < 1;
	)";
}
std::vector<std::string> Box::getArgsForShaderFunc(const std::string &src_arg) const
{
	
	return {src_arg, "mat4("+
		ofJoinString({
			glm::to_string(inv_mat_[0]),
			glm::to_string(inv_mat_[1]),
			glm::to_string(inv_mat_[2]),
			glm::to_string(inv_mat_[3])}, ",")+")"};
}

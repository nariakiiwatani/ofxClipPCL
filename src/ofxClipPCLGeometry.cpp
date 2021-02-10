#include "ofxClipPCLGeometry.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>
#include "of3dGraphics.h"
#include "ofGraphics.h"
#include "ofNode.h"

using namespace ofx::clippcl;
using namespace std;

glm::mat4 Plane::buildMatrix(const glm::vec4 &args, const glm::vec3 &scale) const
{
	ofNode node;
	node.lookAt(-glm::vec3(args));
	node.dolly(-args[3]);
	node.setScale(scale);
	return node.getLocalTransformMatrix();
}

void Plane::setMatrix(const glm::mat4 &mat)
{
	mat_ = mat;
	glm::vec4 args = glm::inverse(glm::transpose(mat))*glm::vec4{0,0,1,0};
	normal_ = args;
	distance_ = args[3];
	scale_ = glm::vec3(glm::length(glm::vec3(mat[0])),
					   glm::length(glm::vec3(mat[1])),
					   glm::length(glm::vec3(mat[2])));
}

glm::mat4 Plane::getMatrix() const
{
	return mat_;
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
	return {src_arg, glm::to_string(normal_), ofToString(-distance_)};
}
bool Plane::isValid(const glm::vec3 &point) const {
	return glm::dot(point, normal_) > -distance_;
}
void Plane::draw() const
{
	ofPushMatrix();
	ofMultMatrix(mat_);
	ofDrawPlane(0,0,0,1,1);
	ofPopMatrix();
}


glm::mat4 Box::getMatrix() const
{
	return mat_;
}
void Box::setMatrix(const glm::mat4 &mat)
{
	mat_ = mat;
	inv_mat_ = glm::inverse(mat);
}

void Box::draw() const
{
	ofPushMatrix();
	ofMultMatrix(mat_);
	ofDrawBox(2);
	ofPopMatrix();
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


glm::mat4 Sphere::getMatrix() const
{
	return mat_;
}
void Sphere::setMatrix(const glm::mat4 &mat)
{
	mat_ = mat;
	inv_mat_ = glm::inverse(mat);
}

void Sphere::draw() const
{
	ofPushMatrix();
	ofMultMatrix(mat_);
	ofDrawSphere(1);
	ofPopMatrix();
}
bool Sphere::isValid(const glm::vec3 &point) const
{
	return glm::length2(glm::vec3(inv_mat_*glm::vec4(point, 1))) < 1;
}

std::string Sphere::getShaderCodeFuncName() const
{
	return "ofxClipPCLFuncSphere";
}
std::vector<std::string> Sphere::getArgsForShaderFuncDeclare(const std::string &src_arg) const
{
	return {src_arg, "mat4 inv_mat"};
}
std::string Sphere::getShaderCodeFuncImpl(const std::string &default_src_arg) const
{
	return R"(
	vec3 p = (inv_mat*vec4()" + default_src_arg + R"(,1)).xyz;
	return dot(p,p) < 1;
	)";
}
std::vector<std::string> Sphere::getArgsForShaderFunc(const std::string &src_arg) const
{
	
	return {src_arg, "mat4("+
		ofJoinString({
			glm::to_string(inv_mat_[0]),
			glm::to_string(inv_mat_[1]),
			glm::to_string(inv_mat_[2]),
			glm::to_string(inv_mat_[3])}, ",")+")"};
}

glm::mat4 Cone::getMatrix() const
{
	return mat_;
}
void Cone::setMatrix(const glm::mat4 &mat)
{
	mat_ = mat;
	inv_mat_ = glm::inverse(mat);
}

void Cone::draw() const
{
	ofPushMatrix();
	ofMultMatrix(mat_);
	ofDrawCone(0,0.5f,0, 1,-1);
	ofPopMatrix();
}
bool Cone::isValid(const glm::vec3 &point) const
{
	auto pos = glm::vec3(inv_mat_*glm::vec4(point, 1));
	auto xz = glm::vec2(pos.x,pos.z);
	auto y = pos.y;
	return y > 0 && y < 1 && glm::length2(xz) < (1-y)*(1-y);
}

std::string Cone::getShaderCodeFuncName() const
{
	return "ofxClipPCLFuncCone";
}
std::vector<std::string> Cone::getArgsForShaderFuncDeclare(const std::string &src_arg) const
{
	return {src_arg, "mat4 inv_mat"};
}
std::string Cone::getShaderCodeFuncImpl(const std::string &default_src_arg) const
{
	return R"(
	vec3 p = (inv_mat*vec4()" + default_src_arg + R"(,1)).xyz;
	return p.y > 0 && p.y < 1 && dot(p.xz,p.xz) < (1-p.y)*(1-p.y);
	)";
}
std::vector<std::string> Cone::getArgsForShaderFunc(const std::string &src_arg) const
{
	
	return {src_arg, "mat4("+
		ofJoinString({
			glm::to_string(inv_mat_[0]),
			glm::to_string(inv_mat_[1]),
			glm::to_string(inv_mat_[2]),
			glm::to_string(inv_mat_[3])}, ",")+")"};
}

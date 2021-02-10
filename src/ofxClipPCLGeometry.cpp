#include "ofxClipPCLGeometry.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>
#include "of3dGraphics.h"
#include "ofGraphics.h"
#include "ofNode.h"

using namespace ofx::clippcl;
using namespace std;

void GeometryByMatrix::setMatrix(const glm::mat4 &mat)
{
	mat_ = mat;
	inv_mat_ = glm::inverse(mat);
}

void GeometryByMatrix::draw() const
{
	ofPushMatrix();
	ofMultMatrix(mat_);
	drawLocal();
	ofPopMatrix();
}

bool GeometryByMatrix::isValid(const glm::vec3 &point) const
{
	return isValidLocal(inv_mat_*glm::vec4(point, 1));
}

std::vector<std::string> GeometryByMatrix::getArgsForShaderFuncDeclare(const std::string &src_arg) const
{
	return {src_arg, "mat4 inv_mat"};
}
std::vector<std::string> GeometryByMatrix::getArgsForShaderFunc(const std::string &src_arg) const
{
	return {src_arg, "mat4("+
		ofJoinString({
			glm::to_string(inv_mat_[0]),
			glm::to_string(inv_mat_[1]),
			glm::to_string(inv_mat_[2]),
			glm::to_string(inv_mat_[3])}, ",")+")"};
}

std::string GeometryByMatrix::getShaderCodeFuncImpl(const std::string &default_src_arg) const
{
	auto local_pos_name = default_src_arg+"_local";
	return R"(
	vec3 )" + local_pos_name + R"( = (inv_mat*vec4()" + default_src_arg + R"(,1)).xyz;
	)" + getShaderCodeFuncImplLocal(local_pos_name);	
}


glm::mat4 Plane::buildMatrix(const glm::vec4 &args, const glm::vec3 &scale) const
{
	ofNode node;
	node.lookAt(-glm::vec3(args));
	node.dolly(-args[3]);
	node.setScale(scale);
	return node.getLocalTransformMatrix();
}

std::string Plane::getShaderCodeFuncImplLocal(const std::string &default_src_arg) const
{
	return "return " + default_src_arg + ".z > 0;";
}

bool Plane::isValidLocal(const glm::vec3 &point) const
{
	return point.z > 0;
}

void Plane::drawLocal() const
{
	ofDrawPlane(0,0,0,1,1);
}


void Box::drawLocal() const
{
	ofDrawBox(2);
}
bool Box::isValidLocal(const glm::vec3 &point) const
{
	auto p = glm::abs(point);
	return std::max(p.x, std::max(p.y, p.z)) < 1;
}

std::string Box::getShaderCodeFuncImplLocal(const std::string &default_src_arg) const
{
	return R"(
	vec3 p = abs()" + default_src_arg + R"();
	return max(p.x, max(p.y, p.z)) < 1;
	)";
}


void Sphere::drawLocal() const
{
	ofDrawSphere(1);
}
bool Sphere::isValidLocal(const glm::vec3 &point) const
{
	return glm::length2(point) < 1;
}

std::string Sphere::getShaderCodeFuncImplLocal(const std::string &default_src_arg) const
{
	return "return dot("+default_src_arg+","+default_src_arg+") < 1;";
}

void Cylinder::drawLocal() const
{
	ofDrawCylinder(1,2);
}
bool Cylinder::isValidLocal(const glm::vec3 &point) const
{
	return glm::length2(glm::vec2(point.x,point.z)) < 1 && glm::abs(point.y) < 1;
}

std::string Cylinder::getShaderCodeFuncImplLocal(const std::string &default_src_arg) const
{
	return R"(
	vec3 p = )" + default_src_arg + R"(;
	return p.y*p.y < 1 && dot(p.xz,p.xz) < 1;
	)";
}

void Cone::drawLocal() const
{
	ofDrawCone(0,0.5f,0, 1,-1);
}
bool Cone::isValidLocal(const glm::vec3 &point) const
{
	auto xz = glm::vec2(point.x,point.z);
	auto y = point.y;
	return y > 0 && y < 1 && glm::length2(xz) < (1-y)*(1-y);
}

std::string Cone::getShaderCodeFuncImplLocal(const std::string &default_src_arg) const
{
	return R"(
	vec3 p = )" + default_src_arg + R"(;
	return p.y > 0 && p.y < 1 && dot(p.xz,p.xz) < (1-p.y)*(1-p.y);
	)";
}

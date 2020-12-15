#include "ofxClipPCLShapes.h"
#include <glm/gtx/string_cast.hpp>

using namespace ofx::clippcl;
using namespace std;

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
	return {src_arg, glm::to_string(normal_), ofToString(distance_)};
}

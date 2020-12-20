#include "ofxClipPCL.h"
#include "ofxClipPCLShaderGenerator.h"
#include "ofxJsonUtils.h"
#include "ofxClipPCLGeometry.h"

using namespace ofx::clippcl;
using namespace std;

ofMesh Clipper::getValid(const ofMesh &src) const
{
	vector<ofIndexType> use;
	auto &verts = src.getVertices();
	use.reserve(verts.size());
	for(ofIndexType i = 0; i < verts.size(); ++i) {
		if(isValid(verts[i])) use.push_back(i);
	}
	ofMesh ret;
	ret.getVertices().resize(use.size());
	if(src.hasColors()) ret.getColors().resize(use.size());
	if(src.hasNormals()) ret.getNormals().resize(use.size());
	if(src.hasTexCoords()) ret.getTexCoords().resize(use.size());

	for(ofIndexType i = 0; i < use.size(); ++i) {
		ret.setVertex(i, src.getVertex(use[i]));
		if(src.hasColors()) ret.setColor(i, src.getColor(use[i]));
		if(src.hasNormals()) ret.setNormal(i, src.getNormal(use[i]));
		if(src.hasTexCoords()) ret.setTexCoord(i, src.getTexCoord(use[i]));
	}
	return ret;
}

std::string Clipper::getShaderCodeFunc(const std::string &default_arg_type, const std::string &default_arg_name) const
{
	return "bool " + getShaderCodeFuncName() + "(" + ofJoinString(getArgsForShaderFuncDeclare(default_arg_type+" "+default_arg_name), ", ") + ") {" + getShaderCodeFuncImpl(default_arg_name) + "}";
}
std::string Clipper::getShaderCodeFuncCall(const std::string &default_src_arg) const
{
	return getShaderCodeFuncName() + "(" + ofJoinString(getArgsForShaderFunc(default_src_arg), ",") + ")";
}

std::vector<std::string> ClipperGroup::getArgsForShaderFuncDeclare(const std::string &src_arg) const
{
	return {"bool["+ofToString(clippers_.size())+"] results"};
}
std::vector<std::string> ClipperGroup::getArgsForShaderFunc(const std::string &src_arg) const
{
	std::vector<std::string> funcs;
	funcs.reserve(clippers_.size());
	for(auto &&clipper : clippers_) {
		funcs.push_back(clipper->getShaderCodeFuncCall(src_arg));
	}
	return {"bool["+ofToString(clippers_.size())+"]("
		+ ofJoinString(funcs, ",") + 
		")"
	};
}

std::string ClipperGroupAll::getShaderCodeFuncName() const
{
	return "ofxClipPCLFuncGroupAll"+ofToString(clippers_.size());
}
std::string ClipperGroupAll::getShaderCodeFuncImpl(const std::string &default_src_arg) const
{
	return R"(
	for(int ptr = 0; ptr < )" + ofToString(clippers_.size()) + R"(; ++ptr) {
		if(!results[ptr]) return false;
	}
	return true;
	)";
}
bool ClipperGroupAll::isValid(const glm::vec3 &point) const
{
	return all_of(begin(clippers_), end(clippers_), [point](shared_ptr<Clipper> clipper) {
		return clipper->isValid(point);
	});
}

ofJson ClipperGroupAll::toJson() const {
	auto children = getClippers();
	return {
		{"type", getTypeForSerialize()},
		{
			"children", accumulate(begin(children), end(children), ofJson(), [](ofJson sum, shared_ptr<Clipper> clipper) {
				sum.push_back(clipper->toJson());
				return sum;
			})
		}
	};
}

void ClipperGroupAll::loadJson(const ofJson &json)
{
	typeAssert(json["type"]);
	clippers_.clear();
	for(auto &&j : json["children"]) {
		if(j["type"] == "plane") {
			add<Plane>()->loadJson(j);
		}
		else {
			add<Clipper>()->loadJson(j);
		}
	}
}


std::string ClipperGroupAny::getShaderCodeFuncName() const
{
	return "ofxClipPCLFuncGroupAny"+ofToString(clippers_.size());
}
std::string ClipperGroupAny::getShaderCodeFuncImpl(const std::string &default_src_arg) const
{
	return R"(
	for(int ptr = 0; ptr < )" + ofToString(clippers_.size()) + R"(; ++ptr) {
		if(results[ptr]) return true;
	}
	return false;
	)";
}
bool ClipperGroupAny::isValid(const glm::vec3 &point) const
{
	return any_of(begin(clippers_), end(clippers_), [point](shared_ptr<Clipper> clipper) {
		return clipper->isValid(point);
	});
}


#include "ofxClipPCL.h"
#include "ofxClipPCLShaderGenerator.h"
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
	return {"bool result"};
}
std::string ClipperGroup::getShaderCodeFuncName() const
{
	return "ofxClipPCLFuncGroup";
}
std::string ClipperGroup::getShaderCodeFuncImpl(const std::string &default_src_arg) const
{
	return "return result;";
}

std::vector<std::string> ClipperGroupAll::getArgsForShaderFunc(const std::string &src_arg) const
{
	if(clippers_.empty()) {
		return {"(true)"};
	}
	std::vector<std::string> funcs;
	funcs.reserve(clippers_.size());
	for(auto &&clipper : clippers_) {
		funcs.push_back(clipper->getShaderCodeFuncCall(src_arg));
	}
	return {"("+ofJoinString(funcs, "\n&& ")+")"};
}

bool ClipperGroupAll::isValid(const glm::vec3 &point) const
{
	return all_of(begin(clippers_), end(clippers_), [point](shared_ptr<Clipper> clipper) {
		return clipper->isValid(point);
	});
}

std::vector<std::string> ClipperGroupAny::getArgsForShaderFunc(const std::string &src_arg) const
{
	if(clippers_.empty()) {
		return {"(false)"};
	}
	std::vector<std::string> funcs;
	funcs.reserve(clippers_.size());
	for(auto &&clipper : clippers_) {
		funcs.push_back(clipper->getShaderCodeFuncCall(src_arg));
	}
	return {"("+ofJoinString(funcs, "\n|| ")+")"};
}

bool ClipperGroupAny::isValid(const glm::vec3 &point) const
{
	return any_of(begin(clippers_), end(clippers_), [point](shared_ptr<Clipper> clipper) {
		return clipper->isValid(point);
	});
}


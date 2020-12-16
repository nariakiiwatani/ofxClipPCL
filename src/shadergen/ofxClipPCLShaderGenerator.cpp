#include "ofxClipPCLShaderGenerator.h"
#include "ofShader.h"

using namespace ofx::clippcl;
using namespace ofx::clippcl::shader;

void Generator::attachToShader(ofShader &dst, const std::string &clipping_func_name) const
{
	GLuint shader = glCreateShader(GL_VERTEX_SHADER);
	auto &&source = createMain(clipping_func_name);
	const char* sptr = source.c_str();
	int ssize = source.size();
	glShaderSource(shader, 1, &sptr, &ssize);
	glCompileShader(shader);
	glAttachShader(dst.getProgram(), shader);
	glDeleteShader(shader);
}

std::string Generator::createFuncs() const
{
	std::string ret;
	auto funcs = createFunc(clipper_);
	for(auto &&func : funcs) {
		ret += func.second + "\n";
	}
	return ret;
}

std::string Generator::createMain(const std::string &func_name) const
{
	return R"(#version 410
	)" + createFuncs() + R"(
	
	bool )" + func_name + R"((vec3 position) {
	return )" + clipper_.getShaderCodeFuncCall("position") + R"(;
	})";
}


std::map<std::string, std::string> Generator::createFunc(const ::ofx::clippcl::Clipper &src, bool check_if_group) const
{
	if(check_if_group) {
		if(auto group = dynamic_cast<const ::ofx::clippcl::ClipperGroup*>(&src)) {
			return createFunc(*group);
		}
	}
	return {{src.getShaderCodeFuncName(), src.getShaderCodeFunc("vec3", "position")}};
}
std::map<std::string, std::string> Generator::createFunc(const ::ofx::clippcl::ClipperGroup &src) const
{
	std::map<std::string, std::string> ret = createFunc(src, false);
	for(auto &&clipper : src.getClippers()) {
		auto funcs = createFunc(*clipper);
		ret.insert(begin(funcs), end(funcs));
	}
	return ret;
}

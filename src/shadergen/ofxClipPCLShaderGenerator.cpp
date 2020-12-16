#include "ofxClipPCLShaderGenerator.h"
#include "ofShader.h"

using namespace ofx::clippcl;
using namespace ofx::clippcl::shader;

namespace {
std::string makeFuncSignature(std::string ret_type, std::string func_name, std::vector<std::string> args) {
	return ret_type + " " + func_name + "("+ofJoinString(args,",")+")";
}
std::string makeFuncCall(std::string func_name, std::vector<std::string> args) {
	return func_name + "(" + ofJoinString(args, ",") + ")";
}
void attachShader(ofShader &dst, const std::string &source, GLuint type) {
	GLuint shader = glCreateShader(type);
	const char* sptr = source.c_str();
	int ssize = source.size();
	glShaderSource(shader, 1, &sptr, &ssize);
	glCompileShader(shader);
	glAttachShader(dst.getProgram(), shader);
	glDeleteShader(shader);
}
}
ofShader Generator::createShader() const
{
	ofShader ret;

	ret.setupShaderFromSource(GL_VERTEX_SHADER, R"(
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
	
	ret.setupShaderFromSource(GL_FRAGMENT_SHADER, R"(
	#version 410

	flat in int valid;
	out vec4 fragColor;

	void main() {
		fragColor = vec4(1.0, float(valid), 0.0, 1.0);
	}
	)");
	
	attachShader(ret, createMain(), GL_VERTEX_SHADER);
	ret.linkProgram();
	return ret;
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

std::string Generator::createCall() const
{
	return makeFuncCall(clipper_.getShaderCodeFuncName(), clipper_.getArgsForShaderFunc("position"));
}

std::string Generator::createMain() const
{
	return R"(#version 410
	)" + createFuncs() + R"(
	
	bool ofxClipPCLMainFunc(vec3 position) {
		return )" + createCall() + R"(;
	})";
}


std::map<std::string, std::string> Generator::createFunc(const ::ofx::clippcl::Clipper &src, bool check_if_group) const
{
	if(check_if_group) {
		if(auto group = dynamic_cast<const ::ofx::clippcl::ClipperGroup*>(&src)) {
			return createFunc(*group);
		}
	}
	return {{src.getShaderCodeFuncName(), makeFuncSignature("bool", src.getShaderCodeFuncName(), src.getArgsForShaderFuncDeclare("vec3 point")) + "{" + src.getShaderCodeFuncImpl("point") + "}"}};
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

#pragma once

#include "ofxClipPCL.h"

namespace ofx { namespace clippcl {
namespace shader {
class Generator
{
public:
	Generator(::ofx::clippcl::Clipper *src):clipper_(src){}
	ofShader createShader() const;
	std::string createFuncs() const;
	std::string createCall() const;
	std::string createMain() const;
private:
	::ofx::clippcl::Clipper *clipper_;

	std::map<std::string, std::string> createFunc(::ofx::clippcl::Clipper *src, bool check_if_group=true) const;
	std::map<std::string, std::string> createFunc(::ofx::clippcl::ClipperGroup *src) const;
};
}
}}

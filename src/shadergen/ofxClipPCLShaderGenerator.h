#pragma once

#include "ofxClipPCL.h"

namespace ofx { namespace clippcl {
namespace shader {
class Generator
{
public:
	Generator(::ofx::clippcl::Clipper &src):clipper_(src){}
	void attachToShader(ofShader &shader, const std::string &clipping_func_name="ofxClipPCLMainFunc") const;
	std::string createFuncs() const;
	std::string createMain(const std::string &funcname="ofxClipPCLMainFunc") const;
private:
	::ofx::clippcl::Clipper &clipper_;

	std::map<std::string, std::string> createFunc(const ::ofx::clippcl::Clipper &src, bool check_if_group=true) const;
	std::map<std::string, std::string> createFunc(const ::ofx::clippcl::ClipperGroup &src) const;
};
}
}}

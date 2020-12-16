#pragma once

#include "ofxClipPCL.h"

namespace ofx { namespace clippcl {

class Plane : public Clipper
{
public:
	Plane(const glm::vec3 &normal, float distance)
	:normal_(glm::normalize(normal))
	,distance_(distance)
	{}
	bool isValid(const glm::vec3 &point) const override {
		return glm::dot(point, normal_) > distance_;
	}

	std::string getShaderCodeFuncName() const override;
	std::vector<std::string> getArgsForShaderFuncDeclare(const std::string &src_arg) const override;
	std::string getShaderCodeFuncImpl(const std::string &default_src_arg) const override;
	std::vector<std::string> getArgsForShaderFunc(const std::string &src_arg) const override;
private:
	glm::vec3 normal_ = glm::vec3(0,1,0);
	float distance_ = 0;
};
}}

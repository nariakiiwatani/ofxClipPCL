#pragma once

#include "ofxClipPCL.h"
#include "ofNode.h"

namespace ofx { namespace clippcl {

class Geometry : public Clipper
{
public:
	glm::mat4 getWorldMatrix() const;
	void setWorldMatrix(const glm::mat4 &mat);
	virtual void draw() const {}
	ofNode& getNode() { return node_; }
protected:
	ofNode node_;
	virtual void applyMatrix(const glm::mat4 &mat) {}
	void refreshMatrix(const glm::mat4 &mat);
};

class Plane : public Geometry
{
public:
	Plane(const glm::vec4 &args)
	:args_(args) {
		refreshMatrix(buildMatrix());
	}
	Plane(const glm::vec3 &normal, float distance)
	:Plane(glm::vec4(normal,distance))
	{}
	void draw() const override;
	bool isValid(const glm::vec3 &point) const override;

	std::string getShaderCodeFuncName() const override;
	std::vector<std::string> getArgsForShaderFuncDeclare(const std::string &src_arg) const override;
	std::string getShaderCodeFuncImpl(const std::string &default_src_arg) const override;
	std::vector<std::string> getArgsForShaderFunc(const std::string &src_arg) const override;
	
	virtual ofJson toJson() const override;
private:
	void applyMatrix(const glm::mat4 &mat) override;
	glm::mat4 buildMatrix() const;
	glm::vec4 args_ = glm::vec4(0,1,0,0);
};
}}

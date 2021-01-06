#pragma once

#include "ofxClipPCL.h"
#include "ofNode.h"

namespace ofx { namespace clippcl {

class Geometry : public Clipper
{
public:
	OFX_CLIPPCL_ACCEPTOR_FUNCTIONS_OVERRIDE
	
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
	OFX_CLIPPCL_ACCEPTOR_FUNCTIONS_OVERRIDE
	
	Plane() = default;
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
private:
	void applyMatrix(const glm::mat4 &mat) override;
	glm::mat4 buildMatrix() const;
	glm::vec4 args_ = glm::vec4(0,1,0,0);
};

class Box : public Geometry
{
public:
	OFX_CLIPPCL_ACCEPTOR_FUNCTIONS_OVERRIDE
	
	Box() = default;
	Box(const glm::mat4 &mat)
	:inv_mat_(glm::inverse(mat)){
		refreshMatrix(mat);
	}
	Box(const glm::vec3 &t, const glm::quat &r, const glm::vec3 &s)
	:Box(glm::translate(t)
		 *glm::mat4_cast(r)
		 *glm::scale(s)){
	}
	Box(const glm::vec3 &center, float width, float height, float depth)
	:Box(center, glm::quat(), glm::vec3(width, height, depth))
	{}
	void draw() const override;
	bool isValid(const glm::vec3 &point) const override;
	
	std::string getShaderCodeFuncName() const override;
	std::vector<std::string> getArgsForShaderFuncDeclare(const std::string &src_arg) const override;
	std::string getShaderCodeFuncImpl(const std::string &default_src_arg) const override;
	std::vector<std::string> getArgsForShaderFunc(const std::string &src_arg) const override;
private:
	void applyMatrix(const glm::mat4 &mat) override;
	glm::mat4 inv_mat_;
};
}}

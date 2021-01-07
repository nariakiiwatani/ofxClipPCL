#pragma once

#include "ofxClipPCL.h"

namespace ofx { namespace clippcl {

class Geometry : public Clipper
{
public:
	OFX_CLIPPCL_ACCEPTOR_FUNCTIONS_OVERRIDE
	
	Geometry()=default;
	Geometry(const glm::mat4 &mat) {
		setMatrix(mat);
	}
	Geometry(const glm::vec3 &translate, const glm::quat &rotate, const glm::vec3 &scale)
	:Geometry(glm::translate(translate)
			  *glm::mat4_cast(rotate)
			  *glm::scale(scale)) {
	}

	virtual glm::mat4 getMatrix() const = 0;
	virtual void setMatrix(const glm::mat4 &mat) {}
	virtual void draw() const {}
};

class Plane : public Geometry
{
public:
	OFX_CLIPPCL_ACCEPTOR_FUNCTIONS_OVERRIDE
	
	using Geometry::Geometry;
	Plane(const glm::vec4 &args)
	:Plane(buildMatrix(args)) {
	}
	Plane(const glm::vec3 &normal, float distance)
	:Plane(glm::vec4(normal, distance)) {
	}

	glm::mat4 getMatrix() const override;
	void setMatrix(const glm::mat4 &mat) override;
	void draw() const override;
	bool isValid(const glm::vec3 &point) const override;

	std::string getShaderCodeFuncName() const override;
	std::vector<std::string> getArgsForShaderFuncDeclare(const std::string &src_arg) const override;
	std::string getShaderCodeFuncImpl(const std::string &default_src_arg) const override;
	std::vector<std::string> getArgsForShaderFunc(const std::string &src_arg) const override;
private:
	glm::mat4 mat_;
	glm::vec3 normal_ = glm::vec3(0,0,1);
	float distance_ = 0;
	glm::vec3 scale_ = glm::vec3(1);
	glm::mat4 buildMatrix(const glm::vec4 &args, const glm::vec3 &scale=glm::vec3(1)) const;
};

class Box : public Geometry
{
public:
	OFX_CLIPPCL_ACCEPTOR_FUNCTIONS_OVERRIDE
	
	using Geometry::Geometry;
	Box(const glm::vec3 &center, float width, float height, float depth)
	:Box(center, glm::quat(), glm::vec3(width, height, depth))
	{}

	glm::mat4 getMatrix() const override;
	void setMatrix(const glm::mat4 &mat) override;
	void draw() const override;
	bool isValid(const glm::vec3 &point) const override;
	
	std::string getShaderCodeFuncName() const override;
	std::vector<std::string> getArgsForShaderFuncDeclare(const std::string &src_arg) const override;
	std::string getShaderCodeFuncImpl(const std::string &default_src_arg) const override;
	std::vector<std::string> getArgsForShaderFunc(const std::string &src_arg) const override;
private:
	glm::mat4 mat_, inv_mat_;
};

class Sphere : public Geometry
{
public:
	OFX_CLIPPCL_ACCEPTOR_FUNCTIONS_OVERRIDE
	
	using Geometry::Geometry;
	Sphere(const glm::vec3 &center, float radius)
	:Sphere(center, glm::quat(), glm::vec3(radius))
	{}
	
	glm::mat4 getMatrix() const override;
	void setMatrix(const glm::mat4 &mat) override;
	void draw() const override;
	bool isValid(const glm::vec3 &point) const override;
	
	std::string getShaderCodeFuncName() const override;
	std::vector<std::string> getArgsForShaderFuncDeclare(const std::string &src_arg) const override;
	std::string getShaderCodeFuncImpl(const std::string &default_src_arg) const override;
	std::vector<std::string> getArgsForShaderFunc(const std::string &src_arg) const override;
private:
	glm::mat4 mat_, inv_mat_;
};
}}

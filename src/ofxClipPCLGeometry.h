#pragma once

#include "ofxClipPCL.h"

namespace ofx { namespace clippcl {

class Geometry : public Clipper
{
public:
	OFX_CLIPPCL_ACCEPTOR_FUNCTIONS_OVERRIDE
	
	virtual void draw() const {}
};

class GeometryByMatrix : public Geometry
{
public:
	OFX_CLIPPCL_ACCEPTOR_FUNCTIONS_OVERRIDE
	
	GeometryByMatrix()=default;
	GeometryByMatrix(const glm::mat4 &mat) {
		setMatrix(mat);
	}
	GeometryByMatrix(const glm::vec3 &translate, const glm::quat &rotate, const glm::vec3 &scale)
	:GeometryByMatrix(glm::translate(translate)
			  *glm::mat4_cast(rotate)
			  *glm::scale(scale)) {
	}
	
	virtual void draw() const override;
	virtual bool isValid(const glm::vec3 &point) const override;
	
	glm::mat4 getMatrix() const { return mat_; }
	virtual void setMatrix(const glm::mat4 &mat);
	
	virtual std::vector<std::string> getArgsForShaderFuncDeclare(const std::string &src_arg) const override;
	virtual std::vector<std::string> getArgsForShaderFunc(const std::string &src_arg) const override;
	virtual std::string getShaderCodeFuncImpl(const std::string &default_src_arg) const override;
protected:
	virtual bool isValidLocal(const glm::vec3 &local_pos) const { return true; }
	virtual std::string getShaderCodeFuncImplLocal(const std::string &default_src_arg) const { return "return true;"; }
	virtual void drawLocal() const {}
	glm::mat4 mat_, inv_mat_;
};
class Plane : public GeometryByMatrix
{
public:
	OFX_CLIPPCL_ACCEPTOR_FUNCTIONS_OVERRIDE
	
	using GeometryByMatrix::GeometryByMatrix;
	Plane():GeometryByMatrix(){}
	Plane(const glm::vec4 &args)
	:GeometryByMatrix(buildMatrix(args)) {
	}
	Plane(const glm::vec3 &normal, float distance)
	:Plane(glm::vec4(normal, distance)) {
	}

	std::string getShaderCodeFuncName() const override { return "ofxClipPCLFuncPlane"; }
private:
	glm::mat4 buildMatrix(const glm::vec4 &args, const glm::vec3 &scale=glm::vec3(1)) const;
	void drawLocal() const override;
	bool isValidLocal(const glm::vec3 &point) const override;
	std::string getShaderCodeFuncImplLocal(const std::string &default_src_arg) const override;
};

class Box : public GeometryByMatrix
{
public:
	OFX_CLIPPCL_ACCEPTOR_FUNCTIONS_OVERRIDE

	using GeometryByMatrix::GeometryByMatrix;
	Box():GeometryByMatrix(){}
	Box(const glm::vec3 &center, float width, float height, float depth)
	:GeometryByMatrix(center, glm::quat(), glm::vec3(width, height, depth)) {
	}

	std::string getShaderCodeFuncName() const override { return "ofxClipPCLFuncBox"; }
private:
	void drawLocal() const override;
	bool isValidLocal(const glm::vec3 &point) const override;
	std::string getShaderCodeFuncImplLocal(const std::string &default_src_arg) const override;
};

class Sphere : public GeometryByMatrix
{
public:
	OFX_CLIPPCL_ACCEPTOR_FUNCTIONS_OVERRIDE
	
	using GeometryByMatrix::GeometryByMatrix;
	Sphere():GeometryByMatrix(){}
	Sphere(const glm::vec3 &center, float radius)
	:GeometryByMatrix(center, glm::quat(), glm::vec3(radius))
	{}
	
	std::string getShaderCodeFuncName() const override { return "ofxClipPCLFuncSphere"; }
private:
	void drawLocal() const override;
	bool isValidLocal(const glm::vec3 &point) const override;
	std::string getShaderCodeFuncImplLocal(const std::string &default_src_arg) const override;
};
class Cylinder : public GeometryByMatrix
{
public:
	OFX_CLIPPCL_ACCEPTOR_FUNCTIONS_OVERRIDE
	
	using GeometryByMatrix::GeometryByMatrix;
	Cylinder():GeometryByMatrix(){}
	Cylinder(const glm::vec3 &center, float radius, float height)
	:GeometryByMatrix(center, glm::quat(), glm::vec3(radius,height,radius))
	{}
	
	std::string getShaderCodeFuncName() const override { return "ofxClipPCLFuncCylinder"; }
private:
	void drawLocal() const override;
	bool isValidLocal(const glm::vec3 &point) const override;
	std::string getShaderCodeFuncImplLocal(const std::string &default_src_arg) const override;
};
class Cone : public GeometryByMatrix
{
public:
	OFX_CLIPPCL_ACCEPTOR_FUNCTIONS_OVERRIDE
	
	using GeometryByMatrix::GeometryByMatrix;
	Cone():GeometryByMatrix(){}
	Cone(const glm::vec3 &center_of_footprint, float radius, float height)
	:GeometryByMatrix(center_of_footprint, glm::quat(), glm::vec3(radius,height,radius))
	{}
	
	std::string getShaderCodeFuncName() const override { return "ofxClipPCLFuncCone"; }
private:
	void drawLocal() const override;
	bool isValidLocal(const glm::vec3 &point) const override;
	std::string getShaderCodeFuncImplLocal(const std::string &default_src_arg) const override;
};
}}

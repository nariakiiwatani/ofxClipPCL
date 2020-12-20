#pragma once

#include "ofMesh.h"
#include "ofJson.h"

namespace ofx { namespace clippcl {
class Clipper
{
public:
	// for clipping by CPU
	virtual bool isValid(const glm::vec3 &point) const { return true; }
	ofMesh getValid(const ofMesh &src) const;
	void reduce(ofMesh &srcdst) const { reduce(srcdst, srcdst); }
	void reduce(const ofMesh &src, ofMesh &dst) const { dst = getValid(src); }

	// clipping by GPU(vertex shader program)
	virtual std::string getShaderCodeFuncName() const { return "ofxClipPCLFuncShape"; }
	virtual std::string getShaderCodeFunc(const std::string &default_arg_type, const std::string &default_arg_name) const;
	virtual std::string getShaderCodeFuncCall(const std::string &default_src_arg) const;
	
	// for serialize/deserialize
	virtual ofJson toJson() const {
		return {{"type", getTypeForSerialize()}};
	}
	virtual void loadJson(const ofJson &json) {
		typeAssert(json["type"]);
	}
	virtual std::string getTypeForSerialize() const { return "clipper"; }

protected:
	virtual std::vector<std::string> getArgsForShaderFuncDeclare(const std::string &src_arg) const {
		return {src_arg};
	}
	virtual std::string getShaderCodeFuncImpl(const std::string &default_src_arg) const {
		return "return true;";
	}
	virtual std::vector<std::string> getArgsForShaderFunc(const std::string &src_arg) const {
		return {src_arg};
	}
	bool typeAssert(const std::string &type) const {
		return type == getTypeForSerialize();
	}
};
template<typename T>
class Not : public T
{
public:
	using T::T;
	bool isValid(const glm::vec3 &point) const override {
		return !T::isValid(point);
	}
	std::string getShaderCodeFuncCall(const std::string &default_src_arg) const override {
		return "!("+T::getShaderCodeFuncCall(default_src_arg)+")";
	}
};
class ClipperGroup : public Clipper
{
public:
	std::vector<std::string> getArgsForShaderFuncDeclare(const std::string &src_arg) const override;
	std::vector<std::string> getArgsForShaderFunc(const std::string &src_arg) const override;
	template<typename T, typename ...Args>
	std::shared_ptr<T> add(Args &&...args) {
		auto clipper = std::make_shared<T>(std::forward<Args>(args)...);
		clippers_.push_back(clipper);
		return clipper;
	}
	std::vector<std::shared_ptr<Clipper>> getClippers() const { return clippers_; }
protected:
	std::vector<std::shared_ptr<Clipper>> clippers_;
};
class ClipperGroupAll : public ClipperGroup
{
public:
	std::string getShaderCodeFuncName() const override;
	std::string getShaderCodeFuncImpl(const std::string &default_src_arg) const override;
	bool isValid(const glm::vec3 &point) const override;
	
	virtual ofJson toJson() const override;
	virtual void loadJson(const ofJson &json) override;
	virtual std::string getTypeForSerialize() const override { return "clipperGroupAll"; }
};
class ClipperGroupAny : public ClipperGroup
{
public:
	std::string getShaderCodeFuncName() const override;
	std::string getShaderCodeFuncImpl(const std::string &default_src_arg) const override;
	bool isValid(const glm::vec3 &point) const override;
};
}}

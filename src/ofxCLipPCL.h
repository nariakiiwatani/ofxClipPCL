#pragma once

#include "ofMesh.h"

namespace ofx { namespace clippcl {
class Clipper
{
public:
	// for clipping by CPU
	virtual bool isValid(const glm::vec3 &point) const=0;
	ofMesh getValid(const ofMesh &src) const;
	void reduce(ofMesh &srcdst) const { reduce(srcdst, srcdst); }
	void reduce(const ofMesh &src, ofMesh &dst) const { dst = getValid(src); }

	// clipping by GPU(vertex shader program)
	virtual std::string getShaderCodeFuncName() const { return "ofxClipPCLFuncShape"; }
	virtual std::vector<std::string> getArgsForShaderFuncDeclare(const std::string &src_arg) const {
		return {src_arg};
	}
	virtual std::string getShaderCodeFuncImpl(const std::string &default_src_arg) const {
		return "return true;";
	}
	virtual std::vector<std::string> getArgsForShaderFunc(const std::string &src_arg) const {
		return {src_arg};
	}
};
class ClipperGroup : public Clipper
{
public:
	std::vector<std::string> getArgsForShaderFuncDeclare(const std::string &src_arg) const override;
	std::vector<std::string> getArgsForShaderFunc(const std::string &src_arg) const override;
	template<typename T, typename ...Args>
	std::shared_ptr<T> addClipper(Args &&...args) {
		auto clipper = std::make_shared<T>(std::forward<Args>(args)...);
		clippers_.push_back(clipper);
		return clipper;
	}
	std::vector<std::shared_ptr<Clipper>> getClippers() { return clippers_; }
protected:
	std::vector<std::shared_ptr<Clipper>> clippers_;
};
class ClipperGroupAll : public ClipperGroup
{
public:
	std::string getShaderCodeFuncName() const override;
	std::string getShaderCodeFuncImpl(const std::string &default_src_arg) const override;
	bool isValid(const glm::vec3 &point) const override;
};
class ClipperGroupAny : public ClipperGroup
{
public:
	std::string getShaderCodeFuncName() const override;
	std::string getShaderCodeFuncImpl(const std::string &default_src_arg) const override;
	bool isValid(const glm::vec3 &point) const override;
};
}}

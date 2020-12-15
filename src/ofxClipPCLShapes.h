#pragma once

#include "ofMesh.h"

namespace ofx { namespace clippcl {
class Shape
{
public:
	virtual bool isValid(const glm::vec3 &point) const=0;
	ofMesh getValid(const ofMesh &src) const;
	void reduce(ofMesh &srcdst) const { reduce(srcdst, srcdst); }
	void reduce(const ofMesh &src, ofMesh &dst) const { dst = getValid(src); }
};
class ShapeGroup : public Shape
{
public:
	template<typename T, typename ...Args>
	std::shared_ptr<T> addShape(Args &&...args) {
		auto shape = std::make_shared<T>(std::forward<Args>(args)...);
		shapes_.push_back(shape);
		return shape;
	}
protected:
	std::vector<std::shared_ptr<Shape>> shapes_;
};
class ShapeGroupAll : public ShapeGroup
{
public:
	bool isValid(const glm::vec3 &point) const override {
		using namespace std;
		return all_of(begin(shapes_), end(shapes_), [point](shared_ptr<Shape> shape) {
			return shape->isValid(point);
		});
	}
};
class ShapeGroupAny : public ShapeGroup
{
public:
	bool isValid(const glm::vec3 &point) const override {
		using namespace std;
		return any_of(begin(shapes_), end(shapes_), [point](shared_ptr<Shape> shape) {
			return shape->isValid(point);
		});
	}
};
class Plane : public Shape
{
public:
	Plane(const glm::vec3 &normal, float distance)
	:normal_(glm::normalize(normal))
	,distance_(distance)
	{}
	bool isValid(const glm::vec3 &point) const override {
		return glm::dot(point, normal_) > distance_;
	}
private:
	glm::vec3 normal_ = glm::vec3(0,1,0);
	float distance_ = 0;
};
}}

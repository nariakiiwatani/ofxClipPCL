#pragma once

#define OFX_CLIPPCL_ACCEPTOR_FUNCTIONS \
virtual inline void accept(Visitor &v) { \
	v.visit(*this); \
} \
virtual inline void accept(ConstVisitor &v) const { \
	v.visit(*this); \
}
#define OFX_CLIPPCL_ACCEPTOR_FUNCTIONS_OVERRIDE \
virtual inline void accept(Visitor &v) override { \
	v.visit(*this); \
} \
virtual inline void accept(ConstVisitor &v) const override { \
	v.visit(*this); \
}

namespace ofx { namespace clippcl {

class Clipper;
class ClipperGroup;
class ClipperGroupAll;
class ClipperGroupAny;
class Geometry;
class GeometryByMatrix;
class Plane;
class Box;
class Sphere;
class Cylinder;
class Cone;
class Visitor
{
public:
	virtual void visit(Clipper&);
	virtual void visit(ClipperGroup&);
	virtual void visit(ClipperGroupAll&);
	virtual void visit(ClipperGroupAny&);
	virtual void visit(Geometry&);
	virtual void visit(GeometryByMatrix&);
	virtual void visit(Plane&);
	virtual void visit(Box&);
	virtual void visit(Sphere&);
	virtual void visit(Cylinder&);
	virtual void visit(Cone&);
};
class ConstVisitor
{
public:
	virtual void visit(const Clipper&);
	virtual void visit(const ClipperGroup&);
	virtual void visit(const ClipperGroupAll&);
	virtual void visit(const ClipperGroupAny&);
	virtual void visit(const Geometry&);
	virtual void visit(const GeometryByMatrix&);
	virtual void visit(const Plane&);
	virtual void visit(const Box&);
	virtual void visit(const Sphere&);
	virtual void visit(const Cylinder&);
	virtual void visit(const Cone&);
};

}}

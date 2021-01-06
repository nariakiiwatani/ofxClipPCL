#pragma once

#define OFX_CLIPPCL_ACCEPTOR_FUNCTIONS \
virtual inline void accept(Visitor &v) { \
	v.visit(*this); \
} \
virtual inline void accept(Visitor &v) const { \
	v.visit(*this); \
}
#define OFX_CLIPPCL_ACCEPTOR_FUNCTIONS_OVERRIDE \
virtual inline void accept(Visitor &v) override { \
	v.visit(*this); \
} \
virtual inline void accept(Visitor &v) const override { \
	v.visit(*this); \
}

namespace ofx { namespace clippcl {

class Clipper;
class ClipperGroup;
class ClipperGroupAll;
class ClipperGroupAny;
class Geometry;
class Plane;
class Visitor
{
public:
#define DECLARE_VISIT_FUNCS(Type) \
	virtual void visit(Type&); \
	virtual void visit(const Type&);
	DECLARE_VISIT_FUNCS(Clipper)
	DECLARE_VISIT_FUNCS(ClipperGroup)
	DECLARE_VISIT_FUNCS(ClipperGroupAll)
	DECLARE_VISIT_FUNCS(ClipperGroupAny)
	DECLARE_VISIT_FUNCS(Geometry)
	DECLARE_VISIT_FUNCS(Plane)
#undef DECLARE_VISIT_FUNCS
};

}}

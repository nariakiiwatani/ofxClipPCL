#include "ofxClipPCLVisitor.h"
#include "ofxClipPCL.h"
#include "ofxClipPCLGeometry.h"

using namespace ofx::clippcl;

void Visitor::visit(Clipper &value) {
}
void ConstVisitor::visit(const Clipper &value) {
}

#define IMPL_VISIT_FUNCS_SUB(Type, Super) \
void Visitor::visit(Type &value) { visit(static_cast<Super&>(value)); }
IMPL_VISIT_FUNCS_SUB(ClipperGroup, Clipper)
IMPL_VISIT_FUNCS_SUB(ClipperGroupAll, ClipperGroup)
IMPL_VISIT_FUNCS_SUB(ClipperGroupAny, ClipperGroup)
IMPL_VISIT_FUNCS_SUB(Geometry, Clipper)
IMPL_VISIT_FUNCS_SUB(Plane, Geometry)
IMPL_VISIT_FUNCS_SUB(Box, Geometry)
IMPL_VISIT_FUNCS_SUB(Sphere, Geometry)
#undef IMPL_VISIT_FUNCS_SUB

#define IMPL_VISIT_FUNCS_SUB(Type, Super) \
void ConstVisitor::visit(const Type &value) { visit(static_cast<const Super&>(value)); }
IMPL_VISIT_FUNCS_SUB(ClipperGroup, Clipper)
IMPL_VISIT_FUNCS_SUB(ClipperGroupAll, ClipperGroup)
IMPL_VISIT_FUNCS_SUB(ClipperGroupAny, ClipperGroup)
IMPL_VISIT_FUNCS_SUB(Geometry, Clipper)
IMPL_VISIT_FUNCS_SUB(Plane, Geometry)
IMPL_VISIT_FUNCS_SUB(Box, Geometry)
IMPL_VISIT_FUNCS_SUB(Sphere, Geometry)
#undef IMPL_VISIT_FUNCS_SUB


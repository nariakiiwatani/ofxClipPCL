#include "ofxClipPCLVisitor.h"
#include "ofxClipPCL.h"
#include "ofxClipPCLGeometry.h"

using namespace ofx::clippcl;

void Visitor::visit(const Clipper &value) {
}
void Visitor::visit(Clipper &value) {
}

#define IMPL_VISIT_FUNCS_SUB(Type, Super) \
void Visitor::visit(const Type &value) { visit(static_cast<const Super&>(value)); } \
void Visitor::visit(Type &value) { visit(static_cast<Super&>(value)); }
IMPL_VISIT_FUNCS_SUB(ClipperGroup, Clipper)
IMPL_VISIT_FUNCS_SUB(ClipperGroupAll, ClipperGroup)
IMPL_VISIT_FUNCS_SUB(ClipperGroupAny, ClipperGroup)
IMPL_VISIT_FUNCS_SUB(Geometry, Clipper)
IMPL_VISIT_FUNCS_SUB(Plane, Geometry)
IMPL_VISIT_FUNCS_SUB(Box, Geometry)
#undef IMPL_VISIT_FUNCS_SUB


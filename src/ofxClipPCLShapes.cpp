#include "ofxClipPCLShapes.h"

using namespace ofx::clippcl;
using namespace std;

ofMesh Shape::getValid(const ofMesh &src) const
{
	vector<ofIndexType> use;
	auto &verts = src.getVertices();
	use.reserve(verts.size());
	for(ofIndexType i = 0; i < verts.size(); ++i) {
		if(isValid(verts[i])) use.push_back(i);
	}
	ofMesh ret;
	ret.getVertices().resize(use.size());
	if(src.hasColors()) ret.getColors().resize(use.size());
	if(src.hasNormals()) ret.getNormals().resize(use.size());
	if(src.hasTexCoords()) ret.getTexCoords().resize(use.size());

	for(ofIndexType i = 0; i < use.size(); ++i) {
		ret.setVertex(i, src.getVertex(use[i]));
		if(src.hasColors()) ret.setColor(i, src.getColor(use[i]));
		if(src.hasNormals()) ret.setNormal(i, src.getNormal(use[i]));
		if(src.hasTexCoords()) ret.setTexCoord(i, src.getTexCoord(use[i]));
	}
	return ret;
}

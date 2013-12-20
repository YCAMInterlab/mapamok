#pragma once

void getBoundingBox(const ofMesh& mesh, ofVec3f& cornerMin, ofVec3f& cornerMax) {
	const vector<ofVec3f>& vertices = mesh.getVertices();
	if(vertices.size() > 0) {
		cornerMin = vertices[0];
		cornerMax = vertices[0];
	}
	for(int i = 0; i < vertices.size(); i++) {
		cornerMin.x = MIN(cornerMin.x, vertices[i].x);
		cornerMin.y = MIN(cornerMin.y, vertices[i].y);
		cornerMin.z = MIN(cornerMin.z, vertices[i].z);
		cornerMax.x = MAX(cornerMax.x, vertices[i].x);
		cornerMax.y = MAX(cornerMax.y, vertices[i].y);
		cornerMax.z = MAX(cornerMax.z, vertices[i].z);
	}
}

void centerAndNormalize(ofMesh& mesh) {
	ofVec3f cornerMin, cornerMax;
	getBoundingBox(mesh, cornerMin, cornerMax);
	ofVec3f translate = -(cornerMax + cornerMin) / 2;
	ofVec3f range = (cornerMax - cornerMin);
	float maxRange = 0;
	maxRange = MAX(maxRange, range.x);
	maxRange = MAX(maxRange, range.y);
	maxRange = MAX(maxRange, range.z);
	float scale = 1 / maxRange;
	vector<ofVec3f>& vertices = mesh.getVertices();
	for(int i = 0; i < vertices.size(); i++) {
		vertices[i] += translate;
		vertices[i] *= scale;
	}
}
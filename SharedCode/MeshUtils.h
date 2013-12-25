#pragma once

class Edge {
public:
	unsigned int i0, i1;
	Edge(unsigned int i0, unsigned int i1)
	:i0(i0)
	,i1(i1) {
	}
	unsigned int getFirst() const {
		return MIN(i0, i1);
	}
	unsigned int getSecond() const {
		return MAX(i0, i1);
	}
	bool operator<(const Edge& edge) const {
		int a0 = getFirst();
		int b0 = edge.getFirst();
		if(a0 == b0) {
			int a1 = getSecond();
			int b1 = edge.getSecond();
			return a1 < b1;
		}
		return a0 < b0;
	}
};

class Face {
public:
	unsigned int i0, i1, i2;
	Face(unsigned int i0, unsigned int i1, unsigned int i2)
	:i0(i0)
	,i1(i1)
	,i2(i2) {
	}
	Edge getEdge(unsigned int i) {
		switch(i) {
			case 0: return Edge(i0, i1);
			case 1: return Edge(i1, i2);
			case 2: return Edge(i2, i0);
		}
	}
	unsigned int getFirst() const {
		if(i0 < i1 && i0 < i2) return i0;
		if(i1 < i0 && i1 < i2) return i1;
		else return i2;
	}
	unsigned int getSecond() const {
		if(i0 < i1 && i0 > i2) return i0;
		if(i0 > i1 && i0 < i2) return i0;
		if(i1 < i0 && i1 > i2) return i1;
		if(i1 > i0 && i1 < i2) return i1;
		else return i2;
	}
	unsigned int getThird() const {
		if(i0 > i1 && i0 > i2) return i0;
		if(i1 > i0 && i1 > i2) return i1;
		else return i2;
	}
	bool operator<(const Face& face) const {
		// might be faster by just sorting vertices before comparison
		int a0 = getFirst();
		int b0 = face.getFirst();
		if(a0 == b0) {
			int a1 = getSecond();
			int b1 = face.getSecond();
			if(a1 == b1) {
				int a2 = getThird();
				int b2 = face.getThird();
				return a2 < b2;
			}
			return a1 < b1;
		}
		return a0 < b0;
	}
};

ofVec3f getVector(const ofMesh& mesh, int from, int to) {
	return mesh.getVertex(to) - mesh.getVertex(from);
}

ofVec3f getEdgeVector(const ofMesh& mesh, const Edge& edge) {
	return getVector(mesh, edge.i0, edge.i1);
}

float getMaximumAngle(const ofMesh& mesh, const vector<Edge>& edges) {
	float maximumAngle = 0;
	for(int i = 0; i < edges.size(); i++) {
		int i0 = edges[i].i0, i1 = edges[i].i1;
		ofVec3f a = getVector(mesh, i0, i1);
		for(int j = 0; j < edges.size(); j++) {
			if(i != j) {
				int j0 = edges[j].i0, j1 = edges[j].i1;
				ofVec3f b;
				if(i0 == j0) {
					b = getVector(mesh, j0, j1);
				} else {
					b = getVector(mesh, j1, j0);
				}
				float curAngle = a.angle(b);
				if(curAngle > maximumAngle) {
					maximumAngle = curAngle;
				}
			}
		}
	}
	return maximumAngle;
}

vector<Edge> getAllEdges(const ofMesh& mesh) {
	set<Edge> uniqueEdges;
	int n = mesh.getNumIndices();
	for(int i = 0; i < n;) {
		int i0 = mesh.getIndex(i++);
		int i1 = mesh.getIndex(i++);
		int i2 = mesh.getIndex(i++);
		uniqueEdges.insert(Edge(i0, i1));
		uniqueEdges.insert(Edge(i1, i2));
		uniqueEdges.insert(Edge(i2, i0));
	}
	vector<Edge> edges;
	edges.assign(uniqueEdges.begin(), uniqueEdges.end());
	return edges;
}

vector< vector<Edge> > getAllEdgesForVertices(const ofMesh& mesh) {
	vector<Edge> allEdges = getAllEdges(mesh);
	vector< vector<Edge> > edgesForVertices(mesh.getNumVertices());
	for(int i = 0; i < allEdges.size(); i++) {
		const Edge& curEdge = allEdges[i];
		edgesForVertices[curEdge.i0].push_back(curEdge);
		edgesForVertices[curEdge.i1].push_back(curEdge);
	}
	return edgesForVertices;
}

vector<Edge> getEdgesFromVertex(const ofMesh& mesh, unsigned int index) {
	set<Edge> uniqueEdges;
	int n = mesh.getNumIndices();
	for(int i = 0; i < n;) {
		int i0 = mesh.getIndex(i++);
		int i1 = mesh.getIndex(i++);
		int i2 = mesh.getIndex(i++);
		if(i0 == index) {
			uniqueEdges.insert(Edge(i0, i1));
			uniqueEdges.insert(Edge(i0, i2));
		}
		if(i1 == index) {
			uniqueEdges.insert(Edge(i1, i0));
			uniqueEdges.insert(Edge(i1, i2));
		}
		if(i2 == index) {
			uniqueEdges.insert(Edge(i2, i0));
			uniqueEdges.insert(Edge(i2, i1));
		}
	}
	vector<Edge> edges;
	edges.assign(uniqueEdges.begin(), uniqueEdges.end());
	return edges;
}

// needs testing
vector<Face> getFacesFromVertex(const ofMesh& mesh, unsigned int index) {
	set<Face> uniqueFaces;
	int n = mesh.getNumIndices();
	for(int i = 0; i < n;) {
		int i0 = mesh.getIndex(i++);
		int i1 = mesh.getIndex(i++);
		int i2 = mesh.getIndex(i++);
		uniqueFaces.insert(Face(i0, i1, i2));
	}
	vector<Face> faces;
	faces.assign(uniqueFaces.begin(), uniqueFaces.end());
	return faces;
}

vector<Face> getFacesFromEdge(const ofMesh& mesh, const Edge& edge) {
	vector<Face> faces;
	// todo
	return faces;
}

vector<unsigned int> getCornerVertices(const ofMesh& mesh, float maximumAngle = 180) {
	vector<unsigned int> corners;
	vector< vector<Edge> > edgesForVertices = getAllEdgesForVertices(mesh);
	for(int i = 0; i < edgesForVertices.size(); i++) {
		const vector<Edge>& edges = edgesForVertices[i];
		cout << getMaximumAngle(mesh, edges) << endl;
		if(getMaximumAngle(mesh, edges) < maximumAngle) {
			corners.push_back(i);
		}
	}
	return corners;
}

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

void project(ofMesh& mesh, const ofCamera& camera, ofRectangle viewport) {
	ofMatrix4x4 modelViewProjectionMatrix = camera.getModelViewProjectionMatrix(viewport);
	viewport.width /= 2;
	viewport.height /= 2;
	for(int i = 0; i < mesh.getNumVertices(); i++) {
		ofVec3f& cur = mesh.getVerticesPointer()[i];
		ofVec3f CameraXYZ = cur * modelViewProjectionMatrix;
		cur.x = (CameraXYZ.x + 1.0f) * viewport.width + viewport.x;
		cur.y = (1.0f - CameraXYZ.y) * viewport.height + viewport.y;
		cur.z = CameraXYZ.z / 2;
	}
}

void drawNormals(const ofMesh& mesh, float normalLength) {
	for(int i = 0; i < mesh.getNumNormals(); i++) {
		const ofVec3f& start = mesh.getVertices()[i];
		const ofVec3f& normal = mesh.getNormals()[i];
		ofVec3f end = start + normal * normalLength;
		ofLine(start, end);
	}
}

ofMesh collapseModel(ofxAssimpModelLoader model) {
	ofMesh mesh;
	//	cout << "collapsing " << model.getNumMeshes() << endl;
	for(int i = 0; i < model.getNumMeshes(); i++) {
		ofMesh curMesh = model.getMesh(i);
		mesh.append(curMesh);
	}
	return mesh;
}

void prepareRender(bool useDepthTesting, bool useBackFaceCulling, bool useFrontFaceCulling) {
	ofSetDepthTest(useDepthTesting);
	if(useBackFaceCulling || useFrontFaceCulling) {
		glEnable(GL_CULL_FACE);
		if(useBackFaceCulling && useFrontFaceCulling) {
			glCullFace(GL_FRONT_AND_BACK);
		} else if(useBackFaceCulling) {
			glCullFace(GL_BACK);
		} else if(useFrontFaceCulling) {
			glCullFace(GL_FRONT);
		}
	} else {
		glDisable(GL_CULL_FACE);
	}
}
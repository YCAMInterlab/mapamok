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

float getAngle(const ofMesh& mesh, int a, int center, int b) {
	ofVec3f v0 = getVector(mesh, center, a);
	ofVec3f v1 = getVector(mesh, center, b);
    if(a == center || a == b || center == b) {
        return 0; // avoid NaN
    }
    return v0.angle(v1);
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

vector<Face> getAllFaces(const ofMesh& mesh) {
	vector<Face> faces;
	int n = mesh.getNumIndices();
	for(int i = 0; i < n;) {
		int i0 = mesh.getIndex(i++);
		int i1 = mesh.getIndex(i++);
		int i2 = mesh.getIndex(i++);
        // ignore faces that are just lines
        if(i0 != i1 && i0 != i2 && i1 != i2) {
            faces.push_back(Face(i0, i1, i2));
        }
	}
	return faces;
}

template <class T>
vector<unsigned int> getSortedIndices(const vector<T>& v) {
    int n = v.size();
	vector< pair<T, unsigned int> > sorted(n);
	for(unsigned int i = 0; i < n; i++) {
		sorted[i].first = v[i];
		sorted[i].second = i;
        if(v[i] != v[i]) {
            ofLogError() << "potential memory corruption from sorting NaN";
        }
	}
	ofSort(sorted);
	vector<unsigned int> indices(n);
	for(unsigned int i = 0; i < n; i++) {
		indices[i] = sorted[i].second;
	}
	return indices;
}

vector<float> getAngleSums(const ofMesh& mesh) {
	vector<float> angleSums(mesh.getNumVertices());
	vector<Face> faces = getAllFaces(mesh);
	for(int i = 0; i < faces.size(); i++) {
		Face& face = faces[i];
		angleSums[face.i0] += getAngle(mesh, face.i2, face.i0, face.i1);
		angleSums[face.i1] += getAngle(mesh, face.i0, face.i1, face.i2);
		angleSums[face.i2] += getAngle(mesh, face.i1, face.i2, face.i0);
	}
	return angleSums;
}

vector<unsigned int> getRankedCorners(const ofMesh& mesh) {
	vector<float> angleSums = getAngleSums(mesh);
	for(int i = 0; i < angleSums.size(); i++) {
        // angleSums should be 360 for flat surfaces
        // clear corners / end points have less than 360
        // wrinkly saddles have more than 360
        // this transforms both to being 0 for flat
        // and negative / small for non-flat
        float before = angleSums[i];
		angleSums[i] = -fabsf(angleSums[i] - 360);
	}
	return getSortedIndices(angleSums);
}

int findNearestVertex(const vector<ofVec3f>& vertices, const ofVec3f& base) {
    int nearestIndex = 0;
    float nearestDistance = 0;
    int n = vertices.size();
    for(int i = 0; i < n; i++) {
        float distance = base.squareDistance(vertices[i]);
        if(i == 0 || distance < nearestDistance) {
            nearestDistance = distance;
            nearestIndex = i;
        }
    }
    return nearestIndex;
}

class PercentStatus {
public:
    int i, total, ticks;
    int lastReport;
    PercentStatus(int total, int ticks = 10)
    :total(total)
    ,ticks(ticks)
    ,lastReport(0) {
    }
    bool update(int i) {
        this->i = i;
        if(i > lastReport + (total / ticks)) {
            lastReport = i;
            return true;
        }
        return false;
    }
    void updateAndLog(int i) {
        if(update(i)) {
            ofLog() << getPercentage() << "%";
        }
    }
    int getPercentage() {
        return (100 * i) / total;
    }
};

// assumes mesh is indexed
// drops all normals, colors, and tex coords
ofMesh mergeNearbyVertices(const ofMesh& mesh, float tolerance = 0) {
    if(tolerance == 0) {
        return mesh;
    }
    float squareTolerance = tolerance * tolerance;
    ofMesh mergedMesh;
    int n = mesh.getNumVertices();
    vector<int> remappedIndices;
    for(int i = 0; i < n; i++) {
        const ofVec3f& cur = mesh.getVertices()[i];
        if(mergedMesh.getNumVertices() > 0) {
            int nearestIndex = findNearestVertex(mergedMesh.getVertices(), cur);
            const ofVec3f& nearestVertex = mergedMesh.getVertices()[nearestIndex];
            if(cur.squareDistance(nearestVertex) < squareTolerance) {
                remappedIndices.push_back(nearestIndex);
            } else {
                remappedIndices.push_back(mergedMesh.getNumVertices());
                mergedMesh.addVertex(cur);
            }
        } else {
            remappedIndices.push_back(0);
            mergedMesh.addVertex(cur);
        }
    }
    n = mesh.getNumIndices();
    for(int i = 0; i < n; i++) {
        mergedMesh.addIndex(remappedIndices[mesh.getIndex(i)]);
    }
    return mergedMesh;
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

void centerAndNormalize(ofMesh& mesh, ofVec3f cornerMin, ofVec3f cornerMax) {
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

void centerAndNormalize(ofMesh& mesh) {
	ofVec3f cornerMin, cornerMax;
	getBoundingBox(mesh, cornerMin, cornerMax);
    centerAndNormalize(mesh, cornerMin, cornerMax);
}

ofVec3f randomVec3f(float range) {
    return ofVec3f(ofRandom(-range, range), ofRandom(-range, range), ofRandom(-range, range));
}

void addJitter(ofMesh& mesh, float range) {
	vector<ofVec3f>& vertices = mesh.getVertices();
    int n = vertices.size();
    for(int i = 0; i < n; i++) {
        vertices[i] += randomVec3f(range);
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

vector<ofMesh> getMeshes(ofxAssimpModelLoader& model) {
    vector<ofMesh> meshes;
    for(int i = 0; i < model.getNumMeshes(); i++) {
        meshes.push_back(model.getMesh(i));
    }
    return meshes;
}

ofMesh joinMeshes(vector<ofMesh>& meshes) {
	ofMesh mesh;
	for(int i = 0; i < meshes.size(); i++) {
		mesh.append(meshes[i]);
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


ofVec3f getNormal(const ofVec3f& v1, const ofVec3f& v2, const ofVec3f& v3) {
	ofVec3f a = v1 - v2;
	ofVec3f b = v3 - v2;
	ofVec3f normal = b.cross(a);
	normal.normalize();
	return normal;
}

ofMesh convertFromIndices(const ofMesh& mesh) {
	ofMesh result;
	// have to do a const_cast because ofMesh::get*() is not const correct
	ofMesh& cmesh = const_cast<ofMesh&>(mesh);
	int vertices = mesh.getNumVertices();
	int colors = mesh.getNumColors();
	int normals = mesh.getNumNormals();
	int texcoords = mesh.getNumTexCoords();
	int indices = mesh.getNumIndices();
	for(int i = 0; i < indices; i++) {
		int cur = cmesh.getIndex(i);
		if(vertices > 0) {
			result.addVertex(cmesh.getVertex(cur));
		}
		if(colors > 0) {
			result.addColor(cmesh.getColor(cur));
		}
		if(normals > 0) {
			result.addNormal(cmesh.getNormal(cur));
		}
		if(texcoords > 0) {
			result.addTexCoord(cmesh.getTexCoord(cur));
		}
	}
	return result;
}

void buildNormalsFaces(ofMesh& mesh) {
	for(int i = 0; i < mesh.getNumVertices(); i += 3) {
		int i0 = i + 0, i1 = i + 1, i2 = i + 2;
		ofVec3f normal = getNormal(mesh.getVertices()[i0], mesh.getVertices()[i1], mesh.getVertices()[i2]);
		for(int j = 0; j < 3; j++) {
			mesh.addNormal(normal);
		}
	}
}

// assumes indexed vertices and triangles
void buildNormalsSingle(ofMesh& mesh) {
	vector<ofIndexType>& indices = mesh.getIndices();
	vector<bool> ready(mesh.getNumVertices());
	vector<ofVec3f> normals(mesh.getNumVertices());
	for(int i = 0; i < indices.size(); i += 3) {
		int i0 = indices[i + 0], i1 = indices[i + 1], i2 = indices[i + 2];
		ofVec3f normal = getNormal(mesh.getVertices()[i0], mesh.getVertices()[i1], mesh.getVertices()[i2]);
		if(!ready[i0]) {
			normals[i0] = normal;
			ready[i0] = true;
		}
		if(!ready[i1]) {
			normals[i1] = normal;
			ready[i1] = true;
		}
		if(!ready[i2]) {
			normals[i2] = normal;
			ready[i2] = true;
		}
	}
	mesh.addNormals(normals);
}

// assumes indexed vertices and triangles
void buildNormalsAverage(ofMesh& mesh) {
	vector<ofIndexType>& indices = mesh.getIndices();
	vector<ofVec3f> normals(mesh.getNumVertices());
	for(int i = 0; i < indices.size(); i += 3) {
		int i0 = indices[i + 0], i1 = indices[i + 1], i2 = indices[i + 2];
		ofVec3f normal = getNormal(mesh.getVertices()[i0], mesh.getVertices()[i1], mesh.getVertices()[i2]);
		normals[i0] += normal;
		normals[i1] += normal;
		normals[i2] += normal;
	}
	for(int i = 0; i < normals.size(); i++) {
		normals[i].normalize();
	}
	mesh.addNormals(normals);
}

// need to check that this actually works
class IndexedPoint {
public:
    const ofVec3f* vertex;
    const ofFloatColor* color;
    const ofVec3f* normal;
    const ofVec2f* texCoord;
    
    IndexedPoint(const ofMesh& mesh, int i) {
        vertex = mesh.getNumVertices() > 0 ? &(mesh.getVerticesPointer()[i]) : NULL;
        color = mesh.getNumColors() > 0 ? &(mesh.getColorsPointer()[i]) : NULL;
        normal = mesh.getNumNormals() > 0 ? &(mesh.getNormalsPointer()[i]) : NULL;
        texCoord = mesh.getNumTexCoords() > 0 ? &(mesh.getTexCoordsPointer()[i]) : NULL;
    }
    // used for map operator[]
    bool operator<(const IndexedPoint& point) const {
        if(vertex->x < point.vertex->x) {
            return false;
        } else if(vertex->x > point.vertex->x) {
            return true;
        } else {
            if(vertex->y < point.vertex->y) {
                return false;
            } else if(vertex->y > point.vertex->y) {
                return true;
            } else {
                return false; // equal
            }
        }
    }
};

ofMesh convertToIndices(ofMesh& mesh) {
    ofMesh result;
    
    int vertices = mesh.getNumVertices(); // ofVec3f
    int colors = mesh.getNumColors(); // ofFloatColor
    int normals = mesh.getNumNormals(); // ofVec3f
    int texcoords = mesh.getNumTexCoords();	// ofVec2f
    
    bool usingColors = colors > 0;
    bool usingNormals = normals > 0;
    bool usingTexcoords = texcoords > 0;
    
    map<IndexedPoint, int> indexedMap;
    vector<vector<int> > shared;
    int index = 0;
    for(int i = 0; i < vertices; i++) {
        IndexedPoint cur(mesh, i);
        if(indexedMap.find(cur) == indexedMap.end()) {
            indexedMap[cur] = index;
            shared.push_back(vector<int>());
            index++;
        }
        int curIndex = indexedMap[cur];
        result.addIndex(curIndex);
        shared[curIndex].push_back(i);
    }
    
    for(int i = 0; i < shared.size(); i++) {
        vector<int>& cur = shared[i];
        
        ofVec3f vertex = mesh.getVerticesPointer()[cur[0]];
        ofFloatColor color(0, 0, 0, 0);
        ofVec3f normal(0, 0, 0);
        ofVec2f texCoord(0, 0);
        float normalize = 1. / cur.size();
        
        for(int j = 0; j < cur.size(); j++) {
            if(usingColors) {
                color += mesh.getColorsPointer()[cur[j]] * normalize;
            }
            if(usingNormals) {
                normal += mesh.getNormalsPointer()[cur[j]] * normalize;
            }
            if(usingTexcoords) {
                texCoord += mesh.getTexCoordsPointer()[cur[j]] * normalize;
            }
        }
        
        result.addVertex(vertex);
        if(usingColors) {
            result.addColor(color);
        }
        if(usingNormals) {
            result.addNormal(normal);
        }
        if(usingTexcoords) {
            result.addTexCoord(texCoord);
        }
    }
    
    return result;
}

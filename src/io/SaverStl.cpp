
//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2026-01-26 17:42:17 taubin>
//------------------------------------------------------------------------
//
// SaverStl.cpp
//
// Written by: Steven Defreitas
//
// Software developed for the course
// Digital Geometry Processing
// Copyright (c) 2026, Gabriel Taubin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Brown University nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL GABRIEL TAUBIN BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "SaverStl.hpp"

#include "wrl/Shape.hpp"
#include "wrl/Appearance.hpp"
#include "wrl/Material.hpp"
#include "wrl/IndexedFaceSet.hpp"

#include "core/Faces.hpp"

const char* SaverStl::_ext = "stl";

//////////////////////////////////////////////////////////////////////
bool SaverStl::save(const char* filename, SceneGraph& wrl) const {
    bool success = false;
    if(filename!=(char*)0) {

        // Check these conditions

        // 1) the SceneGraph should have a single child

        if(wrl.getNumberOfChildren() != 1){
            return false;
        }
        // 2) the child should be a Shape node
        Node* child = wrl.getChildren()[0];

        if(child->getType() != "Shape"){
            return false;
        }

        Shape* shape = (Shape*)child;
        // 3) the geometry of the Shape node should be an IndexedFaceSet node
        IndexedFaceSet* ifs = (IndexedFaceSet*)shape->getGeometry();

        if (ifs == nullptr || ifs->getType() != "IndexedFaceSet") {
            return false;
        }

        // - construct an instance of the Faces class from the IndexedFaceSet
        // - remember to delete it when you are done with it (if necessary)
        //   before returning

        int nVert = ifs->getCoord().size() / 3;
        vector<int> coordIdx = ifs->getCoordIndex();

        Faces* faces = new Faces(nVert, coordIdx);

        // 4) the IndexedFaceSet should be a triangle mesh
        if(!ifs->isTriangleMesh()) {
            delete faces;
            return false;
        }

        // 5) the IndexedFaceSet should have normals per face
        if(ifs->getNormalBinding() != IndexedFaceSet::PB_PER_FACE){
            delete faces;
            return false;
        }

        // if (all the conditions are satisfied) {

        FILE* fp = fopen(filename,"w");
        if(	fp!=(FILE*)0) {

            string objectName;

            // if set, use ifs->getName()
            // otherwise use filename,
            // but first remove directory and extension
            if(!ifs->getName().empty()){
                objectName = ifs->getName();
            } else {
                objectName = filename;

                size_t endSlash = objectName.find_last_of("/\\");
                if(endSlash != string::npos){
                    objectName = objectName.substr(endSlash + 1);
                }

                size_t endPeriod = objectName.find_last_of('.');
                if(endPeriod != string::npos){
                    objectName = objectName.substr(0, endPeriod);
                }
            }

            fprintf(fp, "solid %s\n", objectName.c_str());

            int numFaces = faces->getNumberOfFaces();

            for(int iF = 0; iF < numFaces; iF++){

                float nx = ifs->getNormal()[3 * iF + 0];
                float ny = ifs->getNormal()[3 * iF + 1];
                float nz = ifs->getNormal()[3 * iF + 2];

                fprintf(fp, "  facet normal %f %f %f\n", nx, ny, nz);
                fprintf(fp, "    outer loop\n");

                for(int j = 0; j < 3; j++){
                    int vertIdx = faces->getFaceVertex(iF, j);

                    float vx = ifs->getCoord()[3 * vertIdx + 0];
                    float vy = ifs->getCoord()[3 * vertIdx + 1];
                    float vz = ifs->getCoord()[3 * vertIdx + 2];

                    fprintf(fp, "      vertex %f %f %f\n", vx, vy, vz);
                }

                fprintf(fp, "    endloop\n");
                fprintf(fp, "  endfacet\n");
            }



            fclose(fp);
            delete faces;
            success = true;
        }

        // } endif (all the conditions are satisfied)

        }
    return success;
}

//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2026-01-26 17:42:17 taubin>
//------------------------------------------------------------------------
//
// Faces.cpp
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

#include <math.h>
#include "Faces.hpp"
  
Faces::Faces(const int nV, const vector<int>& coordIndex) {
    this->coordIndex = coordIndex;
    
    int count = 0;
    
    for(int idx : coordIndex) {
        if(idx >= 0){
            count++;
        }
    }
    
    if(count > nV){
        this->nV = count;
    } else {
        this->nV = nV;
    }
}

int Faces::getNumberOfVertices() const {
  return nV;
}

int Faces::getNumberOfFaces() const {
    int count = 0;
    
    for(int idx : coordIndex){
        if(idx == -1){
            count++;
        }
    }
    
    return count;
}

int Faces::getNumberOfCorners() const {
  return coordIndex.size();
}

int Faces::getFaceSize(const int iF) const {
    
    if(iF < 0){
        return 0; // return 0 for negative iF index values
    }
    
    // initialize count vars for the current face and corner count
    int currentFace = 0;
    int corners = 0;
    
    for(int idx : coordIndex){
        if(idx == -1){ // if you hit a face boundary
            if(iF == currentFace){ // and if we are at the right face index
                return corners; // return the count we have established
            }
            currentFace++; // otherwise increment to the next face
            corners = 0; // and reset corners to 0
        } else { // if we aren't at a face boundary
            if(iF == currentFace){ // and we're in the right index
                corners++; // increment the corner count
            }
        }
    }
    
    return 0; // return 0 if out of bounds
}

int Faces::getFaceFirstCorner(const int iF) const {
    if(iF < 0){ // if the input index is negative
        return -1; // return -1
    }
    
    // initialize face counter
    int currentFace = 0;
    
    for(int idx : coordIndex){
        if(currentFace == iF){ // if wer're in the correct face index
            return idx; // return the current value in coordIndex
            // this should trigger right after switching into the correct face
            // so it will return the first corner in the face
        }
        
        if(idx == -1){ // if we hit a face boundary
            currentFace++; // increment the face counter
        }
        
        // otherwise just move to the next element of coordIndex
    }
    
  return -1; // return -1 if we're out of bounds
}

int Faces::getFaceVertex(const int iF, const int j) const {
    if(iF < 0){ // if the input face index is negative
        return -1; // return -1
    }
    
    if(j < 0){ // if the input corner index is negative
        return -1; // return -1
    }
    
    // initialize face and index counters
    int currentFace = 0;
    int vertexInFace = 0;  // Track position within current face

    for(int idx : coordIndex){
        if(idx == -1){
            currentFace++;
            vertexInFace = 0;  // Reset for next face
        } else {
            if(currentFace == iF && vertexInFace == j){
                return idx;
            }
            vertexInFace++;
        }
    }

    return -1;
}

int Faces::getCornerFace(const int iC) const {
    if(iC < 0 || iC >= (int)coordIndex.size()){
        return -1;
    }
    
    if(coordIndex[iC] == -1){
        return -1;
    }
    
    int currentFace = 0;
    int idxCount = 0;
    
    for(int idx : coordIndex){
        if(idxCount == iC){
            return currentFace;
        }
        
        if(idx == -1){
            currentFace++;
        }
        
        idxCount++;
    }
    
    return -1;
}

int Faces::getNextCorner(const int iC) const {
    if(iC < 0 || iC >= (int)coordIndex.size()){
        return -1;
    }
    
    if(coordIndex[iC] == -1){
        return -1;
    }
    
    if(iC + 1 < (int)coordIndex.size() && coordIndex[iC + 1] != -1){
        return iC + 1;
    }
        
    int faceStart = iC;
    while(faceStart > 0 && coordIndex[faceStart - 1] != -1){
        faceStart--;
    }
        
    return faceStart;
    
}


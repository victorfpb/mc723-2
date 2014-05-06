#include "watcher.h"
#include <stdio.h>
#include <stdlib.h>


Watcher::Watcher() {
  started = false;
  hazardRaW = 0;
}

void Watcher::start(){
  if (started) return;
  rdVec = startVector(WATCHER_VEC_SIZE);
  rsVec = startVector(WATCHER_VEC_SIZE);
  rtVec = startVector(WATCHER_VEC_SIZE);
  started = true;
}

void Watcher::finish(){
  printf("\n\n");
  printf("RaW HAZARDS = %d\n", hazardRaW);
  printf("\n\n");
}

void Watcher::registerInstruction(int rd, int rs, int rt	) {
  pushToVector(rd, rdVec, WATCHER_VEC_SIZE);
  pushToVector(rs, rsVec, WATCHER_VEC_SIZE);
  pushToVector(rt, rtVec, WATCHER_VEC_SIZE);
  checkForHazard();
}

void Watcher::branchInstruction(int oldpc, int newpc, int intendedPC) {
}

void Watcher::checkForHazard() {

  // RaW HAZARD
  if (rdVec[1]!=0) {
     if (rdVec[1] == rsVec[0]) hazardRaW++;
     else if (rdVec[1] == rtVec[0]) hazardRaW++;
  }
}

void Watcher::pushToVector(int reg, int* vec, int size) {
  for (int i=(size-1); i>0; i--) {
  	vec[i-1] = vec[i];
  }
  vec[0];
}

int* Watcher::startVector(int size){
  int* vec = (int*)malloc(sizeof(int)*size);
  for (int i=0; i<size-2; i++) {
    vec[i] = 0;
  }
  return vec;
}

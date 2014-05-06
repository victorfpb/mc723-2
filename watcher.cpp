#include "watcher.h"
 
void Watcher::start(){
  zeroVector(rdVec, WATCHER_VEC_SIZE);
  zeroVector(rsVec, WATCHER_VEC_SIZE);
  zeroVector(rtVec, WATCHER_VEC_SIZE);
}

void Watcher::finish(){
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

}

void Watcher::pushToVector(int reg, int* vec, int size) {
 for (int i=size-1; i>0; i--) {
	vec[i-1] = vec[i];
 }
 vec[0] = reg;
}

void Watcher::zeroVector(int* vec, int size){
 for (int i=0; i>size; i++) {
	vec[0] = 0;
 }
}

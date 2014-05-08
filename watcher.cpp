#include "watcher.h"
#include <stdio.h>
#include <stdlib.h>


Watcher::Watcher() {
  started = false;
  hazardRaW5 = 0;
  hazardWaW5 = 0;
  hazardRaW7 = 0;
  hazardWaW7 = 0;
  controlHazard = 0;
  instructionCount = 0;
  actualPC = 0;
}

void Watcher::start(){
  if (started) return;
  writeVec = startVector(WATCHER_VEC_SIZE);
  read1Vec = startVector(WATCHER_VEC_SIZE);
  read2Vec = startVector(WATCHER_VEC_SIZE);
  started = true;
}

void Watcher::finish(){
  printf("\n\n");
  printf("Total instructions = %d\n", instructionCount);
  printf("Total cycles (5 stages, disregarding hazards)* = %d\n", instructionCount+4);
  printf("RaW HAZARDS (5 stages) = %d\n", hazardRaW5);
  printf("WaW HAZARDS (5 stages, disregarding everything)* = %d\n", hazardWaW5);
  printf("RaW HAZARDS (7 stages) = %d\n", hazardRaW7);
  printf("WaW HAZARDS (7 stages, disregarding everything)* = %d\n", hazardWaW7);
  printf("Control HAZARDS = %d\n", controlHazard);
  printf("\n\n");
}

void Watcher::registerInstruction(int write, int read1, int read2) {
  pushToVector(write, writeVec, WATCHER_VEC_SIZE);
  pushToVector(read1, read1Vec, WATCHER_VEC_SIZE);
  pushToVector(read2, read2Vec, WATCHER_VEC_SIZE);
  checkForHazard5();
  checkForHazard7();
  checkForControlHazard();
}

void Watcher::branchInstruction(int oldpc, int newpc, int intendedPC) {
}

void Watcher::checkForHazard5() {

  // Simple RaW HAZARD
  if (findInVec(writeVec, read1Vec[0], 1, 1)) hazardRaW5++;
  else if (findInVec(writeVec, read2Vec[0], 1, 1)) hazardRaW5++;

  // Simple WaW HAZARD
  if (findInVec(writeVec, writeVec[0], 1, 1)) hazardWaW5++;
}


void Watcher::checkForHazard7() {

  // Simple RaW HAZARD
  if (findInVec(writeVec, read1Vec[0], 1, 2)) hazardRaW7++;
  else if (findInVec(writeVec, read2Vec[0], 1, 2)) hazardRaW7++;

  // Simple WaW HAZARD
  if (findInVec(writeVec, writeVec[0], 1, 2)) hazardWaW7++;
}

void Watcher::checkForControlHazard() {

//  Simple Control Hazard
//  if (!findInVec(writeVec, read1Vec[0], 1, WATCHER_VEC_SIZE-1)) controlHazard++;
//  else if (!findInVec(writeVec, read2Vec[0], 1, WATCHER_VEC_SIZE-1)) controlHazard++;

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

bool Watcher::findInVec (int* vec, int val, int minDep, int maxDep) {
  if (val == 0) return false;
  for (int i = minDep; i<=maxDep; i++) {
     if (vec[i] == val) return true;
  }
  return false;
}

void Watcher::anyInstrucion(int pc) {
  actualPC = pc;
  instructionCount++;
}

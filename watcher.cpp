#include "watcher.h"
#include <stdio.h>
#include <stdlib.h>


Watcher::Watcher() {
  started = false;

  hazardRaW5 = 0;
  hazardWaW5 = 0;
  hazardLU5 = 0;

  hazardRaW7 = 0;
  hazardRaW7x2 = 0;
  hazardWaW7 = 0;

  hazardRaW13x3 = 0;
  hazardRaW13x2 = 0;
  hazardRaW13 = 0;

  controlHazard = 0;
  instructionCount = 0;
  actualPC = 0;
}

void Watcher::start(){
  if (started) return;
  writeVec = startVector(WATCHER_VEC_SIZE);
  read1Vec = startVector(WATCHER_VEC_SIZE);
  read2Vec = startVector(WATCHER_VEC_SIZE);
  loadVec = startVector(WATCHER_VEC_SIZE);
  started = true;
}

void Watcher::finish(){
  printf("\n\n");
  printf("Total instructions = %d\n", instructionCount);

  printf("5 stage pipeline:\n");
  printf("\tLoad-Use hazards add %d cycles\n", hazardLU5);

  printf("\n");
  printf("7 stage pipeline:\n");
  printf("\tLoad-Use hazards add %d cycles\n", hazardLU7+(2*hazardLU7x2));

  printf("\n");
  printf("13 stage pipeline:\n");
  printf("\tLoad-Use\\RaW hazards add %d cycles\n", hazardRaW13+(2*hazardRaW13x2)+(3*hazardRaW13x3));

  printf("\n\n");
  free(writeVec);
  free(read1Vec);
  free(read2Vec);
  free(loadVec);
}

void Watcher::updateRegs(int rd, int rs, int rt, ins_types type) {
  //printf("RD = %d, RS = %d, RT = %d",rd,rs,rt);
  if (type == TIPO_R) {
    registerInstruction(rd, rs, rt, 0);
  } else if (type == TIPO_I) {
    registerInstruction(rt, rs, 0, 0);
  } else if (type == TIPO_J) {
    registerInstruction(0, 0, 0, 0);
  } else if (type == TIPO_LW) {
    registerInstruction(rt, rs, 0, rt);
  }
}

void Watcher::registerInstruction(int write, int read1, int read2, int load) {
  pushToVector(write, writeVec, WATCHER_VEC_SIZE);
  pushToVector(read1, read1Vec, WATCHER_VEC_SIZE);
  pushToVector(read2, read2Vec, WATCHER_VEC_SIZE);
  pushToVector(load, loadVec, WATCHER_VEC_SIZE);
  checkForHazard5();
  checkForHazard7();
  checkForHazard13();
  checkForControlHazard();
}

void Watcher::branchInstruction(int oldpc, int newpc, int intendedPC) {
}

void Watcher::checkForHazard5() {

  // Simple RaW HAZARD
  if (findInVec(writeVec, read1Vec[0], 1, 1)) hazardRaW5++;
  else if (findInVec(writeVec, read2Vec[0], 1, 1)) hazardRaW5++;

  // Simple LU HAZARD
  if (findInVec(loadVec, read1Vec[0], 1, 1)) hazardLU5++;
  else if (findInVec(loadVec, read2Vec[0], 1, 1)) hazardLU5++;

  // Simple WaW HAZARD
  if (findInVec(writeVec, writeVec[0], 1, 1)) hazardWaW5++;
}

void Watcher::checkForHazard7() {

  // Simple RaW HAZARD
  if (findInVec(writeVec, read1Vec[0], 1, 1)) hazardRaW7x2++;
  else if (findInVec(writeVec, read2Vec[0], 1, 1)) hazardRaW7x2++;
  else if (findInVec(writeVec, read1Vec[0], 2, 2)) hazardRaW7++;
  else if (findInVec(writeVec, read2Vec[0], 2, 2)) hazardRaW7++;

  // Simple LU HAZARD
  if (findInVec(loadVec, read1Vec[0], 1, 1)) hazardLU7x2++;
  else if (findInVec(loadVec, read2Vec[0], 1, 1)) hazardLU7x2++;
  else if (findInVec(loadVec, read1Vec[0], 2, 2)) hazardLU7++;
  else if (findInVec(loadVec, read2Vec[0], 2, 2)) hazardLU7++;

  // Simple WaW HAZARD
  if (findInVec(writeVec, writeVec[0], 1, 2)) hazardWaW7++;
}

void Watcher::checkForHazard13() {

  // Simple RaW HAZARD
  if (findInVec(writeVec, read1Vec[0], 1, 1)) hazardRaW13x3++;
  else if (findInVec(writeVec, read2Vec[0], 1, 1)) hazardRaW13x3++;
  else if (findInVec(writeVec, read1Vec[0], 2, 2)) hazardRaW13x2++;
  else if (findInVec(writeVec, read2Vec[0], 2, 2)) hazardRaW13x2++;
  else if (findInVec(writeVec, read1Vec[0], 3, 3)) hazardRaW13++;
  else if (findInVec(writeVec, read2Vec[0], 3, 3)) hazardRaW13++;
}

void Watcher::checkForControlHazard() {

//  Simple Control Hazard
//  if (!findInVec(writeVec, read1Vec[0], 1, WATCHER_VEC_SIZE-1)) controlHazard++;
//  else if (!findInVec(writeVec, read2Vec[0], 1, WATCHER_VEC_SIZE-1)) controlHazard++;

}

void Watcher::pushToVector(int reg, int* vec, int size) {
  for (int i=(size-1); i>0; i--) {
  	vec[i] = vec[i-1];
  }
  vec[0] = reg;
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
  if (instructionCount%10 == 11) {
    printVec(writeVec, WATCHER_VEC_SIZE);
    printVec(read1Vec, WATCHER_VEC_SIZE);
    printVec(read2Vec, WATCHER_VEC_SIZE);
    printVec(loadVec, WATCHER_VEC_SIZE);
  }
}

void Watcher::printVec(int* vec, int size) {
  for (int i = 0; i < size; i++) {
     printf("%2d ", vec[i]);
  }
  printf("\n");
}

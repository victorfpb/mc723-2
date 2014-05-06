#ifndef WATCHER_H
#define WATCHER_H

#define WATCHER_VEC_SIZE 13
 
class Watcher
{
private:
   int rdVec[WATCHER_VEC_SIZE];
   int rsVec[WATCHER_VEC_SIZE];
   int rtVec[WATCHER_VEC_SIZE];
 
public:
   Watcher() {}
void start();
void finish();
void registerInstruction(int rs, int rt, int rd);
void checkForHazard();
void branchInstruction(int oldpc, int newpc, int intendedPC);
void pushToVector(int reg, int* vec, int size);
void zeroVector(int* vec, int size);
};
 
#endif

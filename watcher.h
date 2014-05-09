#ifndef WATCHER_H
#define WATCHER_H

#define WATCHER_VEC_SIZE 13

class Watcher
{
  private:
    int* writeVec;
    int* read1Vec;
    int* read2Vec;

    int instructionCount;
    int actualPC;

    bool started;
    int hazardRaW5, hazardWaW5, hazardRaW7, hazardRaW7x2, hazardWaW7, controlHazard;

  public:
    Watcher();
    void start();
    void finish();
    void registerInstruction(int write, int read1, int read2);
    void checkForHazard5();
    void checkForHazard7();
    void checkForControlHazard();
    void branchInstruction(int oldpc, int newpc, int intendedPC);
    void pushToVector(int reg, int* vec, int size);
    int* startVector(int size);
    bool findInVec (int* vec, int val, int minDep, int maxDep);
    void anyInstrucion(int pc);
};

#endif

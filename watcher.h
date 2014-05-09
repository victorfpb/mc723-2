#ifndef WATCHER_H
#define WATCHER_H

#define WATCHER_VEC_SIZE 13

typedef enum enum_ins_types {
  TIPO_I,
  TIPO_R,
  TIPO_J,
  TIPO_LW
} ins_types;

class Watcher
{
  private:
    int* writeVec;
    int* loadVec;
    int* read1Vec;
    int* read2Vec;

    int instructionCount;
    int actualPC;

    bool started;
    int hazardRaW5, hazardWaW5, controlHazard;
    int hazardLU5, hazardLU7, hazardLU7x2, hazardRaW7, hazardRaW7x2, hazardWaW7;
    int hazardRaW13x3, hazardRaW13x2, hazardRaW13; 

    void registerInstruction(int write, int read1, int read2, int load);
    void checkForHazard5();
    void checkForHazard7();
    void checkForHazard13();
    void checkForControlHazard();
    void branchInstruction(int oldpc, int newpc, int intendedPC);
    void pushToVector(int reg, int* vec, int size);
    int* startVector(int size);
    bool findInVec (int* vec, int val, int minDep, int maxDep);
    void printVec(int* vec, int size);

  public:
    Watcher();
    void start();
    void finish();
    void updateRegs(int rd, int rs, int rt, ins_types type);


    void anyInstrucion(int pc);
};

#endif

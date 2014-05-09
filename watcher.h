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
    int hazardRaW5, hazardWaW5, hazardRaW7, hazardRaW7x2, hazardWaW7, controlHazard, hazardLU5;

    void registerInstruction(int write, int read1, int read2, int load);
    void checkForHazard5();
    void checkForHazard7();
    void checkForControlHazard();
    void branchInstruction(int oldpc, int newpc, int intendedPC);
    void pushToVector(int reg, int* vec, int size);
    int* startVector(int size);
    bool findInVec (int* vec, int val, int minDep, int maxDep);

  public:
    Watcher();
    void start();
    void finish();
    void updateRegs(int rd, int rs, int rt, ins_types type);


    void anyInstrucion(int pc);
};

#endif

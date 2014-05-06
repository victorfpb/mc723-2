#ifndef WATCHER_H
#define WATCHER_H

#define WATCHER_VEC_SIZE 13

class Watcher
{
  private:
    int* rdVec;
    int* rsVec;
    int* rtVec;

    bool started;
    int hazardRaW;

  public:
    Watcher();
    void start();
    void finish();
    void registerInstruction(int rs, int rt, int rd);
    void checkForHazard();
    void branchInstruction(int oldpc, int newpc, int intendedPC);
    void pushToVector(int reg, int* vec, int size);
    int* startVector(int size);
};

#endif

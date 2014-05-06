#ifndef DATE_H
#define DATE_H
 
class Watcher
{
private:
 
public:
   Watcher() {}
void start();
void finish();
void registerInstruction(int rs, int rt, int rd);
void checkForHazard();
void branchInstruction(int oldpc, int newpc, int intendedPC);
};
 
#endif

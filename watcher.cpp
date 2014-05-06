#include "watcher.h"
 
void Watcher::start(){
}

void Watcher::finish(){
}

void Watcher::registerInstruction(int rd, int rs, int rt	) {
	checkForHazard();
}

void Watcher::branchInstruction(int oldpc, int newpc, int intendedPC) {
}

void Watcher::checkForHazard() {

}

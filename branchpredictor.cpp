#include "branchpredictor.h"

State::State(){
	state=0;
	guessing=true;
}

void State::taken(){
	if(state!=0)	state--;
	updateGuessing();
}

int State::getState(){
	return state;
}

void State::notTaken(){
	if(state!=3)	state++;
	updateGuessing();
}

bool State::guess(){
	return guessing;
}

void State::updateGuessing(){
	if(state==0 || state==1) guessing=true;
	else guessing=false;
}

bimodalPredictor::bimodalPredictor(){
	numRightGuess=numWrongGuess=0;
}

int bimodalPredictor::getNumWrongGuess(){
	return numWrongGuess;
}

int bimodalPredictor::getNumRightGuess(){
	return numRightGuess;
}

void bimodalPredictor::updateState(bool path){
	if(path)	s.taken();
	else 		s.notTaken();	
}

void bimodalPredictor::updateGuess(bool path){
	if(s.guess()==path)	numRightGuess++;
	else			numWrongGuess++;
}

void bimodalPredictor::update(bool path){
	updateGuess(path);
	updateState(path);
}

double bimodalPredictor::getGuessRate(){
	return ((double)numRightGuess/(double)(numRightGuess+numWrongGuess));
}

oneLevelPredictor::oneLevelPredictor(){
	numRightGuess=numWrongGuess=0;
}

int oneLevelPredictor::getNumWrongGuess(){
	return numWrongGuess;
}

int oneLevelPredictor::getNumRightGuess(){
	return numRightGuess;
}

void oneLevelPredictor::updateState(bool path, int offset){
	if(path)	table[offset & (ONELEVELTABLESIZE-1)].taken();
	else 		table[offset & (ONELEVELTABLESIZE-1)].notTaken();	
}

void oneLevelPredictor::updateGuess(bool path, int offset){
	if(table[offset & (ONELEVELTABLESIZE-1)].guess()==path)	numRightGuess++;
	else			numWrongGuess++;
}

void oneLevelPredictor::update(bool path, int offset){
	updateGuess(path, offset);
	updateState(path, offset);
}

double oneLevelPredictor::getGuessRate(){
	return ((double)numRightGuess/(double)(numRightGuess+numWrongGuess));
}

predictor::predictor(){
}

void predictor::update(bool path, int offset){
	bimod.update(path);
	onelev.update(path, offset);
} 

double predictor::getBimodalGuessRate(){
	return bimod.getGuessRate();
}

double predictor::getOneLevelGuessRate(){
	return onelev.getGuessRate();
}


int predictor::getBimodalNumWrongGuess(){
	return bimod.getNumWrongGuess();
}

int predictor::getOneLevelNumWrongGuess(){
	return onelev.getNumWrongGuess();
}

void predictor::end(){
	printf("\n\n");
	printf("***Using the Bimodal Predictor:***\n");
	printf("Number of correct branch guesses: %d\n", bimod.getNumRightGuess());
	printf("Number of incorrect branch guesses: %d\n", bimod.getNumWrongGuess());
	printf("Guess rate: %F\n", bimod.getGuessRate());

	printf("***Using the Onelevel Predictor:***\n");
	printf("Number of correct branch guesses: %d\n", onelev.getNumRightGuess());
	printf("Number of incorrect branch guesses: %d\n", onelev.getNumWrongGuess());
	printf("Guess rate: %F\n", onelev.getGuessRate());
}

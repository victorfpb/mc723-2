#ifndef BRANCHPREDICTOR_H
#define BRANCHPREDICTOR_H

#include <stdio.h>

#define ONELEVELTABLESIZE 2<<8

class State {
/*Maquina de estados que implementa o bimodal predictor. A variavel state representa a máquina de estados. O valor 0 representa o estado "Strongly taken" (que eh o inicial),
 * o valor 1 representa "Weakly Taken", o valor 2 representa "Weakly not taken" e o valor 3 representa "Strongly not taken".
 * O booleano guessing contém a suposicao do predictor sobre se o proximo branch sera utilizado ou nao (seu valor inicial eh true baseado no valor inicial de state).  */

	private:
	int state;
	bool guessing;

	void updateGuessing();
	
	public:
	State();
	
	void taken();
	void notTaken();
	bool guess();
};

class bimodalPredictor {

	private:
	State s;
	int numWrongGuess;	
	int numRightGuess;
	
	void updateGuess(bool path);
	void updateState(bool path);

	
	public:
	bimodalPredictor();

	void update(bool path);
	int getNumRightGuess();
	int getNumWrongGuess();
	double getGuessRate();
};

class oneLevelPredictor{
	private:
	State table[ONELEVELTABLESIZE];
	int numWrongGuess;
	int numRightGuess;

	void updateGuess(bool path, int offset);
	void updateState(bool path, int offset);

	public:
	oneLevelPredictor();

	void update(bool path, int offset);
	int getNumWrongGuess();
	int getNumRightGuess();
	double getGuessRate();
};

class predictor {
	private:	
	bimodalPredictor bimod;
	oneLevelPredictor onelev;
	
	public:
	predictor();
	
	void update(bool path, int offset);
	double getBimodalGuessRate();
	double getOneLevelGuessRate();
	void end();
};

#endif

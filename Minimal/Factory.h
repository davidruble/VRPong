#pragma once

#include <vector>
#include <ctime>

#include "Molecule.h"

#define FACTORY_PATH "Assets/paddle/paddle.obj"
#define NUM_MOL_INIT 5
#define SECS_BTWN_EMIT 1
#define MAX_MOLS 10
#define MOLS_ON_LOSE 100

class Factory : protected Model
{
public:
	Factory();
	~Factory();

	void Draw(Shader shader);
	void update();
	void restart(bool trigger);

	int & getNumCO2Molecules() { return numCO2Molecules; }
	vector<Molecule*> getMolecules() { return molecules; }

	static bool gameLost;
	static bool gameWon;

private:
	Molecule* createMolecule();

	vector<Molecule*> molecules;
	int numCO2Molecules;

	clock_t timer;
};

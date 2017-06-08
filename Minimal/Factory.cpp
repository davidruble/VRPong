#include "Factory.h"
#include "Molecule.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

bool Factory::gameLost = false;
bool Factory::gameWon = false;

Molecule* Factory::createMolecule()
{
	if (Molecule::modelLoaded)
		return new Molecule();
	else
		return new Molecule(true);
}

Factory::Factory() : Model(FACTORY_PATH)
{
	cout << "\nCreating Factory..." << endl;
	numCO2Molecules = NUM_MOL_INIT;
	for (GLuint i = 0; i < this->meshes.size(); i++) {
		meshes[i].toWorld = glm::translate(glm::scale(meshes[i].toWorld, glm::vec3(0.1f, 0.1f, 0.1f)), glm::vec3(0.0f, 1.0f, -15.0f));
		meshes[i].color = glm::vec3(0.5f, 0.6f, 0.5f);
	}

	// calculate the bounding box origin
	glm::mat4 boundsMat = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)), glm::vec3(0.0f, 1.0f, -30.0f - BOUNDS_DIST));
	boundsOrigin = glm::vec3(boundsMat[3]) + glm::vec3(0.0f, BOUNDS_DIST, 0.0f);

	// Create the first five molecules
	for (int i = 0; i < NUM_MOL_INIT; ++i)
	{
		molecules.push_back(createMolecule());
	}

	timer = std::clock();
}

Factory::~Factory()
{
	for (int i = 0; i < molecules.size(); ++i)
	{
		delete molecules[i];
		molecules[i] = NULL;
	}
	molecules.clear();
	Molecule::cleanup();
}

void Factory::Draw(Shader shader)
{
	
	Model::Draw(shader);
	
	for (int i = 0; i < molecules.size(); ++i)
	{
		molecules[i]->Draw(shader);
	}
}

void Factory::update()
{
	// YOU WIN!
	if (!gameWon && numCO2Molecules <= 0)
	{
		// change the background color to light blue
		glClearColor(0.1f, 0.1f, 1.0f, 1.0f);
		gameWon = true;

		cout << "*************** YOU WIN!!!! *****************" << endl;
	}

	// Emit a new CO2 molecule every second
	else if (!gameWon && numCO2Molecules <= MAX_MOLS)
	{
		if ((std::clock() - timer) / (double)CLOCKS_PER_SEC >= SECS_BTWN_EMIT)
		{
			molecules.push_back(createMolecule());
			++numCO2Molecules;
			timer = std::clock();
		}

		for (int i = 0; i < molecules.size(); ++i)
		{
			molecules[i]->update();
		}
	}

	// YOU LOSE
	else if (!gameLost && !gameWon)
	{
		// spawn a bunch of molecules because you hate the environment
		for (int i = 0; i < MOLS_ON_LOSE; ++i)
		{
			Molecule* mol = createMolecule();
			mol->randomizePosition();
			molecules.push_back(mol);
		}
		gameLost = true;

		cout << "*************** YOU LOSE!!!! *****************" << endl;
		
	}
}

// called after game win/loss and user presses a button
void Factory::restart(bool trigger)
{
	 
	if ((gameWon || gameLost)) {
		glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
		cout << "\n\n\nRestarting game..." << endl << endl;
		
		numCO2Molecules = NUM_MOL_INIT;
	
		for (int i = 0; i < molecules.size(); ++i)
		{
			delete molecules[i];
			molecules[i] = NULL;
		}
		molecules.clear();
		gameWon = false;
		gameLost = false;
	
		// recreate the first five molecules
		for (int i = 0; i < NUM_MOL_INIT; ++i)
		{
			molecules.push_back(createMolecule());
		}

		timer = std::clock();
	}
}
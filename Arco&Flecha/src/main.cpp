//
// Tecnología de la Programación de Videojuegos 1
// Facultad de Informática UCM
//
// Plantilla de proyecto con SDL
//

#include "game.h"

int main(int argc, char* argv[])
{
	try {
		Game().run();
	}
	

	// TODO: manejar excepciones
	catch (const string& e) {
		cout << "ERROR CRITICO: " << e << endl;
		return 1;
	}
	return 0;
}

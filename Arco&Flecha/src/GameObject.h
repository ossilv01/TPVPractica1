#pragma once
class Game;
class GameObject
{

//Para los herederos (que duro suena eso)
protected:
	Game* game = nullptr;

public:
	GameObject(Game* game): game(game){}
	//Metodos virutales puros
	virtual void render() const = 0;
	virtual bool update(float delta) = 0;
	//Destructor virtual
	virtual ~GameObject() {}

};


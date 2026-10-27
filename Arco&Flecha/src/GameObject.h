class Game;
class GameObject
{
private:
	Game* game = nullptr;

public:
	virtual void render() const = 0;
	virtual void update() = 0;
};


// Screen.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>

using namespace std;

void draw(char* loc, const char* face)
{
	strncpy(loc, face, strlen(face));
}

class Screen {
	int size;
	char* screen;

public:
	Screen() {}
	Screen(int sz) : size(sz), screen(new char[sz + 1])
	{}

~Screen() 
	{ 
		if (screen) {
			delete[] screen;
			screen = nullptr;
		}
	}

	void draw(int pos, const char* face) 
	{
		if (face == nullptr) return;
		if (pos < 0 || pos >= size) return;
		strncpy(&screen[pos], face, strlen(face));
	}

	void render() 
	{
		printf("%s\r", screen);
	}

	void clear()
	{
		memset(screen, ' ', size);
		screen[size] = '\0';
	}

	int length()
	{
		return size;
	}

};

class GameObject {
	int pos;
	char face[20];
	Screen& screen;

public:
	GameObject() : screen(screen) {}
	GameObject(int pos, const char* face, Screen& screen)
		: pos(pos), screen(screen)
	{	strcpy(this->face, face);	}

	~GameObject() {}

	
	int getPosition()
	{
		return pos;
	}
	void setPosition(int pos)
	{
		this->pos = pos;
	}
	virtual void draw()
	{
		screen.draw(pos, face);
	}
	virtual void update()
	{
	}
	virtual void update(int )
	{
	}
};

class Player : public GameObject {
	float hp;
	bool isAlive;
public:
	Player() {}
	Player(int pos, const char* face, Screen& screen)
		: GameObject(pos, face, screen), hp(10), isAlive(true)
	{}

	~Player() {}

	
	void moveLeft()
	{
		setPosition(getPosition() - 1);
	}

	void moveRight()
	{
		setPosition(getPosition() + 1);
	}

	bool state()
	{
		if (hp > 0)
			isAlive = true;
		else
			isAlive = false;
		return isAlive;
	}

	void update()
	{
		state();
	}

};

class Enemy : public GameObject {
	float hp;
public:
	Enemy() {}
	Enemy(int pos, const char* face, Screen& screen) 
		: GameObject(pos, face, screen), hp(5)
	{}

	~Enemy() {}

	void moveRandom()
	{
		setPosition( getPosition() + rand() % 3 - 1);
	}

	void update()
	{
		moveRandom();
	}
};

class Bullet : public GameObject {
	bool isFiring;

public:
	Bullet() {}
	Bullet(int pos, const char* face, Screen& screen) 
		: GameObject(pos, face, screen), isFiring(false)
	{}

	~Bullet() {}

	void moveLeft()
	{
		setPosition(getPosition() - 1);
	}

	void moveRight()
	{
		setPosition(getPosition() + 1);
	}

	void draw()
	{
		if (isFiring == false) return;
		GameObject::draw();
	}

	void fire(int player_pos)
	{
		isFiring = true;
		setPosition(player_pos);
	}

	/*
	bool state()
	{
		if (isFiring == true)
			return true;
		else
			return false;
	}
	*/

	void update(int enemy_pos)
	{
		if (isFiring == false) return;
		int pos = getPosition();
		if (pos < enemy_pos) {
			pos = pos + 1;
		}
		else if (pos > enemy_pos) {
			pos = pos - 1;
		}
		else {
			isFiring = false;
		}
		setPosition(pos);
	}
};

#define NORMAL_PLAY 1

#if NORMAL_PLAY
int main()
{
	Screen screen{ 80 };
	Player player ( 30, "(^_^)", screen );
	Enemy enemy ( 60, "(*--*)", screen );
	Bullet bullet (-1, "+", screen);

	//Player players[2] = { Player(30, "(^_^)", screen), Player(-1, "(^_^)", screen) };
	//Enemy enemies[5] = { Enemy(60, "(*--*)", screen), };
	//Bullet bullets[10] = { Bullet(-1, "+", screen), Bullet(-1, "+", screen),
	//	Bullet(-1, "+", screen), Bullet(-1, "+", screen), Bullet(-1, "+", screen),
	//	Bullet(-1, "+", screen), Bullet(-1, "+", screen), Bullet(-1, "+", screen),
	//	Bullet(-1, "+", screen), Bullet(-1, "+", screen) };

	GameObject *gameObjects[20] = { &player, &bullet, &enemy, '\0', };

	while (true)
	{
		screen.clear();

		int playerNum = 0;

		if (_kbhit())
		{
			int c = _getch();
			switch (c) {
			case 'a':
				player.moveLeft();
				/*for (int i = 0; i < sizeof(players); i++)
				{
					if (players[i].state() == true)
					{
						player.moveLeft();
						playerNum = i;
						break;
					}
					else
						continue;
				}*/
				break;
			case 'd':
				player.moveRight();
				/*for (int i = 0; i < sizeof(players); i++)
				{
					if (players[i].state() == true)
					{
						player.moveRight();
						playerNum = i;
						break;
					}
					else
						continue;
				}*/
				break;
			case ' ':
				bullet.fire(player.getPosition());
				/*for (int i = 0; i < sizeof(bullets); i++)
				{
					if (bullets[i].state() == false)
					{
						bullets[i].fire(player.getPosition());
						break;
					}
					else
						continue;
				}*/
				break;
			}
		}

		for (int i = 0; i <= sizeof(gameObjects) / sizeof(GameObject); i++)
		{ 
			if (gameObjects[i] != NULL)
				gameObjects[i]->draw();
			else
				continue;
		}

		for (int i = 0; i <= sizeof(gameObjects) / sizeof(GameObject); i++)
		{
			if (gameObjects[i] != NULL)
			{
				gameObjects[i]->update(); //나중엔 enemy를 enemeis로!?
				gameObjects[i]->update(enemy.getPosition());
			}
			else
				continue;
		}

		//player->draw();
		//enemy->draw();
		//bullet->draw();

		//player.update();
		//enemy.update();
		//bullet.update(enemy.getPosition());
		
		screen.render();
		Sleep(66);
	}

	return 0;
}
#else

void test()
{
	Screen* screen = nullptr;

	screen = new Screen(80);
	/* 80 };
	Player player = { 30, "(^_^)", &screen };
	Enemy enemy{ 60, "(*--*)", &screen };
	Bullet bullet(-1, "+", &screen);
	*/
	
}

int main()
{
	test();


	Screen* another = nullptr;
	int ch = _getch();
	return 0;
}

#endif
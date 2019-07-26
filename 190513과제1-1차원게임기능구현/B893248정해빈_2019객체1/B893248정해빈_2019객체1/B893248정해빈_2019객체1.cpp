#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <ctime>

using namespace std;

void draw(char* loc, const char* face)
{
	strncpy(loc, face, strlen(face));
}

enum SelectWeapons { rifle = 1, laser = 2 };

class Screen {
	int size;
	char* screen;

public:
	Screen(int sz) : size(sz), screen(new char[sz + 1]) {}
	~Screen() { delete[] screen; }

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
	int hp;
	int pos;
	char face[20];
	Screen* screen;

public:
	GameObject(int pos, const char* face, Screen* screen)
		: pos(pos), screen(screen)
	{
		strcpy(this->face, face);
	}


	int getPosition()
	{
		return pos;
	}
	void setPosition(int pos)
	{
		this->pos = pos;
	}
	void draw()
	{
		screen->draw(pos, face);
	}
};

class Player : public GameObject {
	int hp;
public:
	Player(int pos, const char* face, Screen* screen)
		: GameObject(pos, face, screen), hp(10)
	{
	}

	void moveLeft()
	{
		if (getPosition() == 0)
			return;
		setPosition(getPosition() - 1);
	}

	void moveRight()
	{
		if (getPosition() == 75)
			return;
		setPosition(getPosition() + 1);
	}

	//*****
	int getDirection(int dir)
	{
		int direction=0;

		if (dir < 0)
			return -1;
		else
			return 1;
	}
	//*****

	void update()
	{
	}

};

class Enemy : public GameObject {
	int hp;
	int movementTime, movementCount, coolTime, coolCount;
public:
	Enemy(int pos, const char* face, Screen* screen)
		: GameObject(pos, face, screen), hp(5), coolCount(0)
	{
	}

	void moveToPlayer(int player_pos)
	{
		//2초에 1m 이동
		if (getPosition() > (player_pos + 4) && movementCount == 2)
		{
			setPosition(getPosition() - 1);
			movementCount = 0;
		}
		else if (getPosition() < player_pos && movementCount == 2)
		{
			setPosition(getPosition() + 1);
			movementCount = 0;
		}
	}

	void decrease_hp()
	{
		if (this->hp > 0)
			this->hp--;
	}

	void update(int player_pos, int weapon)
	{
		++movementTime;

		if (movementTime == 15)
		{
			movementTime = 0;
			movementCount += 1;
		}
		moveToPlayer(player_pos);

		coolCount = 0;
		while (weapon == laser && coolCount<2)
		{
			++coolTime;
			if (coolTime = 15)
			{
				decrease_hp();
				coolCount += 1;
				coolTime = 0;
			}
		}
		coolCount = 0;
		coolTime = 0;

	}

	~Enemy() {

	}
};

class Bullet : public GameObject, public Screen {
	bool isFiring;
	bool direction; // true == 왼쪽, false == 오른쪽
	bool isUsable, reload;
	int rifleCoolTime, laserCoolTime, coolCount;
	int recordBullets, bulletCounter; //총 10발의 총알
	int weaponNum;

public:
	Bullet(int pos, const char* face, Screen* screen)
		: GameObject(pos, face, screen), Screen(80), isFiring(false), direction(false), isUsable(true), reload(false), recordBullets(10), bulletCounter(10)
	{
	}

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

	void fire(int player_pos, int dir, int weaponNum)
	{
		this->weaponNum = weaponNum;
		
		switch (weaponNum)
		{
		case rifle:
			//비어 있을 경우, 1초당 한 발씩 총알이 자동 추가된다.
			if (bulletCounter < 1)
			{
				recordBullets = bulletCounter;
				reload = true;
				rifleCoolTime = 0;
				return;
			}
			else
			{
				isFiring = true;
				--bulletCounter;
			}

			break;

		case laser:
			// 레이저건 쿨타임
			if (!isUsable)
			{
				++laserCoolTime;
				if (laserCoolTime >= 45)
				{
					isUsable = true;
					laserCoolTime = 0;
				}
			}
			
			break;
		}


		//Direction of Attack
		if (dir > 0) // right
		{
			direction = false;
			setPosition(player_pos + 4);

		}
		else if (dir < 0) // left
		{
			direction = true;
			setPosition(player_pos);
		}
	}

	void update(int enemy_pos)
	{
		if (isFiring == false) return;

		int pos = getPosition();
		if (weaponNum == 1) //rifle
		{
			//Start CoolTimer
			if (reload == true)
				++rifleCoolTime;
			else
				rifleCoolTime = 0;

			//Bullet's pos
			if (direction == false)
				pos = pos + 1;
			else if (direction == true)
				pos = pos - 1;

			setPosition(pos);

			//Erase a Bullet
			if (pos == enemy_pos || pos == enemy_pos + 5 || pos < 1 || pos > 79)
			{
				setPosition(-1);
			}

			// 재장전
			if (reload == true && rifleCoolTime == 15 && recordBullets == bulletCounter && bulletCounter < 10)
			{
				rifleCoolTime = 0;
				bulletCounter += 1;
				recordBullets = bulletCounter;
			}
			else if (reload == true && recordBullets != bulletCounter)
			{
				reload = false;
				rifleCoolTime = 0;
			}
		}
		else // laser
		{
			/*
			++laserCoolTime;
			if (laserCoolTime >= 30 && direction==false)
			{
				int len = enemy_pos - getPosition() + 4;
				//레이저그리기
			}
			else if (laserCoolTime >= 30 && direction == true)
			{
				int len = enemy_pos + 5 - getPosition();
				//레이저그리기
			}
			*/
		}
	}
};

int main()
{
	Screen screen{ 80 };
	Player player = { 30, "(^_^)", &screen };
	Enemy enemy{ 60, "(*--*)", &screen };
	Bullet bullet(-1, "+", &screen);

	enum SelectWeapons weapon;
	weapon = rifle;

	while (true)
	{
		screen.clear();
		player = { player.getPosition(), "(^_^)", &screen };

		if (_kbhit())
		{
			static int direction = 0;

			int c = _getch();
			switch (c) {
			case 'a':
				player = { player.getPosition(), "(_^ )", &screen };
				player.moveLeft();
				direction = player.getDirection(-1);
				break;
			case 'd':
				player = { player.getPosition(), "( ^_)", &screen };
				player.moveRight();
				direction = player.getDirection(1);
				break;
			case ' ':
				bullet.fire(player.getPosition(), direction, weapon);
				break;
			case 'm':
				if (weapon == rifle)
					weapon = laser;
				else
					weapon = rifle;
				break;
			}
		}
		player.draw();
		bullet.draw();
		enemy.draw();

		if (enemy.getPosition() == bullet.getPosition())
			enemy.decrease_hp();

		player.update();
		bullet.update(enemy.getPosition());
		enemy.update(player.getPosition(), weapon);

		screen.render();

		Sleep(66);
	}

	return 0;
}

/*
구현 기능:(4번에서는 연사만 구현못했습니다.)
1. 플레이어는 방향성을 가진다. 오른쪽으로 이동하고자 한다면 머리방향이 먼저 오른쪽으로 방향전환한 후 이동한다. 머리 방향이 전환되었다는 것을 확인하기 위해 머리모양에 해당하는 그림을 표시하도록 한다.
2. 플레이어는 공격시에 현재 방향으로만 공격가능하다.
4. 총 공격의 경우, [총 10발의 총알이 있으며] 연사 가능하며 [비어 있을 경우, 1초당 한 발씩 총알이 자동 추가된다. 단, 총알이 없어야 자동 장전이 가능하다.] [10초간 기다리면 모두 장전할 수 있으나 기다리지 못하고 총알을 쏘게되면 추가로 1초당 한 발씩 총알이 자동 추가되지 않는다].
7. 공격 전환의 경우, ‘m’키를 이용한다.
12. 적은 2초당 1m만큼의 속도로 플레이어를 향해 이동한다.
13. 적, 플레이어는 모두 hp를 가지며 적은 5, 플레이어는 10만큼 갖는다.
추가구현1. 적, 플레이어는 범위 내에서만 움직일 수 있다.
*/
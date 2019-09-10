// Screen.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "Utils.h"

using namespace std;

class GameObject
{
private:
	Position pos;
	char *shape;
	int width;
	int height;

public:
	GameObject(const char *shape, unsigned width, unsigned height) { //unsigned int (u_int표준아님)
		if (!shape || strlen(shape) == 0 || width == 0 || height == 0) { //비정상 입력에 대한 예외처리 (if문 평가순서 고려할 것)
			cout << "something went wrong" << endl; //에러가 있음을 명시 (자동보정 때문에 에러표시가 없으면 알아차리기 어려움)
			this->shape = new char[1];
			this->width = 1;
			this->height = 1; //set defalt values
		}
		else {
			this->shape = new char(strlen(shape) + 1);
			strcpy(this->shape, shape); //string class 사용 가능
			this->width = width;
			this->height = height;
		}
		setPos(0, 0);
	}
	virtual ~GameObject() {

	}
	void erase();
	void draw();
	virtual void update();
	void setPos(int x, int y) { //이미 inline 함수
		this->pos.x = x;
		this->pos.y = y;
	}
};

int main()
{
	GameObject player("xxxyyyzzz",3,3); // (shape , width, height)
	GameObject enemy("aabbcc", 2,3);

	player.setPos(1, 1);
	enemy.setPos(5, 3);
	
	int count = 100;
	while (count-- > 0) {
		player.erase();
		enemy.erase();

		player.draw();
		enemy.draw();

		Sleep(300);

		player.update();
		enemy.update();
	}

	return 0;
}
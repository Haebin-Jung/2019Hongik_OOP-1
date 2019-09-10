// Screen.cpp : �� ���Ͽ��� 'main' �Լ��� ���Ե˴ϴ�. �ű⼭ ���α׷� ������ ���۵ǰ� ����˴ϴ�.
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
	GameObject(const char *shape, unsigned width, unsigned height) { //unsigned int (u_intǥ�ؾƴ�)
		if (!shape || strlen(shape) == 0 || width == 0 || height == 0) { //������ �Է¿� ���� ����ó�� (if�� �򰡼��� ����� ��)
			cout << "something went wrong" << endl; //������ ������ ��� (�ڵ����� ������ ����ǥ�ð� ������ �˾������� �����)
			this->shape = new char[1];
			this->width = 1;
			this->height = 1; //set defalt values
		}
		else {
			this->shape = new char(strlen(shape) + 1);
			strcpy(this->shape, shape); //string class ��� ����
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
	void setPos(int x, int y) { //�̹� inline �Լ�
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
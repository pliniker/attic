// Menu header file. Links in other required files:

#include <string.h>

class Menu
{
protected:
	int _X1,_Y1,_X2,_Y2;
	char *_Name;
public:
	Menu (int _X1,int _Y1,int _X2,int _Y2, char *_Name);
	int Get_X1() { return _X1; }
	int Get_Y1() { return _Y1; }
	int Get_X2() { return _X2; }
	int Get_Y2() { return _Y2; }
	void View()
	{
		Rectangle(15,_X1,_Y1,_X2,_Y2);
		PutWord(_X1+2,_Y1+3,7,_Name);
	}
};

Menu::Menu (int mX1,int mY1,int mX2,int mY2, char *mName)
{
	_X1 = mX1;
	_Y1 = mY1;
	_X2 = mX2;
	_Y2 = mY2;
	_Name  = mName;
}


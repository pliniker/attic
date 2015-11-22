// main header file linking the others

void Window(int _X1,int _Y1,int _X2,int _Y2,char *Title)
{
	Rectangle(15,_X1,_Y1,_X2,_Y2);
	PutWord(_X1 + 2,_Y1 + 2,7,Title);
}


void Menu(int _X1,int _Y1,int _X2,int _Y2,char *Title)
{
	Rectangle(15,_X1,_Y1,_X2,_Y2);
	PutWord(_X1 + 2,_Y1 + 2,7,Title);
}




#include <gtk/gtk.h>

// consts defined for horizontal or vertical boxes
#define HORIZ 1
#define VERT 2
#define ON 1
#define OFF 0

// call this and the application will die - gtk_main() will finish
void AppKiller(	GtkWidget *widget,
		gpointer data);


// window close handler
gint WindowKiller( 	GtkWidget *widget,
			GdkEvent *event,
			gpointer data);
			
			
// base widget class. all gtk widgets should be derived from this
class Widget
{
public:
	GtkWidget *widget;
	GtkWidgetFlags flags;

public:
	Widget();
	void Add(Widget *obj);		// contain a widget within this one
	void Del(Widget *obj);		// remove the same
	void Show();				// display the widget
	void Hide();				// hide it
	int  Status();				// return whether the widget is visible or not
	void Enable();
	void Disable();
	void Resize(int x, int y);
};




/**************************************************************/

// Window classes


// generic window class
class Window : public Widget
{
protected:
	int x, y, border;
public:
	Window(char *t, int x, int y, int b = 10);		// border defaults to 10
	
	void SetSize(int x, int y);
	void SetBorder(int b);
	int GetX();
	int GetY();
};



// sub - window, doesn't end program on close
class WindowSub : public Window
{
public:
	WindowSub(char *t, int x, int y, int b = 10) :
		Window(t,x,y,b) {
			gtk_signal_connect( GTK_OBJECT(widget), "delete_event",
				GTK_SIGNAL_FUNC(WindowKiller), NULL);
		};
};



// main app window - ends program on close
class WindowMain : public Window
{
public:
	WindowMain(char *t, int x, int y, int b = 10) :
		Window(t,x,y,b) {
			gtk_signal_connect( GTK_OBJECT(widget), "destroy",
				GTK_SIGNAL_FUNC(AppKiller), NULL);
		};
};


/**************************************************************/


// standard button stuff
class Button : public Widget
{
public:
	Button(char *t);
	void SetHandler(void *p);
};


// toggle button widget
class Toggle : public Widget
{
public:
	Toggle(char *t);
	void SetHandler(void *p);
	void SetState(int s);
	int GetState();
};

/**************************************************************/



class Box : public Widget
{
private:
	char horiz_or_vert;
	gboolean s, t;
public:
	Box(char hv, gboolean a, gboolean b);
	void Pack(Widget *obj);
};


/**************************************************************/



class Draw : public Widget
{
private:
	int xc,yc;
public:
	Draw(int x, int y);
	void Update();
	void SetSize(int x, int y);
	void SetHandler(void *p);

	int GetX();
	int GetY();
};




/**************************************************************/


class Tile
{
private:
	GdkGC *context;
	int xs,ys;
	
public:
	Tile(Widget *obj, int x, int y);
	void Draw(int x, int y);
	
	void SetColour(guint32 colour);
	void SetSize(int x, int y);
};


/**************************************************************/


class Scrollbar : public Widget
{
private:
	GtkObject *adj;

public:
	Scrollbar(char hv, unsigned char min, unsigned char max);
	unsigned char Value();
	void Set(unsigned char v);
	void SetMax(unsigned char m);
	void SetHandler(void *p);
};


/**************************************************************/


class Label : public Widget
{
public:
	Label(char *t);
	void Change(char *t);
};


/**************************************************************/



class CheckBox : public Widget
{
public:
	CheckBox(char *t);
	void SetHandler(void *p);
};


/**************************************************************/


class List : public Widget
{
 private:
  char **items;
  GtkWidget *list;

 public:
  List(int w);
  void SetHandler(void *p);
  void Add(unsigned char *item);
  void Del(unsigned char *item);
};

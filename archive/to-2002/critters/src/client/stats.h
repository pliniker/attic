



#include <gtk/gtk.h>


#define ENABLED 1
#define DISABLED 0

#define VALUES 10

#define STAT_WIDTH 512
#define STAT_HEIGHT 250


class Chart
{
protected:
	int xc,yc;

public:
	GtkWidget *widget;
	
public:
	Chart(int x, int y);
	void Draw();
};



struct Value
{
	unsigned long int y0, y1;
	guint32 colour;
	GdkGC *context;
	int status;
};


class StatsWindow
{
protected:
	GtkWidget *widget, *vbox, *hbox, *button;
	
	Chart *chart;
	
public:
	StatsWindow();
	void Show();
	void Hide();
	int Status();
	void Draw();
	void SetColour(int i, guint32 c);
	void SetScale(int size);
	
	void SetData(int i, long int d);
	void SetDataStatus(int i, int s);
	int DataIsEnabled(int i);

	void Update(unsigned char *d, int len);
};

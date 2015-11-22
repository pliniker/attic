

#include "windows.h"



void AppKiller(	GtkWidget *widget,
		gpointer data)
{
	gtk_main_quit();
}



gint WindowKiller( 	GtkWidget *widget,
			GdkEvent *event,
			gpointer data)
{
//	gtk_widget_hide(widget);
	return TRUE;
}


/**************************************************************/



Widget::Widget()
{
}


void Widget::Show()
{
	flags = (GtkWidgetFlags) GTK_WIDGET_FLAGS(widget);
	gtk_widget_show(widget);
}


void Widget::Hide()
{
	gtk_widget_hide(widget);
}


void Widget::Add(Widget *obj)
{
	gtk_container_add(GTK_CONTAINER(widget), obj->widget);
	obj->Show();
}


int Widget::Status()
{
	return (GTK_WIDGET_FLAGS (widget) & GTK_VISIBLE);
}


void Widget::Enable()
{
//	GTK_WIDGET_SET_FLAGS(widget, flags);
	Show();
}

void Widget::Disable()
{
//	GTK_WIDGET_SET_FLAGS(widget, flags & (!GTK_SENSITIVE));
	Hide();
}


void Widget::Resize(int x, int y)
{
	gtk_widget_set_usize(widget, x, y);
}


/**************************************************************/



Window::Window(char *t, int x, int y, int b = 10)
{
	widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(widget), t);
	gtk_window_set_default_size(GTK_WINDOW(widget), x, y);
	gtk_container_set_border_width(GTK_CONTAINER(widget), b);
}

int Window::GetX()
{
	return widget->allocation.width;
}


int Window::GetY()
{
	return widget->allocation.height;
}


void Window::SetSize(int x, int y)
{
	gtk_widget_set_usize(GTK_WIDGET(widget), (gint) x, (gint) y);
}


/**************************************************************/



Button::Button(char *t)
{
	widget = gtk_button_new_with_label(t);
}


void Button::SetHandler(void *p)
{
	gtk_signal_handlers_destroy(GTK_OBJECT(widget));
	gtk_signal_connect(	GTK_OBJECT(widget), "clicked",
					GTK_SIGNAL_FUNC(p), NULL);
}


Toggle::Toggle(char *t)
{
	widget = gtk_toggle_button_new_with_label(t);
}


void Toggle::SetHandler(void *p)
{
	gtk_signal_handlers_destroy(GTK_OBJECT(widget));
	gtk_signal_connect(	GTK_OBJECT(widget), "toggled",
					GTK_SIGNAL_FUNC(p), NULL);
}


void Toggle::SetState(int s)
{
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), (gint) s);
}


int Toggle::GetState()
{
	return (int) gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
}

/**************************************************************/



Box::Box(char hv, gboolean a, gboolean b)
{
	if (hv == HORIZ) {
		widget = gtk_hbox_new(FALSE, 0);
	} else {
		widget = gtk_vbox_new(FALSE, 0);
	}
	s = a; t = b;
	gtk_container_set_border_width(GTK_CONTAINER(widget), 0);
}



void Box::Pack(Widget *obj)
{
	gtk_box_pack_start(GTK_BOX(widget), obj->widget, s, t, 0);
	obj->Show();
}



/**************************************************************/

GdkPixmap *pixmap = 0;
int pixmap_x, pixmap_y;

gint configure_event (GtkWidget *wid, GdkEventConfigure *event)
{
  printf("Testing configure_event wid=%x, event=%x\n", wid, event);
	if (pixmap) gdk_pixmap_unref(pixmap);
        
	pixmap = gdk_pixmap_new(wid->window,
						wid->allocation.width,
						wid->allocation.height,
						-1);
                            
        pixmap_x = wid->allocation.width;
        pixmap_y = wid->allocation.height;
	return TRUE;
}


gint expose_event (GtkWidget *wid, GdkEventExpose *event)
{
  printf("Testing expose_event wid=%x, event=%x\n", wid, event);
        gdk_draw_pixmap(wid->window,
                        wid->style->fg_gc[GTK_WIDGET_STATE (wid)],
                        pixmap,
                        event->area.x, event->area.y,
                        event->area.x, event->area.y,
                        event->area.width, event->area.height);

	return FALSE;
}
      

Draw::Draw(int x, int y)
{
	xc = x; yc = y;

	widget = gtk_drawing_area_new();
	gtk_widget_set_usize(GTK_WIDGET(widget), (gint) x, (gint) y);
	
	gtk_signal_connect(	GTK_OBJECT (widget), "expose_event",
					(GtkSignalFunc) expose_event, NULL);
	gtk_signal_connect(	GTK_OBJECT (widget), "configure_event",
					(GtkSignalFunc) configure_event, NULL);
					
	gtk_widget_set_events(widget, GDK_EXPOSURE_MASK);
}


void Draw::Update()
{
	gdk_draw_pixmap(	widget->window,
					widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
					pixmap, 0,0, 0,0, xc, yc);
}


void Draw::SetSize(int x, int y)
{
	int flag = 0;
	if (x != pixmap_x) {
		pixmap_x = x;
		xc = x;
		flag = 1;
	}
	if (y != pixmap_y) {
		pixmap_y = y;
		yc = y;
		flag = 1;
	}
	if (flag) gtk_widget_set_usize(GTK_WIDGET(widget), (gint) pixmap_x, (gint) pixmap_y);
}


void Draw::SetHandler(void *p)
{
	gtk_signal_connect (GTK_OBJECT (widget), "button_press_event",
					(GtkSignalFunc) p, NULL);

	gtk_widget_set_events (widget, GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK);
}

int Draw::GetX()
{
	return pixmap_x;
}

int Draw::GetY()
{
	return pixmap_y;
}

/**************************************************************/


Tile::Tile(Widget *obj, int x, int y)
{
	xs = x; ys = y;
	context = gdk_gc_new(pixmap);
	gdk_gc_copy(context, obj->widget->style->white_gc);
}


void Tile::Draw(int x, int y)
{
	gdk_draw_rectangle(	pixmap,	context,	
					TRUE, (gint) x, (gint) y, xs, ys);
}



void Tile::SetColour(guint32 colour)
{
	gdk_rgb_gc_set_foreground(context, colour);
	gdk_rgb_gc_set_background(context, colour);
}


void Tile::SetSize(int x, int y)
{
	xs = x;
	ys = y;
}

/**************************************************************/


Scrollbar::Scrollbar(char hv, unsigned char min, unsigned char max)
{
	adj = gtk_adjustment_new(0.0, min, max, 1.0, 1.0, 3.0);
	
	if (hv == HORIZ) {
		widget = gtk_hscrollbar_new( GTK_ADJUSTMENT(adj) );
	} else {
		widget = gtk_vscrollbar_new( GTK_ADJUSTMENT(adj) );
	}
	
	gtk_range_set_update_policy(GTK_RANGE(widget), GTK_UPDATE_DELAYED);
}


unsigned char Scrollbar::Value()
{
	return (unsigned char) GTK_ADJUSTMENT(adj)->value;
}

void Scrollbar::Set(unsigned char v)
{
	gtk_adjustment_set_value(GTK_ADJUSTMENT(adj), (gfloat) v);
}

void Scrollbar::SetMax(unsigned char m)
{
	GTK_ADJUSTMENT(adj)->upper = (gfloat) m;
}

void Scrollbar::SetHandler(void *p)
{
	gtk_signal_handlers_destroy(GTK_OBJECT(adj));
	gtk_signal_connect(	GTK_OBJECT(adj), "value_changed",
					GTK_SIGNAL_FUNC(p), NULL);
}


/**************************************************************/


Label::Label(char *t)
{
	widget = gtk_label_new(t);
}


void Label::Change(char *t)
{
	gtk_label_set_text(GTK_LABEL(widget), t);
}

	
/**************************************************************/


CheckBox::CheckBox(char *t)
{
	widget = gtk_check_button_new_with_label(t);
}


void CheckBox::SetHandler(void *p)
{
	gtk_signal_handlers_destroy(GTK_OBJECT(widget));
	gtk_signal_connect(	GTK_OBJECT(widget), "toggled",
					GTK_SIGNAL_FUNC(p), NULL);
}


/**************************************************************/


List::List(int w)
{
  items = 0;
  list = gtk_clist_new (1);
  gtk_clist_set_selection_mode((GtkCList *) list,GTK_SELECTION_SINGLE);
  gtk_clist_set_column_width((GtkCList *) list,1,w);

  widget = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (widget),
                                          GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
  gtk_container_add(GTK_CONTAINER(widget), list);
  gtk_widget_show(list);
}


void List::Add(unsigned char *item)
{
  items = &item;
  gtk_clist_append((GtkCList *) list, (char **) items );
}


void List::SetHandler(void *p)
{
  gtk_signal_handlers_destroy(GTK_OBJECT(list));
  gtk_signal_connect(GTK_OBJECT(list), "select_row",
		     GTK_SIGNAL_FUNC(p), NULL);
}

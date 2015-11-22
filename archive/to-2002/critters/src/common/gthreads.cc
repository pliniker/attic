
#include <gtk/gtk.h>
#include "gthreads.h"




Idle::Idle(void *f)
{
	p = f;
}


void Idle::Start()
{
	tag = gtk_idle_add( (GtkFunction) p, NULL);
}


void Idle::Stop()
{
	gtk_idle_remove(tag);
}



Timer::Timer(void *f, int r)
{
	t = r;
	p = f;
}


void Timer::Start()
{
	tag = gtk_timeout_add(t, (GtkFunction) p, NULL);
}


void Timer::Stop()
{
	gtk_timeout_remove(tag);
}



IOWait::IOWait(void *f, int d)
{
	s = d;
	p = f;
}


void IOWait::Start()
{
	tag = gdk_input_add(s, GDK_INPUT_READ, (GdkInputFunction) p, NULL);
}


void IOWait::Stop()
{
	gdk_input_remove(tag);
}







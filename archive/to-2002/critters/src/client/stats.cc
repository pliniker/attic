

#include <stdio.h>
#include "colours.h"
#include "stats.h"
#include "windows.h"
#include "../common/messages.h"


guint32 colours[VALUES];
Value graphdata[VALUES];
GdkPixmap *stat_pixmap = 0;
int stat_pixmap_x, stat_pixmap_y;
char gens[10];
GtkWidget *label;

float scalefactor;

gint stat_configure_event (GtkWidget *wid, GdkEventConfigure *event)
{
        if (stat_pixmap)
        	gdk_pixmap_unref(stat_pixmap);
	stat_pixmap = gdk_pixmap_new(wid->window,
						wid->allocation.width,
						wid->allocation.height,
						-1);
        gdk_draw_rectangle (stat_pixmap,
                            wid->style->black_gc,
                            TRUE,
                            0, 0,
                            wid->allocation.width,
                            wid->allocation.height);
                            
        stat_pixmap_x = wid->allocation.width;
        stat_pixmap_y = wid->allocation.height;
	return TRUE;
}


gint stat_expose_event (GtkWidget *wid, GdkEventExpose *event)
{
        gdk_draw_pixmap(wid->window,
                        wid->style->fg_gc[GTK_WIDGET_STATE (wid)],
                        stat_pixmap,
                        event->area.x, event->area.y,
                        event->area.x, event->area.y,
                        event->area.width, event->area.height);

	return FALSE;
}
      

Chart::Chart(int x, int y)
{
	xc = x; yc = y;

	widget = gtk_drawing_area_new();
	gtk_widget_set_usize(GTK_WIDGET(widget), (gint) x, (gint) y);
	
	gtk_signal_connect(	GTK_OBJECT (widget), "expose_event",
					(GtkSignalFunc) stat_expose_event, NULL);
	gtk_signal_connect(	GTK_OBJECT (widget), "configure_event",
					(GtkSignalFunc) stat_configure_event, NULL);
					
	gtk_widget_set_events(widget, GDK_EXPOSURE_MASK);
}



void Chart::Draw()
{
	int i;
	
	gdk_window_copy_area(stat_pixmap, widget->style->fg_gc[GTK_WIDGET_STATE(widget)], 0, 0,
		stat_pixmap, 1, 0, xc-1, yc);
	gdk_draw_rectangle(stat_pixmap, widget->style->black_gc, TRUE, 
		xc-1, 0, 1, yc);
	
	for (i = 0; i < VALUES - 1; i++) {
		if (graphdata[i].status) {
			gdk_draw_line(stat_pixmap, graphdata[i].context, xc-2, STAT_HEIGHT - 8 - (int) ((float) graphdata[i].y0 * scalefactor), 
							xc-1, STAT_HEIGHT - 8 - (int) ((float) graphdata[i].y1 * scalefactor) );
		}
	}

	if (graphdata[GENERATIONS].status) {
		gtk_label_set_text(GTK_LABEL(label), gens);
	}
		
	gdk_draw_pixmap(widget->window, widget->style->fg_gc[GTK_WIDGET_STATE(widget)], 
		stat_pixmap, 0,0, 0,0, xc, yc);
}


/**************************************************************/


void stats_population_handler(GtkWidget *widget, gpointer data)
{
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
		graphdata[CRITTERS].status = ENABLED;
	} else {
		graphdata[CRITTERS].status = DISABLED;
	}
}

void stats_herbivores_handler(GtkWidget *widget, gpointer data)
{
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
		graphdata[HERBS].status = ENABLED;
	} else {
		graphdata[HERBS].status = DISABLED;
	}
}

void stats_carnivores_handler(GtkWidget *widget, gpointer data)
{
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
		graphdata[CARNS].status = ENABLED;
	} else {
		graphdata[CARNS].status = DISABLED;
	}
}

void stats_omnivores_handler(GtkWidget *widget, gpointer data)
{
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
		graphdata[OMNIS].status = ENABLED;
	} else {
		graphdata[OMNIS].status = DISABLED;
	}
}

void stats_legs_handler(GtkWidget *widget, gpointer data)
{
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
		graphdata[LEGS].status = ENABLED;
	} else {
		graphdata[LEGS].status = DISABLED;
	}
}

void stats_fins_handler(GtkWidget *widget, gpointer data)
{
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
		graphdata[FINS].status = ENABLED;
	} else {
		graphdata[FINS].status = DISABLED;
	}
}

void stats_wings_handler(GtkWidget *widget, gpointer data)
{
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
		graphdata[WINGS].status = ENABLED;
	} else {
		graphdata[WINGS].status = DISABLED;
	}
}

void stats_segs_handler(GtkWidget *widget, gpointer data)
{
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
		graphdata[SEGS].status = ENABLED;
	} else {
		graphdata[SEGS].status = DISABLED;
	}
}

void stats_generations_handler(GtkWidget *widget, gpointer data)
{
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
		graphdata[GENERATIONS].status = ENABLED;
	} else {
		graphdata[GENERATIONS].status = DISABLED;
	}
}

void StatsWindow::SetColour(int i, guint32 c)
{
  	gdk_rgb_gc_set_foreground(graphdata[i].context, c);
	gdk_rgb_gc_set_background(graphdata[i].context, c);
}


void StatsWindow::SetScale(int size)
{
  scalefactor = ((float) (STAT_HEIGHT - 8) / (float) size);
}

StatsWindow::StatsWindow()
{
	int i;
	
	widget = gtk_window_new(GTK_WINDOW_DIALOG);
	gtk_window_set_title(GTK_WINDOW(widget), "Statistics");
	gtk_window_set_default_size(GTK_WINDOW(widget), STAT_WIDTH, STAT_HEIGHT + 5);
	gtk_container_set_border_width(GTK_CONTAINER(widget), 3);
	gtk_signal_connect( GTK_OBJECT(widget), "delete_event",	GTK_SIGNAL_FUNC(WindowKiller), NULL);

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_set_border_width(GTK_CONTAINER(vbox), 3);
	gtk_widget_show(vbox);

	// buttons and handlers	
	button = gtk_toggle_button_new_with_label("Population");
	gtk_signal_connect(GTK_OBJECT(button), "toggled", GTK_SIGNAL_FUNC(stats_population_handler), NULL);
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
	gtk_widget_show(button);
	
	button = gtk_toggle_button_new_with_label("Herbivores");
	gtk_signal_connect(GTK_OBJECT(button), "toggled", GTK_SIGNAL_FUNC(stats_herbivores_handler), NULL);
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
	gtk_widget_show(button);
	
	button = gtk_toggle_button_new_with_label("Carnivores");
	gtk_signal_connect(GTK_OBJECT(button), "toggled", GTK_SIGNAL_FUNC(stats_carnivores_handler), NULL);
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
	gtk_widget_show(button);

	button = gtk_toggle_button_new_with_label("Omnivores");
	gtk_signal_connect(GTK_OBJECT(button), "toggled", GTK_SIGNAL_FUNC(stats_omnivores_handler), NULL);
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
	gtk_widget_show(button);

	button = gtk_toggle_button_new_with_label("Legs");
	gtk_signal_connect(GTK_OBJECT(button), "toggled", GTK_SIGNAL_FUNC(stats_legs_handler), NULL);
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
	gtk_widget_show(button);

	button = gtk_toggle_button_new_with_label("Fins");
	gtk_signal_connect(GTK_OBJECT(button), "toggled", GTK_SIGNAL_FUNC(stats_fins_handler), NULL);
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
	gtk_widget_show(button);

	button = gtk_toggle_button_new_with_label("Wings");
	gtk_signal_connect(GTK_OBJECT(button), "toggled", GTK_SIGNAL_FUNC(stats_wings_handler), NULL);
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
	gtk_widget_show(button);

	button = gtk_toggle_button_new_with_label("Avg Segments");
	gtk_signal_connect(GTK_OBJECT(button), "toggled", GTK_SIGNAL_FUNC(stats_segs_handler), NULL);
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
	gtk_widget_show(button);

	button = gtk_toggle_button_new_with_label("Generations");
	gtk_signal_connect(GTK_OBJECT(button), "toggled", GTK_SIGNAL_FUNC(stats_generations_handler), NULL);
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
	gtk_widget_show(button);
	
	gens[0] = 0;
	label = gtk_label_new(gens);
	gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
	gtk_widget_show(label);
	
	hbox = gtk_hbox_new(FALSE, 0);
	gtk_container_set_border_width(GTK_CONTAINER(hbox), 3);
	gtk_widget_show(hbox);
	
	gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 0);
	
	// chart drawing area object
	chart = new Chart(STAT_WIDTH - 80, STAT_HEIGHT - 6);
	gtk_box_pack_start(GTK_BOX(hbox), chart->widget, FALSE, FALSE, 0);
	gtk_widget_show(chart->widget);
	
	gtk_container_add(GTK_CONTAINER(widget), hbox);
	gtk_widget_show(widget);

	for (i = 0; i < VALUES; i++) {
	  colours[i] = 0;
	}
	colours[CRITTERS] = C_CRIT;
	colours[CARNS] = c_carn;
	colours[HERBS] = c_herb;
	colours[OMNIS] = C_OMNI;
	colours[LEGS] = c_legs;
	colours[FINS] = c_fins;
	colours[WINGS] = c_wings;
	colours[SEGS] = c_segs;

	// initialise stats data values
	for (i = 0; i < VALUES; i++) {
		graphdata[i].y0 = 0;
		graphdata[i].y1 = 0;
		graphdata[i].context = gdk_gc_new(stat_pixmap);
		gdk_gc_copy(graphdata[i].context, chart->widget->style->white_gc);
		SetColour(i, colours[i]);
		graphdata[i].status = DISABLED;
	}
}


void StatsWindow::Show()
{
	gtk_widget_show(widget);
}


void StatsWindow::Hide()
{
	gtk_widget_hide(widget);
}


int StatsWindow::Status()
{
	return (GTK_WIDGET_FLAGS (widget) & GTK_VISIBLE);
}


void StatsWindow::SetData(int i, long int d)
{
	graphdata[i].y0 = graphdata[i].y1;
	graphdata[i].y1 = d;
}



void StatsWindow::Update(unsigned char *d, int len)
{
	int i = 0;
	
	while (i < len) {
		switch (d[i++]) {
		case GENERATIONS:
		  SetData(GENERATIONS, (d[i]<<24) | (d[i+1]<<16) | (d[i+2]<<8) | (d[i+3]) ); i += 4;
  		  sprintf(gens, "%09li", graphdata[GENERATIONS].y0);
		  break;
		case CARNS:
		  SetData(CARNS, (d[i]<<8) | (d[i+1]) ); i += 2; break;
		case HERBS:
		  SetData(HERBS, (d[i]<<8) + (d[i+1]) ); i += 2; break;
		case OMNIS:			
		  SetData(OMNIS, (d[i]<<8) | (d[i+1]) ); i += 2; break;
		case CRITTERS:		
		  SetData(CRITTERS, (d[i]<<8) | (d[i+1])); i += 2; break;
 		case LEGS:		
		  SetData(LEGS, (d[i]<<8) | (d[i+1])); i += 2; break;
 		case FINS:		
		  SetData(FINS, (d[i]<<8) | (d[i+1])); i += 2; break;
		case WINGS:		
		  SetData(WINGS, (d[i]<<8) | (d[i+1])); i += 2; break;
		case SEGS:		
		  SetData(SEGS, d[i]); i += 1; break;
		  graphdata[SEGS].y1 *= 20;
		  break;

		default:
		  g_print("StatsWindow::Update(): WARNING: invalid statistics data!\n");
		}
	}
}

void StatsWindow::Draw()
{
  chart->Draw();
}

void StatsWindow::SetDataStatus(int i, int s)
{
	graphdata[i].status = s;
}


int StatsWindow::DataIsEnabled(int i)
{
	return graphdata[i].status;
}

/**************************************************************/



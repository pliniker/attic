
TEMPLATE = subdirs

SUBDIRS = brainlib navigator gui-qt	plugins

isEmpty( PREFIX ) {
	error("usage: qmake PREFIX=/usr/local/thebrain/ [CONFIG+=debug]")
}

CONFIG_H = echo const char* CONFIG_PREFIX = \"$${PREFIX}\"\; > config.h
system( $${CONFIG_H} )


/****************************************************************************
** Form implementation generated from reading ui file 'src/thebrain-0.1/plugins/TextIO/form1.ui'
**
** Created: Wed May 30 21:50:22 2007
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/


#include "TextIO.h"
#include "TextIODialog.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qtextedit.h>
#include <qlistbox.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qstringlist.h>

/*
 *  Constructs a TextIODialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
TextIODialog::TextIODialog( TextIO* client, QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl ),
    m_client( client )
{
    if ( !name )
	setName( "TextIODialog" );
    setMinimumSize( QSize( 480, 320 ) );
    setPaletteBackgroundColor( QColor( 217, 211, 203 ) );
    TextIODialogLayout = new QGridLayout( this, 1, 1, 11, 6, "TextIODialogLayout"); 

    layout5 = new QVBoxLayout( 0, 7, 7, "layout5"); 

    layout1 = new QHBoxLayout( 0, 0, 6, "layout1"); 

    historyBox = new QGroupBox( this, "historyBox" );
    historyBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)5, 0, 0, historyBox->sizePolicy().hasHeightForWidth() ) );
    historyBox->setColumnLayout(0, Qt::Vertical );
    historyBox->layout()->setSpacing( 6 );
    historyBox->layout()->setMargin( 11 );
    historyBoxLayout = new QGridLayout( historyBox->layout() );
    historyBoxLayout->setAlignment( Qt::AlignTop );

    layout2 = new QHBoxLayout( 0, 0, 6, "layout2"); 

    formattedHistory = new QTextEdit( historyBox, "formattedHistory" );
    layout2->addWidget( formattedHistory );

    historyBoxLayout->addLayout( layout2, 0, 0 );
    layout1->addWidget( historyBox );

    dictionaryBox = new QGroupBox( this, "dictionaryBox" );
    dictionaryBox->setColumnLayout(0, Qt::Vertical );
    dictionaryBox->layout()->setSpacing( 6 );
    dictionaryBox->layout()->setMargin( 11 );
    dictionaryBoxLayout = new QGridLayout( dictionaryBox->layout() );
    dictionaryBoxLayout->setAlignment( Qt::AlignTop );

    dictionary = new QListBox( dictionaryBox, "dictionary" );
    dictionary->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)7, 0, 0, dictionary->sizePolicy().hasHeightForWidth() ) );
    dictionary->setMinimumSize( QSize( 100, 0 ) );
    dictionary->setMaximumSize( QSize( 150, 32767 ) );

    dictionaryBoxLayout->addWidget( dictionary, 0, 0 );
    layout1->addWidget( dictionaryBox );
    layout5->addLayout( layout1 );

    layout4 = new QHBoxLayout( 0, 0, 6, "layout4"); 

    userInputText = new QLineEdit( this, "userInputText" );
    layout4->addWidget( userInputText );

    userInputButton = new QPushButton( this, "userInputButton" );
    layout4->addWidget( userInputButton );
    layout5->addLayout( layout4 );

    TextIODialogLayout->addLayout( layout5, 0, 0 );
    languageChange();
    resize( QSize(674, 454).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( userInputText, SIGNAL( returnPressed() ), this, SLOT( userInputTextSlot() ) );
    connect( userInputButton, SIGNAL( clicked() ), this, SLOT( userInputTextSlot() ) );
    connect( dictionary, SIGNAL( clicked(QListBoxItem*) ), this, SLOT( addWordToTextSlot(QListBoxItem*) ) );

    m_userPara = 0;
    m_userIndex = 0;
    m_agentPara = 0;
    m_agentIndex = 0;
}


/*
 *  Destroys the object and frees any allocated resources
 */
TextIODialog::~TextIODialog()
{
    // no need to delete child widgets, Qt does it all for us
}


void TextIODialog::addWord( std::string word )
{
    QString w( word );
    dictionary->insertItem( w );
}


void TextIODialog::userSayWord( std::string word )
{
    if( word == "\n" )
    {
        formattedHistory->insertParagraph( "<font color=\"black\"></font>", -1 );
        m_agentPara = formattedHistory->paragraphs();
        m_agentIndex = 22;
    }
    else
    {
        QString str( word.c_str() );
        str += " ";
        formattedHistory->insertAt( str, m_agentPara, m_agentIndex );
        m_agentIndex += str.length();
    }
    
}


void TextIODialog::agentSayWord( std::string word )
{
    if( word == "\n" )
    {
        formattedHistory->insertParagraph( "<font color=\"red\"></font>", -1 );
        m_agentPara = formattedHistory->paragraphs();
        m_agentIndex = 20;
    }
    else
    {
        QString str( word.c_str() );
        str += " ";
        formattedHistory->insertAt( str, m_agentPara, m_agentIndex );
        m_agentIndex += str.length();
    }
}


void TextIODialog::addWordToTextSlot( QListBoxItem* item )
{
    QString what = item->text();
    QString sentence = userInputText->text();

    sentence += " ";
    sentence += what;

    userInputText->setText( sentence );
}


void TextIODialog::userInputTextSlot()
{
    QChar delimiter = ' ';
    QString source( userInputText->text() );
    QStringList words = QStringList::split( delimiter, source );

    wordlist_t output;

    for( QStringList::Iterator it = words.begin(); 
         it != words.end(); 
         ++it ) 
    {
        output.push_back( (*it).latin1() );
    }

    m_client->userInput( output );

    formattedHistory->insertParagraph( userInputText->text(), -1 );
    m_userPara = formattedHistory->paragraphs();
    m_userIndex = 0;

    userInputText->clear();
}


/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void TextIODialog::languageChange()
{
    setCaption( tr( "TextIO" ) );
    historyBox->setTitle( tr( "History" ) );
    dictionaryBox->setTitle( tr( "Dictionary" ) );
    dictionary->clear();
    userInputButton->setText( tr( "Speak entered text" ) );
}


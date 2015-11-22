/****************************************************************************
** Form interface generated from reading ui file 'src/thebrain-0.1/plugins/TextIO/form1.ui'
**
** Created: Wed May 30 21:50:02 2007
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.7   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef TEXTIODIALOG_H
#define TEXTIODIALOG_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QGroupBox;
class QTextEdit;
class QListBox;
class QListBoxItem;
class QLineEdit;
class QPushButton;

class TextIO;

class TextIODialog : public QDialog
{
    Q_OBJECT

public:
    TextIODialog( TextIO* client, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~TextIODialog();

    QGroupBox* historyBox;
    QTextEdit* formattedHistory;
    QGroupBox* dictionaryBox;
    QListBox* dictionary;
    QLineEdit* userInputText;
    QPushButton* userInputButton;

    void addWord( std::string word );

    void agentSayWord( std::string word );
    void userSayWord( std::string word );

public slots:
    virtual void userInputTextSlot();
    virtual void addWordToTextSlot( QListBoxItem* );

protected:
    QGridLayout* TextIODialogLayout;
    QVBoxLayout* layout5;
    QHBoxLayout* layout1;
    QGridLayout* historyBoxLayout;
    QHBoxLayout* layout2;
    QGridLayout* dictionaryBoxLayout;
    QHBoxLayout* layout4;

protected slots:
    virtual void languageChange();

private:
    TextIO*     m_client;
    
    int         m_userPara;
    int         m_userIndex;

    int         m_agentPara;
    int         m_agentIndex;
};

#endif // TEXTIODIALOG_H

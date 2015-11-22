#ifndef TEXTIO_H
#define TEXTIO_H

#include <string>
#include <map>
#include <list>
#include <NodeLayer.h>


typedef std::list<std::string> wordlist_t;


class TextIODialog;


class TextIO : public nnet::NodeLayer
{
    typedef std::map<std::string, size_t, std::less<std::string> > WordMap;
    typedef WordMap::iterator WordMap_it;

public:
    TextIO( std::string name );
    ~TextIO();

	std::string type();
    virtual void write( xmlNodePtr to ); 
    virtual void read( xmlNodePtr from );
    virtual bool check();

    virtual void recall( size_t neuron );
    virtual void learn( size_t neuron );

    void addWord( std::string word );
    void userInput( std::string word );
    void userInput( wordlist_t sentence );

    void setAgentLayer( nnet::NodeLayerPtr agent );


private:
    nnet::IntPtr                m_numWords;
    nnet::IntPtr                m_itersPerWord;

    nnet::NodeLayerPtr          m_inputAgent;

    WordMap                     m_words;

    TextIODialog*               m_dialog;
    wordlist_t                  m_sentence;
    WordMap_it                  m_it;

    size_t                      m_iterations;
    size_t                      m_multiplier;

public:
	static nnet::SerializablePtr Singleton( std::string name );
};


#endif


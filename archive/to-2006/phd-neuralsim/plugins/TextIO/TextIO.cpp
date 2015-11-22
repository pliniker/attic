
#include <XmlFile.h>

#include <Common.h>
#include <ErrorInterface.h>
#include <NodeLayer.h>
#include <Integer.h>

#include <qdialog.h>

#include "TextIODialog.h"
#include "TextIO.h"




TextIO::TextIO( std::string name ) :
    nnet::NodeLayer( name ),
    m_numWords( new nnet::Int( "numWords" ) ),
    m_itersPerWord( new nnet::Int( "iterationsPerWord" ) ),
    m_dialog( new TextIODialog( this ) )
{
    m_numWords->setPrivate( true );
    add( m_numWords );
        
    nnet::BooleanPtr isAsync( get( "isAsync" ) );
    isAsync->set( false );

    m_itersPerWord->setMin( 1 );
    m_itersPerWord->setMax( 1000 );
    m_itersPerWord->set( 1 );
    add( m_itersPerWord );

    m_dialog->show();
    addWord( "cat" );
    addWord( "dog" );
    addWord( "steve" );
    addWord( "bill" );
    addWord( "above" );
    addWord( "under" );
    addWord( "left" );
    addWord( "right" );
    addWord( "is" );
    addWord( "of" );
    addWord( "to" );
    addWord( "the" );
}


TextIO::~TextIO()
{
    m_dialog->hide();
    delete m_dialog;
}
	

std::string TextIO::type() 
{ 
    return "TextIO"; 
}

    
void TextIO::write( xmlNodePtr to ) 
{
    nnet::NodeLayer::write( to );

    for( WordMap_it i = m_words.begin(); i != m_words.end(); ++i ) 
    {
        std::ostringstream stringid;
        stringid << "word" << i->second;
        nnet::XmlFile::WriteString( to, stringid.str(), i->first );
    }
}


void TextIO::read( xmlNodePtr from ) 
{
    nnet::NodeLayer::read( from );

    m_numWords = get( "numWords" );
    m_itersPerWord = get( "iterationsPerWord" );

    if( from ) 
    {
        m_words.clear();

        for( int i = 0; i < m_numWords->get(); i++ ) 
        {
            std::ostringstream stringid;
            stringid << "word" << i;
            std::string value = nnet::XmlFile::ReadString( from, stringid.str() );
            m_words[ value ] = i;
        }
    }
}


bool TextIO::check()
{
    bool rv = true;

    m_iterations = 0;
    m_multiplier = m_itersPerWord->get();

    if( rv )
    {
        if( m_inputAgent.get() != NULL )
        {
            if( m_inputAgent->size() != m_neurons )
            {
                layerMessage( "Error: TextIO inputAgent has different dimensions" );
                rv = false;
            }
        }
        else
        {
            layerMessage( "Warning: TextIO has no inputAgent" );
        }
    }

    if( rv )
    {
        rv = nnet::NodeLayer::check();
    }

    return rv;
}


void TextIO::recall( size_t neuron )
{
    if( m_iterations == 0 )
    {
        if( !m_sentence.empty() )
        {
            m_it = m_words.find( m_sentence.front() );
            m_dialog->userSayWord( m_sentence.front() );
        }
        else
        {
            m_it = m_words.end();
            m_dialog->userSayWord( "\n" );
        }
    }

    if( m_it != m_words.end() &&
        m_it->second == neuron )
    {
        m_state[ neuron ] = 255;
    }
    else
    {
        m_state[ neuron ] = 0;
    }

    ++m_iterations;

    if( m_iterations == ( m_neurons * m_multiplier ) )
    {
        m_iterations = 0;

        if( !m_sentence.empty() )
        {
            m_sentence.pop_front();
        }

        if( m_inputAgent.get() != NULL )
        {
            bool validWord = false;
            nnet::Unit max = 0;

            WordMap_it w = m_words.begin();
            WordMap_it word = w;

            nnet::ImagePtr agentState( m_inputAgent->get( "state" ) );
            nnet::Unit* state = &agentState->at( 0 );
            for( size_t i = 0; i < m_neurons; ++i )
            {
                nnet::Unit thisVal = nnet::Common::HdistLookup[ state[i] ];

                if( thisVal > max )
                {
                    validWord = true;
                    max = thisVal;
                    word = w;
                }
                else if( thisVal == max )
                {
                    validWord = false;
                }

                ++w;
            }

            if( validWord )
            {
                m_dialog->agentSayWord( w->first );
            }
        }
    }
}


void TextIO::learn( size_t neuron )
{
    if( m_it != m_words.end() &&
        m_it->second == neuron )
    {
        m_state[ neuron ] = 255;
    }
    else
    {
        m_state[ neuron ] = 0;
    }
}


void TextIO::addWord( std::string word )
{
    m_words[ word ] = m_numWords->get();
    m_numWords->set( m_numWords->get() + 1 );
    m_dialog->addWord( word );
}


void TextIO::userInput( std::string word )
{
    m_sentence.push_back( word );
}


void TextIO::userInput( wordlist_t sentence )
{
    m_sentence = sentence;
}


void TextIO::setAgentLayer( nnet::NodeLayerPtr agent )
{
    m_inputAgent = agent;
}


nnet::SerializablePtr TextIO::Singleton( std::string name )
{
    static nnet::RefCountPtr<TextIO> textio;

    if( textio.get() == NULL )
    {
        textio = nnet::RefCountPtr<TextIO>( new TextIO( name ) );
    }

    return textio;
}


extern "C" void Initialize()
{
	nnet::Serializable::AddFactory( "TextIO", TextIO::Singleton );
	nnet::string::get_list( "node_layers" )->add( "TextIO" );
}


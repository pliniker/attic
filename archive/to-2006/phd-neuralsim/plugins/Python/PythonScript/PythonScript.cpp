

#ifdef __APPLE_CC__
#include <Python/Python.h>
#else
#include <Python.h>
#endif

#include <iostream>
#include <ErrorInterface.h>
#include <ExecEngine.h>
#include <../BaseTypes/FileName.h>


// NOTE:
//
// To get this to work, the PYTHONPATH environment variable must contain
// the path to nnet.so (e.g. /usr/local/thebrain/python/) and the path
// to any script you use. The easiest way to do the latter is to append
// '.' (current working directory) to PYTHONPATH.
//

class PythonScript : public nnet::ExecEngine 
{
public:
    PythonScript(std::string name) :
        nnet::ExecEngine ( name ),
        p_name( 0 ), 
        p_module( 0 ),
        p_init( 0 ),
        p_check( 0 ),
        p_recall( 0 ),
        p_learn( 0 ),
        script_path( new nnet::FileName( "script_path" ) ) 
    {
        add( script_path );
        valid_script = false;
    }


    std::string type() { return "PythonScript"; }
    
    
    void read( xmlNodePtr from ) 
    {
        nnet::Map::read( from );
        script_path = get( "script_path" );
    }
    
    
    void loadScript() 
    {
        if( valid_script ) 
        {
            Py_DECREF( p_module );
            Py_Finalize();
            valid_script = false;

            p_name      = 0; 
            p_module    = 0;
            p_init      = 0;
            p_check     = 0;
            p_recall    = 0;
            p_learn     = 0;
        }
    
        char oldpath[ 255 ];
        getcwd( oldpath, 255 );

        //         
        int slashpos        = script_path->get().rfind( "/" );
        std::string filenm  = script_path->get().substr( slashpos + 1 );
        std::string parent  = script_path->get().substr( 0, slashpos );
        int dotpos          = filenm.rfind( ".py" );
        std::string pymodu  = filenm.substr( 0, dotpos );
        
        if( chdir( parent.c_str() ) == -1 ) 
        {
            nnet::error::alert( "PythonScript trying to load script from non-existent path" );
            valid_script = false;
        }
        else 
        {
            std::string msg = "PythonScript loading " + pymodu;
            nnet::error::log( msg );

            valid_script = false;
        
            Py_Initialize();
            p_name   = PyString_FromString( pymodu.c_str() );
            p_module = PyImport_Import( p_name );
            Py_DECREF( p_name );

            if( p_module != NULL )
            {
                valid_script = true;
            }
            else
            {
                std::cerr << "p_module == NULL" << std::endl;
                PyErr_Print();
            }
            
            if( valid_script )
            {
                valid_script = false;
                p_init = PyObject_GetAttrString( p_module, "init" );
                if( p_init && PyCallable_Check( p_init ) )
                {
                    valid_script = true;
                }
                else
                {
                    std::cerr << "p_init && PyCallable_Check" << std::endl;
                    PyErr_Print();
                }
            }
            
            if( valid_script )
            {
                valid_script = false;
                p_check = PyObject_GetAttrString( p_module, "check" );
                if( p_check && PyCallable_Check( p_check ) )
                {
                    valid_script = true;
                }
                else
                {
                    std::cerr << "p_check && PyCallable_Check" << std::endl;
                    PyErr_Print();
                }
            } 

            if( valid_script )
            {
                valid_script = false;
                p_recall = PyObject_GetAttrString( p_module, "recall" );
                if( p_recall && PyCallable_Check( p_recall ) )
                {
                    valid_script = true;
                }
                else
                {
                    std::cerr << "p_recall && PyCallable_Check" << std::endl;
                    PyErr_Print();
                }
            } 

            if( valid_script )
            {
                valid_script = false;
                p_learn = PyObject_GetAttrString( p_module, "learn" );                
                if( p_learn  && PyCallable_Check( p_learn ) )
                {
                    valid_script = true;
                }
                else
                {
                    std::cerr << "p_learn && PyCallable_Check" << std::endl;
                    PyErr_Print();
                }
            }

            if( valid_script )
            {
                nnet::error::log( "PythonScript loaded OK" );
            }
            else
            {
                std::cerr << "ERROR in PythonScript: " << std::endl;
                nnet::error::alert( "Python script failed to load" );
                Py_Finalize();
                valid_script = false;
            }

            if( valid_script )
            {
                init();
            }
            
            chdir( oldpath );
        }
    }
    
    
    void init() 
    {
        try
        {
            callFunction( p_init );
        }
        catch( ... )
        {
            nnet::error::alert( "PythonScript ERROR in init, see console log" );
        }
    }

    
    void check() 
    {
        try
        {
            if( script_path->fileChanged() ) 
            {
                loadScript();
            }
            
            callFunction( p_check );
        }
        catch( ... )
        {
            nnet::error::alert( "PythonScript ERROR in check, see console log" );
        }
    }
    
    
    void recall() 
    {
        try
        {
            if( script_path->fileChanged() ) 
            {
                loadScript();
            }

            callFunction( p_recall );
        }
        catch( ... )
        {
            nnet::error::alert( "PythonScript ERROR in recall, see console log" );
        }
    }


    void learn() 
    {
        try
        {
            if( script_path->fileChanged() ) 
            {
                loadScript();
            }

            callFunction( p_learn );
        }
        catch( ... )
        {
            nnet::error::alert( "PythonScript ERROR in learn, see console log" );
        }
    }
    
    
    void callFunction( PyObject* function ) 
    {
        if( valid_script ) 
        {
            PyObject *p_args, *p_value;
    
            if( function && PyCallable_Check( function ) ) 
            {
                p_args  = PyTuple_New( 0 );
                p_value = PyObject_CallObject( function, p_args );
                Py_DECREF( p_args );

                if( p_value != NULL )
                {
                    Py_DECREF( p_value );
                }
            } 
            else 
            {
                if( PyErr_Occurred() )
                {
                    PyErr_Print();
                    nnet::error::alert( "A Python error occurred: see stdout" );
                }
                nnet::error::log( "PythonScript: cannot find function" );
            }
        } 
        else 
        {
            nnet::error::alert( "PythonScript: no script loaded" );
        }
    }
    
    
    ~PythonScript() 
    {
        if( valid_script ) 
        {
            Py_DECREF( p_module );
            Py_XDECREF( p_init );
            Py_XDECREF( p_check );
            Py_XDECREF( p_recall );
            Py_XDECREF( p_learn );
            Py_Finalize();
        }
    }
    
    
private:
    bool                valid_script;
    PyObject            *p_name, *p_module;
    PyObject            *p_recall, *p_learn, *p_init, *p_check;
    nnet::FileNamePtr   script_path;


public:
    static nnet::SerializablePtr Create( std::string name ) 
    {
        return nnet::RefCountPtr<PythonScript>( new PythonScript( name ) );
    }
};



extern "C" void Initialize() {
    nnet::Serializable::AddFactory( "PythonScript", PythonScript::Create );
    nnet::string::get_list( "exec_maps" )->add( "PythonScript" );
}


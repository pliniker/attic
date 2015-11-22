#ifndef BRAINLIB_ERRORINTERFACE_H
#define BRAINLIB_ERRORINTERFACE_H


#include <string>


namespace nnet 
{


#if defined(NDEBUG)
#define LOG_UNIMPL
#else
#define LOG_UNIMPL //{ std::cout << typeid(*this).name() << ":" << __FUNCTION__ << " UNIMPLEMENTED" << std::endl; }
#endif


    class ErrorInterfaceBase 
    {
		/// Object methods
    public:
        virtual ~ErrorInterfaceBase();
		
        virtual void alert( std::string err ) = 0;
        virtual void log( std::string err ) = 0;
		virtual bool ask( std::string err ) = 0;
		virtual void clear() = 0;
    };
	
	
	class ErrorInterface : public ErrorInterfaceBase 
    {
		/// Object methods
	public:
		void alert( std::string a );
		void log( std::string l );
		bool ask( std::string q );
		void clear();
    };
	
   
    // public error messages
    namespace error 
    {
        void set_error_interface( ErrorInterfaceBase* iface );
        void alert( std::string a );
        void log( std::string e );
        bool ask( std::string q );
        void clear();

        // generic error messages for anything under std::exception
        // 'in' is the name of the object that caused the exception
        void std_exception( std::string in, const char* what );

	    extern std::string const NewObjectFailed;
        extern std::string const DeleteObjectFailed;
        extern std::string const CheckObjectFailed;
        extern std::string const RecallObjectFailed;
        extern std::string const LearnObjectFailed;
        extern std::string const ResetObjectFailed;
        extern std::string const UnserializeFailed;
    }


} // end namespace


#endif

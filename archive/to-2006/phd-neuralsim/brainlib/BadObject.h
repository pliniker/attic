#ifndef BRAINLIB_BADOBJECT_H
#define BRAINLIB_BADOBJECT_H


#include <exception>
#include <string>
#include <iostream>
#include "ErrorInterface.h"
#include "Serializable.h"


namespace nnet {

	/** Exception class thrown by any and every class */
    class BadObject : public std::exception {
		// Object methods
    public:
        BadObject( SerializablePtr object, char const* wh );
		~BadObject() throw();

		/** What caused the exception.
		 *
		 * \return   string describing the error.
		 */
        char const*     what()    const throw();
		
		/** In what object did the exception originate.
		 *
		 * \return   RefCounted Serializable-derived object.
		 */
        SerializablePtr culprit() const throw();
		
		// Object data
    private:
		SerializablePtr     m_culprit;
		char const*         m_error;
        mutable std::string m_formattedError;
    };


#define LOG_EXCEPTION_E( e ) \
    { \
        std::cerr << __FILE__ << ":" << __FUNCTION__ << "(" << __LINE__ \
            << ") " << e.what() << std::endl; \
    }

#define LOG_EXCEPTION \
    { \
        std::cerr << __FILE__ << ":" << __FUNCTION__ << "(" << __LINE__ \
            << ") unknown exception!" << std::endl; \
    }



}// end namespace


#endif

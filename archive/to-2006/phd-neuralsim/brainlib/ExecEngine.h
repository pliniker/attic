#ifndef BRAINLIB_EXECENGINE_H
#define BRAINLIB_EXECENGINE_H


#include "Map.h"


namespace nnet {

	
	class ExecEngine : public Map {
		/// Object methods
	public:
		ExecEngine( std::string name );
				
		virtual void recall();
		virtual void learn();
	};

	
	typedef RefCountPtr<ExecEngine> ExecEnginePtr;
	
}


#endif


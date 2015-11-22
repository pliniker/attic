

#include <string>
#include <vector>
#include <list>
#include <ErrorInterface.h>
#include <Integer.h>
#include <Boolean.h>
#include <RandomInt.h>
#include <Common.h>
#include <NeuronArray.h>
#include "Pattern.h"


using namespace brainlib;


class VGRAM : public NeuronArray {
	
    typedef std::list<Pattern*> Patterns;
    typedef Patterns::iterator PatternsIt;

	typedef std::vector<Patterns> PatternList;
	typedef PatternList::iterator PatternListIt;


public:
	VGRAM(std::string name) : 
	NeuronArray   ( name                        ),
	randomOutput  ( new Boolean("randomOutput") ),
	generalization( new Int("generalization")   ) {
		
		randomOutput->set(true);
		
		generalization->set(100);
		generalization->setMin(0);
		generalization->setMax(100);
		generalization->setStep(1);
		
		add(randomOutput);
		add(generalization);
		
		general           = 0;
		randOut           = 0;
		synapsesPerNeuron = 0;
	}
	
	
	std::string type() { return "VGRAM"; }

	
	void read(xmlNodePtr from) {
		NeuronArray::read(from);
		
		randomOutput   = get("randomOutput");
		generalization = get("generalization");
	}
	
	
	void create(unsigned int array_size) {
		reset();
		patternList.reserve(array_size);
		size->set(array_size);
	}

	
	bool check() {
		general     =       generalization->get();
		randOut     =       randomOutput->get();
		maxResponse =       Common::MaxResponse( bits->get() );
		maxDist     =       bits->get() * inputs->get() + 1;
		synapsesPerNeuron = inputs->get();
		
		return true;
	}


    Unit recall(unsigned int neuron, Unit* pattern) {
		
		Patterns& patterns = patternList[neuron];
		
        if ( !patterns.empty() ) {

            int dist;
            int numInputs = inputs->get();
            int output = 0;
            bool defined = false;

            for (PatternsIt i = patterns.begin(); i != patterns.end(); i++) {
                dist = 0;
                int j = numInputs - 1;
                Pattern* learnt = *i;

                while ( (dist += Common::HdistLookup[ pattern[j] ^ learnt->get(j) ]
                         <= maxDist) && j-- ) {}

                if (j == 0) { // the pattern is less distant or equidistant
                    if (dist < maxDist) {
                        maxDist = dist;
                        output = learnt->response();
                        defined = true;
                    } else if ( (dist == maxDist) &&
                                (output != learnt->response()) ) {
                        defined = false;
                    }
                }
            }

            if (defined) {
                return output;
            }
        }

        if (randOut) {
            return RandomInt::Get() & maxResponse;
        } else {
            return 0;
        }
    }


    void learn(unsigned int neuron, Unit* pattern, Unit response) {
		Patterns& patterns = patternList[neuron];
        Pattern* p = new Pattern(pattern, response, synapsesPerNeuron);
        patterns.push_back(p);
    }


    void reset() {
		for (PatternListIt j = patternList.begin(); j != patternList.end(); j++) {
			Patterns& patterns = *j;
			for (PatternsIt i = patterns.begin(); i != patterns.end(); i++) {
				delete *i;
			}
			patterns.clear();
		}
    }
	
	
    ~VGRAM() {
        reset();
    }

	
private:
	BooleanPtr  randomOutput;
	IntPtr      generalization;
	
	bool        randOut;
	int         general;
	int         synapsesPerNeuron;
	int         maxResponse;
	int         maxDist;
	
    PatternList patternList;
	
	
public:
	static SerializablePtr Create(std::string name) {
		return RefCountPtr<VGRAM>( new VGRAM(name) );
	}
};


extern "C" void Initialize() {
	Serializable::AddFactory("VGRAM", VGRAM::Create);
	StringList::Get("Neuron Arrays")->add("VGRAM");
}


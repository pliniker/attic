

#include <math.h>
#include <NodeLayer.h>
#include <Array.h>
#include <Operations.h>
#include <RandomInt.h>
#include <ErrorInterface.h>


using namespace brainlib;


class DigitalSOM : public NodeLayer {
public:
    DigitalSOM(std::string name) :
    NodeLayer  ( name                           ),
	weights    ( new FloatArray("weights")      ),
	outputs    ( new FloatArray("outputs")      ),
	dist       ( new Float("start_dist")        ),
	dist_rate  ( new Float("dist_decay_rate")   ),
	alpha      ( new Float("alpha")             ),
	alpha_rate ( new Float("alpha_decay_rate")  ) {
		add(weights);
		add(outputs);
		add(dist);
		add(dist_rate);
		add(alpha);
		add(alpha_rate);
		
		dist->set( ( outputState->x() + outputState->y() ) / 2 );
		dist_rate->set(0.90);
		alpha->set(1.0);
		alpha_rate->set(0.98);
		
		output_size         = 0;                     
		synapses_per_neuron = 0;
	}
	
	
	std::string type() { return "DigitalSOM"; }
	
	
	void read(xmlNodePtr from) {
		NodeLayer::read(from);
		weights    = get("weights");
		outputs    = get("outputs");
		dist       = get("start_dist");
		dist_rate  = get("dist_decay_rate");
		alpha      = get("alpha");
		alpha_rate = get("alpha_decay_rate");
	}
	

	bool check() {
		if ( NodeLayer::check() ) {                
		
			if ( inputs->get() == 0 ) {
                layerMessage("layer should have at least one input");
				return false;
			}
			
			if ( (outputState->size() != output_size       ) ||
				 (inputs->get()       != synapses_per_neuron) ) {
				output_size = outputState->size();
				synapses_per_neuron = inputs->get();
				weights->create( inputState->size() );
				outputs->create( outputState->size() );
				reset();
			}
						
			return true;
		} else {
			return false;
		}
	}
	

    void recall() {
		int step = synapses_per_neuron;        
		int size = outputState->size();
		
		for (int i = 0; i < size; i++) {
			int offset = i * step;
			outputs->at(i) = 0;
			for (int j = 0; j < step; j++) {
				outputs->at(i) += (float)Common::HdistLookup[ inputState->at(offset + j) ] / 8.0 * weights->at(offset + j);
			}
		}
		
		winner_pos = 0;
		float max = 100000.0;
		for (int i = 0; i < size; i++) {
			float out = outputs->at(i);
			if ( out < max ) {
				max = out;
				winner_pos = i;
			}
			outputState->at(i) = 0;
		}
		
		outputState->at(winner_pos) = Common::UnitMax;
    }

	
    void learn() {
		// hopefully winner_pos still set from recall...
        float T   = dist->get();
        float A   = alpha->get();
        float dT  = dist_rate->get();
        float dA  = alpha_rate->get();
		
		int o_x  = outputState->x();
		int o_y  = outputState->y();
		
        int winner_y = winner_pos / o_x;
        int winner_x = winner_pos % o_x;

        // Wv(t + 1) = Wv(t) + Θ(t)α(t)(D(t) - Wv(t))
        for (int j = 0; j < o_y; j++) {
            for (int i = 0; i < o_x; i++) {
                int n = j * o_x + i;
                float Th = (float)sqrt( (winner_y-j)*(winner_y-j) + (winner_x-i)*(winner_x-i) ) / T;
                if (Th < 1.0) {
                    for (int k = 0; k < synapses_per_neuron; k++) {
                        weights->at(n + k) += Th*A*( (float)Common::HdistLookup[ inputState->at(n + k) ]/8.0 - weights->at(n + k) );
                    }
                }
            }
        }
        
        dist->set( T * dT );
        alpha->set( A * dA );
	}
	
	
	void reset() {
		int size = weights->size();
		for (int i = 0; i < size; i++) {
			weights->at(i) = (float)RandomInt::Get() / (float)RandomInt::Max;
		}
	}
	
	
    ~DigitalSOM() {}
	
	
private:
	FloatArrayPtr   weights;
	FloatArrayPtr   outputs;
	FloatPtr        dist;
	FloatPtr        dist_rate;
	FloatPtr        alpha;
	FloatPtr        alpha_rate;
	
	int             output_size;                    
	int				synapses_per_neuron;
	int				winner_pos;

public:
	static SerializablePtr Create(std::string name) {
		return RefCountPtr<DigitalSOM>( new DigitalSOM(name) );
	}
};


extern "C" void Initialize() {
	Serializable::AddFactory("DigitalSOM", DigitalSOM::Create);
	StringList::Get("node_layers")->add("DigitalSOM");
}


/*

===================================================

Perceptron Version 1.0 Copyright Peter Liniker 1997

===================================================

Simple single layer version of Frank Rosenblatt's original
Perceptron neural network created in the 1950s.

How to use this program:

	first reset all the neurons so that none are defined
		reset_neurons();

	next specify how many inputs & outputs you want
		define_neurons(inputs,outputs);

	then to train the network do for each set of inputs:
		inputs[?] = ?;
		correct_outputs[?] = ?;
		train_neurons();

	to output the result from any inputs then
		calc_outputs();

	and to display information
		display_input_output();

NOTE! this neural architecture only works for linear relationships
	 between the inputs and the outputs. eg. XOR won't work.
	 Inputs and outputs can only be 0 or 1.

*/

/****************************************************************************/

#include <stdio.h>

/****************************************************************************/

/* There are three possible layers : input, buffer and output.
 Buffer and input are not really necessary for this simple program. */
#define out_neuron 1

/* maximum number of possible neurons - variable */
#define _no_neurons 16

/* maximum number of inputs and outputs - variable */
#define _no_inputs 16
#define _no_outputs 16

/* This defines a neuron -
 type in,out,buffer
 input: the cross product of the weights & inputs = net
 output: f(net) = the input after the activation function
 weights - one for the output of every neuron of the previous layer */
struct _neuron {
	int type;
	int output;
	int input;
	int weights[_no_inputs];
	} neuron[_no_neurons];

/* input information */
int inputs[_no_inputs],no_inputs,current_input;

/* output information  */
int outputs[_no_outputs],no_outputs,current_output;

/* information about the number of each type of neuron */
int input_neurons,output_neurons,current_neuron;

/* learning variables */
int correct_outputs[_no_outputs];
int alpha = 1;

/****************************************************************************/

/* reset all the neurons and inputs to zero. ie no defined neurons.
 current_neuron = 0, and no inputs are present. */

void reset_neurons()
{
	int counter;

	current_neuron = 0;
	while (current_neuron != _no_neurons) {
		neuron[current_neuron].type = 0;
		neuron[current_neuron].output = 0;
		neuron[current_neuron].input = 0;

		counter = 0;
		while (counter != _no_inputs) {
			neuron[current_input].weights[counter] = 0;
			counter++;
			}

		current_neuron++;
		}

	current_input = 0;
	while (current_input != _no_inputs) {
		inputs[current_input] = 1;
		current_input++;
		}

	current_output = 0;
	while (current_output != no_outputs) {
		outputs[current_output] = 0;
		current_output++;
		}

	current_neuron = 0;
	current_input = 0;
	current_output = 0;
	input_neurons = 0;
	output_neurons = 0;

	printf("Neurons reset.\r\n\n");
}

/****************************************************************************/

/* defines a given number of each type of neuron in the structure
 eg define_neurons(2,0,1); for 2 input neurons, 0 buffer neurons
 and 1 output neuron. */

void define_neurons(int inputs,int no_neurons)
{
	int counter;

	no_inputs = inputs;
	no_outputs = no_neurons;

	counter = 0;
	while (counter != no_neurons) {
		neuron[current_neuron].type = out_neuron;
		output_neurons++;
		current_neuron++;
		counter++;
		}
}

/****************************************************************************/

/* calculate the neural network output(s)
 assuming inputs have been given. */

void calc_outputs()
{
	int net_sum;

	current_neuron = 0;
	current_output = 0;
	while (current_neuron != _no_neurons) {

		if (neuron[current_neuron].type == out_neuron) {

			net_sum = neuron[current_neuron].weights[no_inputs];
			current_input = 0;
			while (current_input != no_inputs) {
				net_sum += (neuron[current_neuron].weights[current_input]
					* inputs[current_input]);
				current_input++;
				}

			neuron[current_neuron].input = net_sum;
			if (net_sum >  0) { neuron[current_neuron].output = 1; }
			else { neuron[current_neuron].output = 0; }
			outputs[current_output] = neuron[current_neuron].output;

			}

		current_output++;
		current_neuron++;
		}
}

/****************************************************************************/

/* supervised training : calculate the weights from the
 correct outputs & the inputs. */

void calc_weights()
{
	int d_weight;

	current_neuron = 0;
	while (current_neuron != _no_neurons) {

		if (neuron[current_neuron].type == out_neuron) {

			current_input = 0;
			while (current_input != no_inputs+1) {
				d_weight = alpha * (correct_outputs[current_neuron] - outputs[current_neuron]);
				neuron[current_neuron].weights[current_input] += (d_weight * inputs[current_input]);
				current_input++;
				}

			}
		current_neuron++;
		}
}

/****************************************************************************/


/* displays the output array on the screen */

void display_inputs_outputs()
{

	printf("INPUTS: \t");
	current_input = 0;
	while (current_input != no_inputs+1) {
		printf("%i ",inputs[current_input]);
		current_input++;
		}

	printf("\r\nWEIGHTS: ");
	current_neuron = 0;
	while (current_neuron != _no_neurons) {
		if (neuron[current_neuron].type == out_neuron) {
			printf("\t(%i) ",current_neuron);
			current_input = 0;
			while (current_input != no_inputs+1) {
				printf("%i ",neuron[current_neuron].weights[current_input]);
				current_input++;
				}
			}
		current_neuron++;
		}
	printf("\r\n");

	printf("OUTPUTS: ");
	current_output = 0;
	while (current_output != no_outputs) {
		printf("\t(%i) %i->%i",current_output,outputs[current_output],
			correct_outputs[current_output]);
		current_output++;
		}
	printf("\r\n\n");
}


/****************************************************************************/

void train_neurons()
{
	/* calculate the outputs */
	calc_outputs();

	/* calculate new weights  */
	calc_weights();

	/* display the outputs on the screen */
	display_inputs_outputs();
}

/****************************************************************************/

/* Main program block */

int main()
{
	int counter;

	/* set program to zero neurons */
	reset_neurons();

	/* define 4 input and 2 output neurons */
	define_neurons(2,4);


	/* train the logical AND, OR, NAND and NOR functions */

	counter = 16;
	while (counter != 0) {

	/*  00 - 0 */
		correct_outputs[0] = 0;
		correct_outputs[1] = 0;
		correct_outputs[2] = 1;
		correct_outputs[3] = 1;
		inputs[0] = 0;
		inputs[1] = 0;

		train_neurons();

	/* 01 - 0 */
		correct_outputs[0] = 0;
		correct_outputs[1] = 1;
		correct_outputs[2] = 1;
		correct_outputs[3] = 0;
		inputs[0] = 0;
		inputs[1] = 1;

		train_neurons();

	/* 10 - 0 */
		correct_outputs[0] = 0;
		correct_outputs[1] = 1;
		correct_outputs[2] = 1;
		correct_outputs[3] = 0;
		inputs[0] = 1;
		inputs[1] = 0;

		train_neurons();

	/* 11 - 1 */
		correct_outputs[0] = 1;
		correct_outputs[1] = 1;
		correct_outputs[2] = 0;
		correct_outputs[3] = 0;
		inputs[0] = 1;
		inputs[1] = 1;

		train_neurons();

		counter--;
		}


	printf("Perceptron execution ended.\n\r");
	return 0;
}

/****************************************************************************/
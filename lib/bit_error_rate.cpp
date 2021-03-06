#include <algorithm>
#include <complex>

#include "netxpto.h"
#include "bit_error_rate.h"

void BitErrorRate::initialize(void){
	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
}


bool BitErrorRate::runBlock(void){
	int ready1 = inputSignals[0]->ready();
	int ready2 = inputSignals[1]->ready();
	int ready = min(ready1, ready2);
	int space = outputSignals[0]->space();

	int process = min(ready, space);



	if (process == 0)
	{

		t_real NumberOfBits = recievedbits;
		t_real Coincidences = coincidences;

		t_real BER;
		BER = (NumberOfBits - Coincidences) / NumberOfBits;

		t_real UpperBound = BER + 1 / sqrt(NumberOfBits) * z  * sqrt(BER*(1 - BER)) + 1 / (3 * NumberOfBits)*(2 * z * z * (1 / 2 - BER) + (2 - BER));
		t_real LowerBound = BER - 1 / sqrt(NumberOfBits) * z  * sqrt(BER*(1 - BER)) + 1 / (3 * NumberOfBits)*(2 * z * z * (1 / 2 - BER) - (1 + BER));

		/* Outputting a .txt report*/
		ofstream myfile;
		myfile.open("BER.txt");
		myfile << "BER=" << BER << "\n";
		myfile << "Upper and Lower Confidence bounds for stipulated interval \n";
		myfile << "Upper Bound=" << UpperBound << "\n";
		myfile << "Lower Bound=" << LowerBound << "\n";
		myfile << "Number of recieved bits =" << NumberOfBits << "\n";
		myfile.close();
		return false;
	}



	for (long int i = 0; i < process; i++) {

		t_binary signalValue;
		inputSignals[0]->bufferGet(&signalValue);
		t_binary SignalValue;
		inputSignals[1]->bufferGet(&SignalValue);

		recievedbits++;

		if (signalValue == SignalValue)
		{
			coincidences++;
			outputSignals[0]->bufferPut(1);
		}
		else
		{
			outputSignals[0]->bufferPut(0);
		}




	}
	return true;

}
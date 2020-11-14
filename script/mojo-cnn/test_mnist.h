#include <iostream> // cout
#include <vector>
#include <sstream>
#include <fstream>
#include <stdio.h>
//#include <tchar.h>

#include "mojo.h"

//*
#include "mnist_parser.h"
using namespace mnist;
std::string data_path = "./";
std::string model_file = "./MojoCNN/models/mnist_deepcnet.mojo";

void test(mojo::network &cnn, const std::vector<std::vector<float>> &test_images, const std::vector<int> &test_labels)
{
	int out_size = cnn.out_size(); // we know this to be 10 for MNIST and CIFAR
	int correct_predictions = 0;

	// use progress object for simple timing and status updating
	mojo::progress progress((int)test_images.size(), "  testing : ");

	const int record_cnt = (int)test_images.size();

	// when MOJO_OMP is defined, we use standard "omp parallel for" loop, 
	// the number of threads determined by network.enable_external_threads() call
#pragma omp parallel for reduction(+:correct_predictions) schedule(dynamic)  // dynamic schedule just helps the progress class to work correcly
	for (int k = 0; k < record_cnt; k++)
	{
		// predict_class returnes the output index of the highest response
		const int prediction = cnn.predict_class(test_images[k].data());
		if (prediction == test_labels[k]) correct_predictions++;
		if (k % 1000 == 0) progress.draw_progress(k);
	}
	float dt = progress.elapsed_seconds();
	std::cout << "  test time: " << dt << " seconds                                          " << std::endl;
	std::cout << "  records: " << test_images.size() << std::endl;
	std::cout << "  speed: " << (float)record_cnt / dt << " records/second" << std::endl;
	std::cout << "  accuracy: " << (float)correct_predictions / record_cnt * 100.f << "%" << std::endl;
}

std::string fuck(mojo::network &cnn, const std::vector<std::vector<float>> &test_images)
{
	std::string result;
	int out_size = cnn.out_size(); // we know this to be 10 for MNIST and CIFAR
	int correct_predictions = 0;

	// use progress object for simple timing and status updating
	mojo::progress progress((int)test_images.size(), "  testing : ");

	const int record_cnt = (int)test_images.size();
	for (int k = 0; k < record_cnt; k++)
	{
		// predict_class returnes the output index of the highest response
		const int prediction = cnn.predict_class(test_images[k].data());
		if (prediction >= 0) {
			if (prediction <= 9)		result += (prediction + '0' - 0);
			else if (prediction <= 35)	result += (prediction + 'A' - 10);
			else if (prediction <= 61)	result += (prediction + 'a' - 36);
			else						result += "?";
		}
		else							result += '?';
	}
	float dt = progress.elapsed_seconds();
	std::cout << "  test time: " << dt << " seconds                                          " << std::endl;
	std::cout << "  records: " << test_images.size() << std::endl;
	std::cout << "  speed: " << (float)record_cnt / dt << " records/second" << std::endl;
	return result;
}

/**/
int main_test_mnist()
{
	// == parse data
	// array to hold image data (note that mojo does not require use of std::vector)
	std::vector<std::vector<float>> test_images;
	// array to hold image labels 
	std::vector<int> test_labels;
	// calls MNIST::parse_test_data  or  CIFAR10::parse_test_data depending on 'using'
	//if (!parse_test_data(data_path, test_images, test_labels)) { std::cerr << "error: could not parse data.\n"; return 1; }
	if (!getData(data_path, test_images, test_labels)) {
		std::cerr << "error: could not parse data.\n"; return 1;
	}
	
	//for (int i = 0; i < test_labels.size(); i++)std::cout << test_labels[i] << " ";std::cout << "***\n";
	
	// == setup the network  
	mojo::network cnn;

	// here we need to prepare mojo cnn to store data from multiple threads
	// !! enable_external_threads must be set prior to loading or creating a model !!
	cnn.enable_external_threads();

	// load model
	if (!cnn.read(model_file)) { std::cerr << "error: could not read model.\n"; return 1; }
	std::cout << "Mojo CNN Configuration:" << std::endl;
	std::cout << cnn.get_configuration() << std::endl << std::endl;

	// == run the test
	std::cout << "Testing " << data_name() << ":" << std::endl;
	// this function will loop through all images, call predict, and print out stats
	test(cnn, test_images, test_labels);

	std::cout << std::endl;
	return 0;
}
/**/

std::string recognize_in_test_mnist_h(const std::vector<std::vector<float>> &test_images) {
	mojo::network cnn;
	cnn.enable_external_threads();
	if (!cnn.read(model_file)) { std::cerr << "error: could not read model.\n"; return "error"; }
	std::cout << "Mojo CNN Configuration:" << std::endl;
	std::cout << cnn.get_configuration() << std::endl << std::endl;

	// == run the test
	std::cout << "Testing " << data_name() << ":" << std::endl;
	// this function will loop through all images, call predict, and print out stats
	return fuck(cnn, test_images);
}


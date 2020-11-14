#pragma once
#include <iostream> // cout
#include <vector>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int ArrayIndexNew[400000];
void random(int a[], int n)
{
	int index, tmp, i;
	srand(time(NULL));
	for (i = n - 1; i > 0; i--)
	{
		index = rand() % i;
		tmp = a[i];
		a[i] = a[index];
		a[index] = tmp;
	}

}

#include "mojo.h"
#include "util.h"
#include "mnist_parser.h"

const int mini_batch_size = 24;
const float initial_learning_rate = 0.04f;
std::string solver = "adam";
std::string data_path = "./";
std::string model_file = "./MojoCNN/models/mnist_deepcnet.mojo";
using namespace mnist;
float test(mojo::network &cnn,
	const std::vector<std::vector<float>> &test_images,
	const std::vector<int> &test_labels) {
	mojo::progress progress((int)test_images.size(), "  testing:\t\t");
	int out_size = cnn.out_size(); // we know this to be 10 for MNIST
	int correct_predictions = 0;
	const int record_cnt = (int)test_images.size();
#pragma omp parallel for reduction(+:correct_predictions) schedule(dynamic)
	for (int k = 0; k < record_cnt; k++)
	{
		const int prediction = cnn.predict_class(test_images[k].data());
		if (prediction == test_labels[k]) correct_predictions += 1;
		if (k % 1000 == 0) progress.draw_progress(k);
	}
	float accuracy = (float)correct_predictions / record_cnt * 100.f;
	return accuracy;
}
static std::vector<std::vector<float>> test_images;
static std::vector<int> test_labels;
static std::vector<std::vector<float>> train_images;
static std::vector<int> train_labels;
/**/
int main_train_mnist()
{
	if (!getData(data_path, test_images, test_labels)) {
		std::cerr << "error: could not parse data.\n"; return 1; 
	}
	std::cout << "测试数据装载完成！\n";
	int mylength = test_images.size();
	for (int i = 0; i < mylength; i++)
		ArrayIndexNew[i] = i;
	for (int ii = 0; ii < 100; ii++)//打乱100次应该够乱的了吧
		random(ArrayIndexNew, mylength);
	int countt = 0;
	for (int ii = 0; ii < mylength; ii++) {
		countt++;
		train_images.push_back(test_images[ArrayIndexNew[ii]]);
		train_labels.push_back(test_labels[ArrayIndexNew[ii]]);
	}
	std::cout << "共有" << countt << "条数据" << std::endl;
	std::cout << "训练数据装载完成！\n";


	// ==== setup the network  - when you train you must specify an optimizer ("sgd", "rmsprop", "adagrad", "adam")
	mojo::network cnn(solver.c_str());
	// !! the threading must be enabled with thread count prior to loading or creating a model !!
	cnn.enable_external_threads();
	cnn.set_mini_batch_size(mini_batch_size);
	cnn.set_smart_training(true); // automate training
	cnn.set_learning_rate(initial_learning_rate);

	// Note, network descriptions can be read from a text file with similar format to the API
	cnn.read("./MojoCNN/models/mnist_quickstart.txt");

	std::cout << "==  Network Configuration  ====================================================" << std::endl;
	std::cout << cnn.get_configuration() << std::endl;

	// add headers for table of values we want to log out
	mojo::html_log log;
	log.set_table_header("epoch\ttest accuracy(%)\testimated accuracy(%)\tepoch time(s)\ttotal time(s)\tlearn rate\tmodel");
	log.set_note(cnn.get_configuration());

	// augment data random shifts only
	cnn.set_random_augmentation(1, 1, 0, 0, mojo::edge);
	//*cnn.read(model_file);
	// setup timer/progress for overall training
	mojo::progress overall_progress(-1, "  overall:\t\t");
	const int train_samples = (int)train_images.size();
	float old_accuracy = 0;
	while (1) {
		overall_progress.draw_header(data_name() + "  Epoch  " + std::to_string((long long)cnn.get_epoch() + 1), true);
		mojo::progress progress(train_samples, "  training:\t\t");
		cnn.start_epoch("cross_entropy");
		// manually loop through data. batches are handled internally. if data is to be shuffled, the must be performed externally
#pragma omp parallel for schedule(dynamic)  // schedule dynamic to help make progress bar work correctly
		for (int k = 0; k < train_samples; k++)
		{
			cnn.train_class(train_images[k].data(), train_labels[k]);
			if (k % 1000 == 0) progress.draw_progress(k);
		}

		// draw weights of main convolution layers
		cnn.end_epoch();
		float dt = progress.elapsed_seconds();
		std::cout << "  mini batch:\t\t" << mini_batch_size << "                               " << std::endl;
		std::cout << "  training time:\t" << dt << " seconds on " << cnn.get_thread_count() << " threads" << std::endl;
		std::cout << "  model updates:\t" << cnn.train_updates << " (" << (int)(100.f*(1. - (float)cnn.train_skipped / cnn.train_samples)) << "% of records)" << std::endl;
		std::cout << "  estimated accuracy:\t" << cnn.estimated_accuracy << "%" << std::endl;
		
		// ==== run testing set
		progress.reset((int)test_images.size(), "  testing out-of-sample:\t");
		float accuracy = test(cnn, test_images, test_labels);
		std::cout << "  test accuracy:\t" << accuracy << "% (" << 100.f - accuracy << "% error)      " << std::endl;

		// if accuracy is improving, reset the training logic that may be thinking about quitting
		if (accuracy > old_accuracy) {
			cnn.reset_smart_training();
			old_accuracy = accuracy;
		}

		// save model
		std::string model_file = "./MojoCNN/models/snapshots/tmp_" + std::to_string((long long)cnn.get_epoch()) + ".txt";
		cnn.write(model_file, true);
		std::cout << "  saved model:\t\t" << model_file << std::endl << std::endl;

		// write log file
		std::string log_out;
		log_out += float2str(dt) + "\t";
		log_out += float2str(overall_progress.elapsed_seconds()) + "\t";
		log_out += float2str(cnn.get_learning_rate()) + "\t";
		log_out += model_file;
		log.add_table_row(cnn.estimated_accuracy, accuracy, log_out);
		// will write this every epoch
		log.write("./MojoCNN/models/snapshots/mojo_mnist_log.htm");

		// can't seem to improve
		if (cnn.elvis_left_the_building()) {
			std::cout << "Elvis just left the building. No further improvement in training found.\nStopping.." << std::endl;
			break;
		}

	};
	std::cout << std::endl;
	return 0;
}
/**/

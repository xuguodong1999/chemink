#ifndef PATTERNRECOGNITION_H
#define PATTERNRECOGNITION_H
#include "mojo.h"
static mojo::network cnn;
void initCNN() {
	DCinit(); cout << "Dictionary has been loaded successfully!" << endl;
	cnn.enable_external_threads();
	cnn.superRead(); cout << "CNN Model has been loaded successfully!" << endl;
	//cnn.get_configuration();
}
std::string recognize_pattern_mode(
	const std::vector<std::vector<float>> &test_images
) {
	std::string result; int len;
	const int record_cnt = (int)test_images.size();
	for (int k = 0; k < record_cnt; k++) {
		//std::cout << "test_images[k] = " << test_images[k].size() << std::endl;
		len = result.length(); _important_string.clear();
		int j = _location_of_prefix.size() - 1;
		//std::cout << "断点位置列表：";
		//for (int g = 0; g < _location_of_prefix.size(); g++)
		//	cout << _location_of_prefix.at(g) << ", ";
		//cout << endl;
		while (j >= 0 && _location_of_prefix.at(j) > len)j--;
		if (j >= 0){
			for (int i = _location_of_prefix.at(j); i < len; i++)
				_important_string += result[i];
		}
		//std::cout << "_important_string = \"" << _important_string << "\"" << std::endl;
		const int prediction = cnn.predict_class(test_images[k].data());
		if (prediction >= 0) {
			if (prediction <= 9)		result += (prediction + '0' - 0);
			else if (prediction <= 35)	result += (prediction + 'A' - 10);
			else if (prediction <= 61)	result += (prediction + 'a' - 36);
			else						result += "?";
		}
		else							result += '?';
	}
	//std::cout << "  records: " << test_images.size() << std::endl;
	return result;
}
#endif // !PATTERNRECOGNITION_H


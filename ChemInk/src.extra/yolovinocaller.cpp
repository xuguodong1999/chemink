#include "yolovinocaller.h"
//// Copyright (C) 2018-2019 Intel Corporation
//// SPDX-License-Identifier: Apache-2.0
////

//  以下代码出自open_model_zoo

/**
* \brief The entry point for the Inference Engine object_detection demo application
* \file object_detection_demo_yolov3_async/main.cpp
* \example object_detection_demo_yolov3_async/main.cpp
*/

#include <functional>
#include <iostream>
#include <fstream>
#include <random>
#include <memory>
#include <chrono>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

#include <inference_engine.hpp>
#include <ie_utils.hpp>

#include <samples/ocv_common.hpp>
#include <samples/slog.hpp>

#include <ext_list.hpp>
#include <QDebug>

using namespace InferenceEngine;
using namespace std;

#define FLAGS_d "gpu"
#define FLAGS_m "Resources/weights/chardetector-yolov3-tiny.xml"
#define FLAGS_auto_resize false
#define FLAGS_iou_t 0.4
#define FLAGS_t 0.5


static InferRequest::Ptr async_infer_request_curr;
static ExecutableNetwork network;
static InputsDataMap inputInfo;
static OutputsDataMap outputInfo;
static CNNNetReader netReader;

static void FrameToBlob(const cv::Mat& frame, InferRequest::Ptr& inferRequest, const std::string& inputName) {
	if (FLAGS_auto_resize) {
		/* Just set input blob containing read image. Resize and layout conversion will be done automatically */
		inferRequest->SetBlob(inputName, wrapMat2Blob(frame));
	}
	else {
		/* Resize and copy data from the image to the input blob */
		Blob::Ptr frameBlob = inferRequest->GetBlob(inputName);
		matU8ToBlob<uint8_t>(frame, frameBlob);
	}
}

static int EntryIndex(int side, int lcoords, int lclasses, int location, int entry) {
	int n = location / (side * side);
	int loc = location % (side * side);
	return n * side * side * (lcoords + lclasses + 1) + entry * side * side + loc;
}

static double IntersectionOverUnion(const DetectionObject& box_1, const DetectionObject& box_2) {
	double width_of_overlap_area = fmin(box_1.xmax, box_2.xmax) - fmax(box_1.xmin, box_2.xmin);
	double height_of_overlap_area = fmin(box_1.ymax, box_2.ymax) - fmax(box_1.ymin, box_2.ymin);
	double area_of_overlap;
	if (width_of_overlap_area < 0 || height_of_overlap_area < 0)
		area_of_overlap = 0;
	else
		area_of_overlap = width_of_overlap_area * height_of_overlap_area;
	double box_1_area = (box_1.ymax - box_1.ymin) * (box_1.xmax - box_1.xmin);
	double box_2_area = (box_2.ymax - box_2.ymin) * (box_2.xmax - box_2.xmin);
	double area_of_union = box_1_area + box_2_area - area_of_overlap;
	return area_of_overlap / area_of_union;
}

static void ParseYOLOV3Output(const CNNLayerPtr& layer, const Blob::Ptr& blob, const unsigned long resized_im_h,
	const unsigned long resized_im_w, const unsigned long original_im_h,
	const unsigned long original_im_w,
	const double threshold, std::vector<DetectionObject>& objects) {
	// --------------------------- Validating output parameters -------------------------------------
	if (layer->type != "RegionYolo")
		throw std::runtime_error("Invalid output type: " + layer->type + ". RegionYolo expected");
	const int out_blob_h = static_cast<int>(blob->getTensorDesc().getDims()[2]);
	const int out_blob_w = static_cast<int>(blob->getTensorDesc().getDims()[3]);
	if (out_blob_h != out_blob_w)
		throw std::runtime_error("Invalid size of output " + layer->name +
			" It should be in NCHW layout and H should be equal to W. Current H = " + std::to_string(out_blob_h) +
			", current W = " + std::to_string(out_blob_h));
	// --------------------------- Extracting layer parameters -------------------------------------
	auto num = layer->GetParamAsInt("num");
	auto coords = layer->GetParamAsInt("coords");
	auto classes = layer->GetParamAsInt("classes");
	std::vector<float> anchors = { 10.0, 13.0, 16.0, 30.0, 33.0, 23.0, 30.0, 61.0, 62.0, 45.0, 59.0, 119.0, 116.0, 90.0,
								  156.0, 198.0, 373.0, 326.0 };
	try { anchors = layer->GetParamAsFloats("anchors"); }
	catch (...) {}
	try {
		auto mask = layer->GetParamAsInts("mask");
		num = mask.size();

		std::vector<float> maskedAnchors(num * 2);
		for (int i = 0; i < num; ++i) {
			maskedAnchors[i * 2] = anchors[mask[i] * 2];
			maskedAnchors[i * 2 + 1] = anchors[mask[i] * 2 + 1];
		}
		anchors = maskedAnchors;
	}
	catch (...) {}

	auto side = out_blob_h;
	auto side_square = side * side;
	const float* output_blob = blob->buffer().as<PrecisionTrait<Precision::FP32>::value_type*>();
	// --------------------------- Parsing YOLO Region output -------------------------------------
	for (int i = 0; i < side_square; ++i) {
		int row = i / side;
		int col = i % side;
		for (int n = 0; n < num; ++n) {
			int obj_index = EntryIndex(side, coords, classes, n * side * side + i, coords);
			int box_index = EntryIndex(side, coords, classes, n * side * side + i, 0);
			float scale = output_blob[obj_index];
			if (scale < threshold)
				continue;
			double x = (col + output_blob[box_index + 0 * side_square]) / side * resized_im_w;
			double y = (row + output_blob[box_index + 1 * side_square]) / side * resized_im_h;
			double height = std::exp(output_blob[box_index + 3 * side_square]) * anchors[2 * n + 1];
			double width = std::exp(output_blob[box_index + 2 * side_square]) * anchors[2 * n];
			for (int j = 0; j < classes; ++j) {
				int class_index = EntryIndex(side, coords, classes, n * side_square + i, coords + 1 + j);
				float prob = scale * output_blob[class_index];
				if (prob < threshold)
					continue;
				DetectionObject obj(x, y, height, width, j, prob,
					static_cast<float>(original_im_h) / static_cast<float>(resized_im_h),
					static_cast<float>(original_im_w) / static_cast<float>(resized_im_w));
				objects.push_back(obj);
			}
		}
	}
}

YoloVinoCaller::YoloVinoCaller() { ; }

YoloVinoCaller::~YoloVinoCaller() { 
	inputInfo.clear(); outputInfo.clear();
}

void YoloVinoCaller::load() {
	try {
		std::cout << "InferenceEngine: " << GetInferenceEngineVersion() << std::endl;

		// --------------------------- 1. Load inference engine -------------------------------------
		slog::info << "Loading Inference Engine" << slog::endl;
		Core ie;
		slog::info << "Device info: " << slog::endl;
		std::cout << ie.GetVersions(FLAGS_d);

		/**Loading extensions to the devices **/

		/** Loading default extensions **/
		if (FLAGS_d == std::string("CPU")) {
			/**
			 * cpu_extensions library is compiled from the "extension" folder containing
			 * custom CPU layer implementations.
			**/
			ie.AddExtension(std::make_shared<Extensions::Cpu::CpuExtensions>(), "CPU");
		}

		// --------------- 2. Reading the IR generated by the Model Optimizer (.xml and .bin files) ------------
		slog::info << "Loading network files" << slog::endl;
		/** Reading network model **/
		netReader.ReadNetwork(FLAGS_m);
		/** Setting batch size to 1 **/
		slog::info << "Batch size is forced to  1." << slog::endl;
		netReader.getNetwork().setBatchSize(1);
		/** Extracting the model name and loading its weights **/
		std::string binFileName = fileNameNoExt(FLAGS_m) + ".bin";
		netReader.ReadWeights(binFileName);
		/** Reading labels (if specified) **/
		std::string labelFileName = fileNameNoExt(FLAGS_m) + ".labels";
		std::ifstream inputFile(labelFileName);
		std::copy(std::istream_iterator<std::string>(inputFile),
			std::istream_iterator<std::string>(),
			std::back_inserter(labels));
		// -----------------------------------------------------------------------------------------------------

		/** YOLOV3-based network should have one input and three output **/
		// --------------------------- 3. Configuring input and output -----------------------------------------
		// --------------------------------- Preparing input blobs ---------------------------------------------
		slog::info << "Checking that the inputs are as the demo expects" << slog::endl;
		inputInfo = InputsDataMap(netReader.getNetwork().getInputsInfo());
		if (inputInfo.size() != 1) {
			throw std::logic_error("This demo accepts networks that have only one input");
		}
		InputInfo::Ptr& input = inputInfo.begin()->second;
		auto inputName = inputInfo.begin()->first;
		input->setPrecision(Precision::U8);
		if (FLAGS_auto_resize) {
			input->getPreProcess().setResizeAlgorithm(ResizeAlgorithm::RESIZE_BILINEAR);
			input->getInputData()->setLayout(Layout::NHWC);
		}
		else {
			input->getInputData()->setLayout(Layout::NCHW);
		}
		// --------------------------------- Preparing output blobs -------------------------------------------
		slog::info << "Checking that the outputs are as the demo expects" << slog::endl;
		outputInfo = OutputsDataMap(netReader.getNetwork().getOutputsInfo());
		for (auto& output : outputInfo) {
			output.second->setPrecision(Precision::FP32);
			output.second->setLayout(Layout::NCHW);
		}
		// -----------------------------------------------------------------------------------------------------

		// --------------------------- 4. Loading model to the device ------------------------------------------
		slog::info << "Loading model to the device" << slog::endl;
		network = ie.LoadNetwork(netReader.getNetwork(), FLAGS_d);

		// -----------------------------------------------------------------------------------------------------

		// --------------------------- 5. Creating infer request -----------------------------------------------
		async_infer_request_curr = network.CreateInferRequestPtr();
		// -----------------------------------------------------------------------------------------------------

		// --------------------------- 6. Doing inference ------------------------------------------------------
		slog::info << "Start inference " << slog::endl;

	}
	catch (const std::exception& error) {
		std::cerr << "[ ERROR ] " << error.what() << std::endl;
		return;
	}
	catch (...) {
		std::cerr << "[ ERROR ] Unknown/internal exception happened." << std::endl;
		return;
	}
	return;
}

void YoloVinoCaller::predict() {
	try {
		size_t obindex = 0;
		outputs->resize(inputs->size());
		for (auto& input : *inputs) {
			int w = input.width(), h = input.height();
			FrameToBlob(QImage2CvMat(input), async_infer_request_curr, "inputs");
			// Main sync point:
			// in the true Async mode, we start the NEXT infer request while waiting for the CURRENT to complete
			// in the regular mode, we start the CURRENT request and wait for its completion
			async_infer_request_curr->StartAsync();
			if (OK == async_infer_request_curr->Wait(IInferRequest::WaitMode::RESULT_READY)) {
				// ---------------------------Processing output blobs--------------------------------------------------
				// Processing results of the CURRENT request
				const TensorDesc& inputDesc = inputInfo.begin()->second.get()->getTensorDesc();
				unsigned long resized_im_h = getTensorHeight(inputDesc);
				unsigned long resized_im_w = getTensorWidth(inputDesc);
				vector<DetectionObject>& objects = outputs->at(obindex++);
				objects.clear();
				// Parsing outputs
				for (auto& output : outputInfo) {
					auto output_name = output.first;
					CNNLayerPtr layer = netReader.getNetwork().getLayerByName(output_name.c_str());
					Blob::Ptr blob = async_infer_request_curr->GetBlob(output_name);
					ParseYOLOV3Output(layer, blob, resized_im_h, resized_im_w, h, w, FLAGS_t, objects);
				}
				// Filtering overlapping boxes
				std::sort(objects.begin(), objects.end(), std::greater<DetectionObject>());
				for (size_t i = 0; i < objects.size(); ++i) {
					if (objects[i].confidence == 0)
						continue;
					for (size_t j = i + 1; j < objects.size(); ++j)
						if (IntersectionOverUnion(objects[i], objects[j]) >= FLAGS_iou_t)
							objects[j].confidence = 0;
				}

			}

		}
		slog::info << "Execution successful" << slog::endl;
	}
	catch (const std::exception& error) {
		std::cerr << "[ ERROR ] " << error.what() << std::endl;
		return;
	}
	catch (...) {
		std::cerr << "[ ERROR ] Unknown/internal exception happened." << std::endl;
		return;
	}

}

/* cv::mat和Qt::QImage互相转化，
 * 代码出处： https://blog.csdn.net/qq_17550379/article/details/78683153
 * 改为原位解析
 */
inline cv::Mat QImage2CvMat(QImage& inImage) {
	switch (inImage.format()) {
	case QImage::Format_ARGB32:// 8-bit, 4 channel
	case QImage::Format_ARGB32_Premultiplied: {
		cv::Mat  mat(inImage.height(), inImage.width(), CV_8UC4,
			const_cast<uchar*>(inImage.bits()),
			static_cast<size_t>(inImage.bytesPerLine())
		);
		return mat;
	}
	case QImage::Format_RGB32:// 8-bit, 3 channel
	case QImage::Format_RGB888: {
		if (inImage.format() == QImage::Format_RGB32) {
			inImage = inImage.convertToFormat(QImage::Format_RGB888);
		}
		inImage = inImage.rgbSwapped();
		return cv::Mat(inImage.height(), inImage.width(), CV_8UC3,
			const_cast<uchar*>(inImage.bits()),
			static_cast<size_t>(inImage.bytesPerLine())
		);
	}
	case QImage::Format_Indexed8: {// 8-bit, 1 channel
		cv::Mat  mat(inImage.height(), inImage.width(), CV_8UC1,
			const_cast<uchar*>(inImage.bits()),
			static_cast<size_t>(inImage.bytesPerLine())
		);
		return mat;
	}
	default:
		qWarning() << "CVS::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
		break;
	}
	return cv::Mat();
}

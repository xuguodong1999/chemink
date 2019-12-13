# -*- coding:utf-8
from __future__ import print_function
import os
import cv2
import numpy as np
from keras import backend as K
from keras.optimizers import Adam
from keras.models import Model
from keras.utils import to_categorical
from keras.callbacks import ModelCheckpoint
from keras.preprocessing.image import img_to_array
from densenet import *

int_epochs = 10
float_learning_rate = 1e-4  # 学习率
int_batch_size = 12
bool_load_weights = 1     # 是否加载预训练权重 1：加载 else：不加载


int_class_num = 78
int_width = 64
string_model_name = "DenseNet-78-64x64"

# 以 darknet CNN 加载数据集的方式加载数据


def loadChemChar(collectedNames):
    print("从加载所有图片...", collectedNames)
    data = []
    labels = []
    f = open(collectedNames, 'r', encoding='utf8')
    for line in f.readlines():
        if len(line) < 5:
            continue
        line = line[0:len(line)-1]
        # print("当前文件为：", line)
        # image = cv2.imread(line)
        image = cv2.imdecode(np.fromfile(line, dtype=np.uint8), -1)  # 邪恶的中文编码
        image = cv2.resize(image, (int_width, int_width))
        # cv2.imshow('当前图片',image);
        # cv2.waitKey(0);
        image = img_to_array(image)
        data.append(image)
        i = 0
        while line[i] != '(':
            i = i+1
        i = i+1
        label = ''
        while line[i] != ')':
            label += line[i]
            i = i+1
        # print("当前标签为：",label);
        labels.append(label)
    data = np.array(data, dtype="float") / 255.0
    label_set = set(labels)
    img_label_ = sorted(list((label_set)))
    dict0 = {}
    for i in range(len(label_set)):
        dict0[img_label_[i]] = i
    newlabels = [dict0[i] for i in labels]
    newlabels = np.array(newlabels)
    newlabels = to_categorical(newlabels, num_classes=int_class_num)
    iii = 0
    for key in dict0.keys():
        print("case ", iii, ":\nreturn \"", key, "\";\n", end="")
        iii = iii+1
    # cv2.waitKey(0);
    return data, newlabels

# 训练模型


def train(trainX, trainY, testX, testY, savename):
    print("开始构建模型...")
    img_input = Input(shape=(None, None, 1))
    output = densenet(img_input, int_class_num)
    model = Model(img_input, output)

    if bool_load_weights == 1:
        model.load_weights("Final-"+string_model_name+".h5")
    opt = Adam(lr=float_learning_rate, decay=float_learning_rate / int_epochs)
    model.compile(loss='categorical_crossentropy',
                  optimizer=opt,
                  metrics=['accuracy'])

    print("开始训练模型...")
    checkpoint = ModelCheckpoint("weights.epoch={epoch:02d}.h5", monitor='val_acc', verbose=1, save_best_only=False,
                                 mode='max',save_weights_only=False,period=1)
    callbacks_list = [checkpoint]
    H = model.fit(trainX, trainY, batch_size=int_batch_size,
                  validation_data=(testX, testY),
                  epochs=int_epochs, verbose=1,callbacks=callbacks_list)
    print("开始保存模型...")
    model.save("Final-"+savename)


if __name__ == '__main__':
    testX, testY = loadChemChar('./test.list')
    # trainX, trainY = testX, testY
    trainX, trainY = loadChemChar('./train.list')
    train(trainX, trainY, testX, testY, string_model_name+".h5")

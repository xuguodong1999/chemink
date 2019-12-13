# -*- coding:utf-8
import keras
import math
import numpy as np
from keras.layers.normalization import BatchNormalization
from keras.layers import Conv2D, Dense, Input, add, Activation, AveragePooling2D, GlobalAveragePooling2D, Lambda, concatenate, Dropout
from keras.models import Model

growth_rate = 12
depth = 64
compression = 0.5

def densenet(img_input, classes_num):
    def conv(x, out_filters, k_size):
        x = Conv2D(filters=out_filters,
                      kernel_size=k_size,
                      strides=(1, 1),
                      padding='same',
                      use_bias=False)(x)
        return x

    def dense_layer(x):
        return Dense(units=classes_num, activation='softmax')(x)

    def bn_relu(x):
        x = BatchNormalization(momentum=0.9, epsilon=1e-5)(x)
        x = Activation('relu')(x)
        return x

    def bottleneck(x):
        channels = growth_rate * 4
        x = bn_relu(x)
        x = conv(x, channels, (1, 1))
        x = bn_relu(x)
        x = conv(x, growth_rate, (3, 3))
        return x

    def transition(x, inchannels):
        outchannels = int(inchannels * compression)
        x = bn_relu(x)
        x = conv(x, outchannels, (1, 1))
        x = AveragePooling2D((2, 2), strides=(2, 2))(x)
        return x, outchannels

    def dense_block(x, blocks, nchannels):
        concat = x
        for i in range(blocks):
            x = bottleneck(concat)
            concat = concatenate([x, concat], axis=-1)
            nchannels += growth_rate
        return concat, nchannels

    nblocks = (depth - 4) // 6
    nchannels = growth_rate * 2

    x = conv(img_input, nchannels, (3, 3))
    x, nchannels = dense_block(x, nblocks, nchannels)
    x, nchannels = transition(x, nchannels)
    x, nchannels = dense_block(x, nblocks, nchannels)
    x, nchannels = transition(x, nchannels)
    x, nchannels = dense_block(x, nblocks, nchannels)
    x = bn_relu(x)
    x = GlobalAveragePooling2D()(x)
    x = dense_layer(x)
    return x

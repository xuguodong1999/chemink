from keras.models import load_model
from keras.utils import plot_model
string_model_name = 'densenet-chdsbw'
model = load_model('Final-DenseNet-78-64x64.h5')
model.summary()

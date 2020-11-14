from keras.models import load_model
from keras.utils import plot_model
string_model_name = 'DenseNet-78-64x64'

model = load_model(string_model_name+'.h5') 
plot_model(model, to_file=string_model_name+'.png',show_shapes=True)

import cv2
import numpy as np
from keras.models import load_model

model = load_model('densenet-chdsbw.h5')  # .h模型名称
image = cv2.imread('000000.PNG')
# sp=image.shape
# img = cv2.cvtColor(image,cv2.COLOR_RGB2GRAY) # RGB图像转为gray
# print(sp)
# img=image.reshape(1,sp[1],sp[0],sp[2])

# img=cv2.resize(image,(132,132),interpolation=cv2.INTER_CUBIC)
# img = (img.reshape(1,132,132,3)).astype('float32')/255.0
# cv2.imshow('cacacas',img)
# cv2.waitKey(0)
# 可能是没做多尺度的原因，全卷积不是很有效。发现，w=20~40能得到靠谱的预测
w = 32
img = cv2.resize(image, (w, w), interpolation=cv2.INTER_CUBIC)
img = (img.reshape(1, w, w, 3)).astype('float32')/255.0
predict = model.predict(img)

maxp = np.argmax(predict, axis=1)
print("最高概率的：", maxp)
print('概率分布：')
for i in predict[0]:
    print(round(i, 3), end="")
    print("\t", end="")

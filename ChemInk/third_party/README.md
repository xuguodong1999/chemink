# 软链接备忘

## (管理员模式的CMD) mklink /D "XXX\ZZ" "YYY\ZZ"

### 表示创建目录XXX\ZZ，指向YYY\ZZ。"XXX\ZZ"和"YYY\ZZ"拥有独立的inode，删除XXX\ZZ不影响YYY\ZZ（可以在WSL上用stat命令查看inode值）

### 一般地，会在一个统一的目录维护所有第三方库，比如C:/3rdlib，在自己的工程下用软链接的形式做库的寻址

### 以opencv为例，假设存在C:/3rdlib/opencv-4.1.1、C:/3rdlib/opencv-4.1.2、C:/3rdlib/opencv-3.4.1，为了方便切换版本（为什么要切？4.x.x不带lsd而3.4.1带，切了望望效果是有必要的），或者是为了切换动态库和静态库，一般的做法是：mklink /D (注意这个D不能小写) "C:/3rdlib/opencv-x.x.x"(统一放库的地方) "XXX/third_party/opencv"(工程目录)，这里可以用opencv代称各个版本，来方便编写包含目录、库目录。切换的时候直接删掉软链接再重建即可
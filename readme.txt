http://toutiao.com/i6310085086318428674/
http://blog.csdn.net/ya_da/article/details/51959547
file:///home/cloud/opencv3.0/opencv-3.0.0-alpha/doc/haartraining.htm
http://blog.csdn.net/pb09013037/article/details/44221809
http://www.xuebuyuan.com/2106552.html
http://www.computer-vision-software.com/blog/2009/11/faq-opencv-haartraining/
http://stackoverflow.com/questions/8697385/opencv-2-3-1-how-to-tell-if-haartraining-is-stuck-or-still-working-on-tiny-exa
http://blog.csdn.net/huixingshao/article/details/44561385
http://answers.opencv.org/question/3085/why-always-opencv-error-assertion-failed-elements_read-1-in-unknown-function/
http://blog.csdn.net/woaipaoche/article/details/41517089
vec-file has to contain >= (numPos + (numStages-1) * (1 - minHitRate) * numPos) + S
7000 >= (numPos + (20-1) * (1 - 0.999) * numPos) + 2973

(7000 - 2973)/(1 + 19*0.001) >= numPos

numPos <= 4027/1.019

numPos <= 3951 ~~ 3950
-npos 3950 -nneg 2973

 测试最终分类器性能
  这部分工作通过简单地调用两个函数实现。调用 icvGetHaarTrainingDataFromVec 测试
检出率；通过调用 icvGetHaarTrainingDataFromBG测试虚警率。 

adaboost
http://blog.csdn.net/tiandijun/article/details/48036025
http://blog.csdn.net/haidao2009/article/details/7514787


https://www.pyimagesearch.com/pyimagesearch-gurus/

https://github.com/opencv/opencv/tree/master/data/haarcascades

http://www.cnblogs.com/wengzilin/p/3845271.html

http://www.open-open.com/lib/view/open1440832074794.html

http://www.cnblogs.com/denny402/p/5031181.html

http://www.cnblogs.com/chensheng-zhou/p/5542887.html

http://www.cnblogs.com/ello/archive/2012/04/28/2475419.html

LBP
http://blog.csdn.net/zouxy09/article/details/7929531
http://www.07net01.com/program/651674.html
http://www.walthelm.net/picture-relate/download.php

fs = 1e3; %частота дискретизация
t = 0:1/fs:1; %массив времени
x = [1 2]*sin(2*pi*[50 250]'.*t) + randn(size(t))/10; %сигнал с высокочастотной составляющей
lowpass (x, 350, fs) %фильтрация нижних частот и построение графиков, второй аргумент - частота среза
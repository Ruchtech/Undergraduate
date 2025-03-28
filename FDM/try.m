clc; clear; close all;
%{
% 语音初始化
fs = 80000;                                     % 采样频率Hz
nBits = 16;                                     % 位深度
nChannels = 1;                                  % 声道数
recObj = audiorecorder(fs, nBits, nChannels);   % 创建录音对象

% 开始录音
disp('按回车键开始录音...');
pause;
disp('开始录音...');
record(recObj);  

% 停止录音
disp('按回车键停止录音...');
pause;  
stop(recObj);
disp('录音结束。');

% 获取音频数据
audioData = getaudiodata(recObj);
disp('正在播放录音...');
sound(audioData, fs);

% 保存音频为 wav 文件
outputFileName = 'audio_4.wav';
audiowrite(outputFileName, audioData, fs);
disp(['录音已保存为 MP3 文件：' outputFileName]);

%}

fs = 90000;
%%% 2. 音频信号分析 %%
% 音频信号读取
[s1, ~] = audioread('audio_1.wav');
[s2, ~] = audioread('audio_2.wav');
[s3, ~] = audioread('audio_3.wav');

% 对齐音频信号长度
max_len = max([length(s1), length(s2), length(s3)]);% 找最长的信号
t = (0:max_len-1) / fs;
s1 = [s1; zeros(max_len - length(s1), 1)];
s2 = [s2; zeros(max_len - length(s2), 1)];
s3 = [s3; zeros(max_len - length(s3), 1)];

% 显示时域波形
figure(1);
subplot(3, 2, 1); plot(t, s1); xlabel('时间 (秒)'); ylabel('幅度'); title('时域波形1');
subplot(3, 2, 3); plot(t, s2); xlabel('时间 (秒)'); ylabel('幅度'); title('时域波形2');
subplot(3, 2, 5); plot(t, s3); xlabel('时间 (秒)'); ylabel('幅度'); title('时域波形3');

% 显示频谱图
N = 250000;                                         % FFT点数
f = (-N/2:N/2-1) * (fs / N);                        % 中心对齐的频率轴
Y1 = fftshift(fft(s1, N));
Y2 = fftshift(fft(s2, N));
Y3 = fftshift(fft(s3, N));
subplot(3, 2, 2); plot(f, abs(Y1)); xlabel('频率 (Hz)'); ylabel('幅度'); title('频谱图1');
subplot(3, 2, 4); plot(f, abs(Y2)); xlabel('频率 (Hz)'); ylabel('幅度'); title('频谱图2');
subplot(3, 2, 6); plot(f, abs(Y3)); xlabel('频率 (Hz)'); ylabel('幅度'); title('频谱图3');

%%% 3. 信号调制 %%%
fc1 = 10000; fc2 = 20000; fc3 = 30000;              % 高频载波频率
carrier1 = cos(2 * pi * fc1 * t)';
carrier2 = cos(2 * pi * fc2 * t)';
carrier3 = cos(2 * pi * fc3 * t)';

% 调制信号
sm1 = s1 .* carrier1;
sm2 = s2 .* carrier2;
sm3 = s3 .* carrier3;

% 调制信号频谱分析
N = length(sm1); 
f = (-N/2:N/2-1) * (fs / N);
F1 = fftshift(fft(sm1, N));
F2 = fftshift(fft(sm2, N));
F3 = fftshift(fft(sm3, N));
figure(2);
subplot(3, 2, 1); plot(t, sm1); xlabel('单位：s'); ylabel('幅度'); title('调制信号1');
subplot(3, 2, 3); plot(t, sm2); xlabel('单位：s'); ylabel('幅度'); title('调制信号2');
subplot(3, 2, 5); plot(t, sm3); xlabel('单位：s'); ylabel('幅度'); title('调制信号3');
subplot(3, 2, 2); plot(f, abs(F1)); xlabel('单位：HZ'); ylabel('幅度'); title('调制频谱图1');
subplot(3, 2, 4); plot(f, abs(F2)); xlabel('单位：HZ'); ylabel('幅度'); title('调制频谱图2');
subplot(3, 2, 6); plot(f, abs(F3)); xlabel('单位：HZ'); ylabel('幅度'); title('调制频谱图3');

%%% 4. 信号叠加和频谱分析 %%%
combined_signal = sm1 + sm2 + sm3;                  % 信号叠加
combined_spectrum = fftshift(fft(combined_signal, N)); 
figure(3);
subplot(2, 1, 1); plot(t, combined_signal); xlabel('单位：s'); ylabel('幅度'); title('叠加信号时域图');
subplot(2, 1, 2); plot(f, abs(combined_spectrum)); xlabel('单位：HZ'); ylabel('幅度'); title('叠加信号频谱图');


%%% 5. 信号分频 %%%
% 带通滤波器的设计
fs1 = 100000;                                       % 采样频率
Rp = 0.5; Rs = 40;                                  % 通带纹波和阻带衰减

% 带通滤波器1
Wp1 = [9000 11000]; Ws1 = [8000 12000];             % 模拟频率
[n1, Wp1_analog] = cheb2ord(2*pi*Wp1, 2*pi*Ws1, Rp, Rs, 's');  
[b1_analog, a1_analog] = cheby2(n1, Rs, Wp1_analog, 'bandpass', 's'); 
[b1, a1] = impinvar(b1_analog, a1_analog, fs1);     % 脉冲响应不变法

% 带通滤波器2
Wp2 = [19000 23000]; Ws2 = [18000 24000];
[n2, Wp2_analog] = cheb2ord(2*pi*Wp2, 2*pi*Ws2, Rp, Rs, 's');  
[b2_analog, a2_analog] = cheby2(n2, Rs, Wp2_analog, 'bandpass', 's'); 
[b2, a2] = impinvar(b2_analog, a2_analog, fs1);

% 带通滤波器3
Wp3 = [27000 33000]; Ws3 = [26000 34000];
[n3, Wp3_analog] = cheb2ord(2*pi*Wp3, 2*pi*Ws3, Rp, Rs, 's');  
[b3_analog, a3_analog] = cheby2(n3, Rs, Wp3_analog, 'bandpass', 's'); 
[b3, a3] = impinvar(b3_analog, a3_analog, fs1);

% 显示频率响应
[h1, w1] = freqz(b1, a1);
[h2, w2] = freqz(b2, a2);
[h3, w3] = freqz(b3, a3);

figure(4);
subplot(3,1,1); plot(w1*fs1/(2*pi), abs(h1)); xlabel('频率'); ylabel('幅度'); title('切比雪夫2型带通滤波器1');
subplot(3,1,2); plot(w2*fs1/(2*pi), abs(h2)); xlabel('频率'); ylabel('幅度'); title('切比雪夫2型带通滤波器2');
subplot(3,1,3); plot(w3*fs1/(2*pi), abs(h3)); xlabel('频率'); ylabel('幅度'); title('切比雪夫2型带通滤波器3');
lv11 = filter(b1, a1, combined_signal);             % 信号滤波 
lv12 = filter(b2, a2, combined_signal);
lv13 = filter(b3, a3, combined_signal);


%%% 6. 解调 %%%
% 对滤出的信号进行解调
s10 = lv11 .* cos(2 * pi * fc1 * t)';
s20 = lv12 .* cos(2 * pi * fc2 * t)';
s30 = lv13 .* cos(2 * pi * fc3 * t)';

% 解调信号频谱
Z10 = fftshift(fft(s10));  
Z20 = fftshift(fft(s20));  
Z30 = fftshift(fft(s30));
figure(6);
subplot(3,2,1); plot(t, s10); xlabel('单位：s'); ylabel('幅度'); title('解调信号波形1');
subplot(3,2,2); plot(f, abs(Z10)); xlabel('单位：HZ'); ylabel('幅度'); title('解调信号频谱1'); 
subplot(3,2,3); plot(t, s20); xlabel('单位：s'); ylabel('幅度'); title('解调信号波形2');
subplot(3,2,4); plot(f, abs(Z20)); xlabel('单位：HZ'); ylabel('幅度'); title('解调信号频谱2'); 
subplot(3,2,5); plot(t, s30); xlabel('单位：s'); ylabel('幅度'); title('解调信号波形3');
subplot(3,2,6); plot(f, abs(Z30)); xlabel('单位：HZ'); ylabel('幅度'); title('解调信号频谱3');


%%% 7. 解调后的信号低通滤波器处理 %%%
% 设计低通滤波器
Wp = 1500 / (fs / 2);                               % 截止频率
Ws = 8000 / (fs / 2);                               % 阻带频率
Rp = 0.5; Rs = 40;                                  % 纹波和衰减
[n, Wn] = cheb2ord(Wp, Ws, Rp, Rs);                 % 计算滤波器的最小阶数和截止频率
[b, a] = cheby2(n, Rs, Wn);

% 绘制滤波器响应
figure(7);
[H, f1] = freqz(b, a, 1024, fs);                     % 计算频率响应
plot(f1, abs(H));                                    % 绘制幅频响应
xlabel('频率 (Hz)');                                % 横轴标注
ylabel('幅度');                                     % 纵轴标注
title('切比雪夫2型低通滤波器');                     % 图表标题

% 滤波操作
lv1_filtered = filter(b, a, s10);
lv2_filtered = filter(b, a, s20);
lv3_filtered = filter(b, a, s30);

% 显示低通滤波后的信号时域和频谱图
figure(8);
subplot(3,2,1); plot(t, lv1_filtered); xlabel('单位：s'); ylabel('幅度'); title('低通滤波后的信号1');
subplot(3,2,2); plot(f, abs(fftshift(fft(lv1_filtered, N)))); xlabel('单位：HZ'); ylabel('幅度'); title('低通滤波后的信号1频谱');
subplot(3,2,3); plot(t, lv2_filtered); xlabel('单位：s'); ylabel('幅度'); title('低通滤波后的信号2');
subplot(3,2,4); plot(f, abs(fftshift(fft(lv2_filtered, N)))); xlabel('单位：HZ'); ylabel('幅度'); title('低通滤波后的信号2频谱');
subplot(3,2,5); plot(t, lv3_filtered); xlabel('单位：s'); ylabel('幅度'); title('低通滤波后的信号3');
subplot(3,2,6); plot(f, abs(fftshift(fft(lv3_filtered, N)))); xlabel('单位：HZ'); ylabel('幅度'); title('低通滤波后的信号3频谱');
%sound(lv1_filtered, fs);
sound(lv2_filtered, fs);
%sound(lv3_filtered, fs);


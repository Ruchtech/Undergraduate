clc; clear; close all;
%{
% ������ʼ��
fs = 80000;                                     % ����Ƶ��Hz
nBits = 16;                                     % λ���
nChannels = 1;                                  % ������
recObj = audiorecorder(fs, nBits, nChannels);   % ����¼������

% ��ʼ¼��
disp('���س�����ʼ¼��...');
pause;
disp('��ʼ¼��...');
record(recObj);  

% ֹͣ¼��
disp('���س���ֹͣ¼��...');
pause;  
stop(recObj);
disp('¼��������');

% ��ȡ��Ƶ����
audioData = getaudiodata(recObj);
disp('���ڲ���¼��...');
sound(audioData, fs);

% ������ƵΪ wav �ļ�
outputFileName = 'audio_4.wav';
audiowrite(outputFileName, audioData, fs);
disp(['¼���ѱ���Ϊ MP3 �ļ���' outputFileName]);

%}

fs = 90000;
%%% 2. ��Ƶ�źŷ��� %%
% ��Ƶ�źŶ�ȡ
[s1, ~] = audioread('audio_1.wav');
[s2, ~] = audioread('audio_2.wav');
[s3, ~] = audioread('audio_3.wav');

% ������Ƶ�źų���
max_len = max([length(s1), length(s2), length(s3)]);% ������ź�
t = (0:max_len-1) / fs;
s1 = [s1; zeros(max_len - length(s1), 1)];
s2 = [s2; zeros(max_len - length(s2), 1)];
s3 = [s3; zeros(max_len - length(s3), 1)];

% ��ʾʱ����
figure(1);
subplot(3, 2, 1); plot(t, s1); xlabel('ʱ�� (��)'); ylabel('����'); title('ʱ����1');
subplot(3, 2, 3); plot(t, s2); xlabel('ʱ�� (��)'); ylabel('����'); title('ʱ����2');
subplot(3, 2, 5); plot(t, s3); xlabel('ʱ�� (��)'); ylabel('����'); title('ʱ����3');

% ��ʾƵ��ͼ
N = 250000;                                         % FFT����
f = (-N/2:N/2-1) * (fs / N);                        % ���Ķ����Ƶ����
Y1 = fftshift(fft(s1, N));
Y2 = fftshift(fft(s2, N));
Y3 = fftshift(fft(s3, N));
subplot(3, 2, 2); plot(f, abs(Y1)); xlabel('Ƶ�� (Hz)'); ylabel('����'); title('Ƶ��ͼ1');
subplot(3, 2, 4); plot(f, abs(Y2)); xlabel('Ƶ�� (Hz)'); ylabel('����'); title('Ƶ��ͼ2');
subplot(3, 2, 6); plot(f, abs(Y3)); xlabel('Ƶ�� (Hz)'); ylabel('����'); title('Ƶ��ͼ3');

%%% 3. �źŵ��� %%%
fc1 = 10000; fc2 = 20000; fc3 = 30000;              % ��Ƶ�ز�Ƶ��
carrier1 = cos(2 * pi * fc1 * t)';
carrier2 = cos(2 * pi * fc2 * t)';
carrier3 = cos(2 * pi * fc3 * t)';

% �����ź�
sm1 = s1 .* carrier1;
sm2 = s2 .* carrier2;
sm3 = s3 .* carrier3;

% �����ź�Ƶ�׷���
N = length(sm1); 
f = (-N/2:N/2-1) * (fs / N);
F1 = fftshift(fft(sm1, N));
F2 = fftshift(fft(sm2, N));
F3 = fftshift(fft(sm3, N));
figure(2);
subplot(3, 2, 1); plot(t, sm1); xlabel('��λ��s'); ylabel('����'); title('�����ź�1');
subplot(3, 2, 3); plot(t, sm2); xlabel('��λ��s'); ylabel('����'); title('�����ź�2');
subplot(3, 2, 5); plot(t, sm3); xlabel('��λ��s'); ylabel('����'); title('�����ź�3');
subplot(3, 2, 2); plot(f, abs(F1)); xlabel('��λ��HZ'); ylabel('����'); title('����Ƶ��ͼ1');
subplot(3, 2, 4); plot(f, abs(F2)); xlabel('��λ��HZ'); ylabel('����'); title('����Ƶ��ͼ2');
subplot(3, 2, 6); plot(f, abs(F3)); xlabel('��λ��HZ'); ylabel('����'); title('����Ƶ��ͼ3');

%%% 4. �źŵ��Ӻ�Ƶ�׷��� %%%
combined_signal = sm1 + sm2 + sm3;                  % �źŵ���
combined_spectrum = fftshift(fft(combined_signal, N)); 
figure(3);
subplot(2, 1, 1); plot(t, combined_signal); xlabel('��λ��s'); ylabel('����'); title('�����ź�ʱ��ͼ');
subplot(2, 1, 2); plot(f, abs(combined_spectrum)); xlabel('��λ��HZ'); ylabel('����'); title('�����ź�Ƶ��ͼ');


%%% 5. �źŷ�Ƶ %%%
% ��ͨ�˲��������
fs1 = 100000;                                       % ����Ƶ��
Rp = 0.5; Rs = 40;                                  % ͨ���Ʋ������˥��

% ��ͨ�˲���1
Wp1 = [9000 11000]; Ws1 = [8000 12000];             % ģ��Ƶ��
[n1, Wp1_analog] = cheb2ord(2*pi*Wp1, 2*pi*Ws1, Rp, Rs, 's');  
[b1_analog, a1_analog] = cheby2(n1, Rs, Wp1_analog, 'bandpass', 's'); 
[b1, a1] = impinvar(b1_analog, a1_analog, fs1);     % ������Ӧ���䷨

% ��ͨ�˲���2
Wp2 = [19000 23000]; Ws2 = [18000 24000];
[n2, Wp2_analog] = cheb2ord(2*pi*Wp2, 2*pi*Ws2, Rp, Rs, 's');  
[b2_analog, a2_analog] = cheby2(n2, Rs, Wp2_analog, 'bandpass', 's'); 
[b2, a2] = impinvar(b2_analog, a2_analog, fs1);

% ��ͨ�˲���3
Wp3 = [27000 33000]; Ws3 = [26000 34000];
[n3, Wp3_analog] = cheb2ord(2*pi*Wp3, 2*pi*Ws3, Rp, Rs, 's');  
[b3_analog, a3_analog] = cheby2(n3, Rs, Wp3_analog, 'bandpass', 's'); 
[b3, a3] = impinvar(b3_analog, a3_analog, fs1);

% ��ʾƵ����Ӧ
[h1, w1] = freqz(b1, a1);
[h2, w2] = freqz(b2, a2);
[h3, w3] = freqz(b3, a3);

figure(4);
subplot(3,1,1); plot(w1*fs1/(2*pi), abs(h1)); xlabel('Ƶ��'); ylabel('����'); title('�б�ѩ��2�ʹ�ͨ�˲���1');
subplot(3,1,2); plot(w2*fs1/(2*pi), abs(h2)); xlabel('Ƶ��'); ylabel('����'); title('�б�ѩ��2�ʹ�ͨ�˲���2');
subplot(3,1,3); plot(w3*fs1/(2*pi), abs(h3)); xlabel('Ƶ��'); ylabel('����'); title('�б�ѩ��2�ʹ�ͨ�˲���3');
lv11 = filter(b1, a1, combined_signal);             % �ź��˲� 
lv12 = filter(b2, a2, combined_signal);
lv13 = filter(b3, a3, combined_signal);


%%% 6. ��� %%%
% ���˳����źŽ��н��
s10 = lv11 .* cos(2 * pi * fc1 * t)';
s20 = lv12 .* cos(2 * pi * fc2 * t)';
s30 = lv13 .* cos(2 * pi * fc3 * t)';

% ����ź�Ƶ��
Z10 = fftshift(fft(s10));  
Z20 = fftshift(fft(s20));  
Z30 = fftshift(fft(s30));
figure(6);
subplot(3,2,1); plot(t, s10); xlabel('��λ��s'); ylabel('����'); title('����źŲ���1');
subplot(3,2,2); plot(f, abs(Z10)); xlabel('��λ��HZ'); ylabel('����'); title('����ź�Ƶ��1'); 
subplot(3,2,3); plot(t, s20); xlabel('��λ��s'); ylabel('����'); title('����źŲ���2');
subplot(3,2,4); plot(f, abs(Z20)); xlabel('��λ��HZ'); ylabel('����'); title('����ź�Ƶ��2'); 
subplot(3,2,5); plot(t, s30); xlabel('��λ��s'); ylabel('����'); title('����źŲ���3');
subplot(3,2,6); plot(f, abs(Z30)); xlabel('��λ��HZ'); ylabel('����'); title('����ź�Ƶ��3');


%%% 7. �������źŵ�ͨ�˲������� %%%
% ��Ƶ�ͨ�˲���
Wp = 1500 / (fs / 2);                               % ��ֹƵ��
Ws = 8000 / (fs / 2);                               % ���Ƶ��
Rp = 0.5; Rs = 40;                                  % �Ʋ���˥��
[n, Wn] = cheb2ord(Wp, Ws, Rp, Rs);                 % �����˲�������С�����ͽ�ֹƵ��
[b, a] = cheby2(n, Rs, Wn);

% �����˲�����Ӧ
figure(7);
[H, f1] = freqz(b, a, 1024, fs);                     % ����Ƶ����Ӧ
plot(f1, abs(H));                                    % ���Ʒ�Ƶ��Ӧ
xlabel('Ƶ�� (Hz)');                                % �����ע
ylabel('����');                                     % �����ע
title('�б�ѩ��2�͵�ͨ�˲���');                     % ͼ�����

% �˲�����
lv1_filtered = filter(b, a, s10);
lv2_filtered = filter(b, a, s20);
lv3_filtered = filter(b, a, s30);

% ��ʾ��ͨ�˲�����ź�ʱ���Ƶ��ͼ
figure(8);
subplot(3,2,1); plot(t, lv1_filtered); xlabel('��λ��s'); ylabel('����'); title('��ͨ�˲�����ź�1');
subplot(3,2,2); plot(f, abs(fftshift(fft(lv1_filtered, N)))); xlabel('��λ��HZ'); ylabel('����'); title('��ͨ�˲�����ź�1Ƶ��');
subplot(3,2,3); plot(t, lv2_filtered); xlabel('��λ��s'); ylabel('����'); title('��ͨ�˲�����ź�2');
subplot(3,2,4); plot(f, abs(fftshift(fft(lv2_filtered, N)))); xlabel('��λ��HZ'); ylabel('����'); title('��ͨ�˲�����ź�2Ƶ��');
subplot(3,2,5); plot(t, lv3_filtered); xlabel('��λ��s'); ylabel('����'); title('��ͨ�˲�����ź�3');
subplot(3,2,6); plot(f, abs(fftshift(fft(lv3_filtered, N)))); xlabel('��λ��HZ'); ylabel('����'); title('��ͨ�˲�����ź�3Ƶ��');
%sound(lv1_filtered, fs);
sound(lv2_filtered, fs);
%sound(lv3_filtered, fs);


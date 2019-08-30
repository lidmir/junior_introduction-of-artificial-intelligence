clear;
clc;

A=importdata('trainData.txt');  
A=A'
[mm1,nn1]=size(A);
B=importdata('trainOutput.txt');
[m,n]=size(B);%n为数据总个数
x1=1:1:m*n;

[Pn,minp,maxp,Tn,mint,maxt]=premnmx(A,B);%把网络的输入数据和目标数据归一化为Pn和Tn[-1,1]
net=newff(minmax(Pn),[24,12,1],{'tansig','tansig','purelin'},'traingdx'); 
net.trainParam.epochs = 1000;  
net.trainparam.goal = 0.000000000001 ;
net.trainParam.lr = 0.0000000000001;

net=init(net);
net = train(net,Pn,Tn); 
S = sim(net,Pn); 
O=postmnmx(S,mint,maxt);   
[outputn,outputps]=mapminmax(B);

%神经网络预测
figure;
C=importdata('testData.txt');  
[mm,nn]=size(C);
D=importdata('testOutput.txt');
[m,n]=size(D);%n为数据总个数
x2=2005:1:2016;
[Cn,mina,maxa,Dn,minb,maxb]=premnmx(C,D);
Cnn=tramnmx(C,mina,maxa);%测试数据的归一化处理
Sn=sim(net,Cnn);


%网络输出反归一化  
BPoutput=mapminmax('reverse',Sn,outputps); 
%% 结果分析 
  
error=BPoutput-B; 
plot(error,'-*')  
title('神经网络预测误差') 
errorsum=sum(abs(error))  

figure  
On=postmnmx(Sn,minb,maxb);   %逆归一化
plot(x2,D,x2,On); %实际值和预测值的曲线
legend('实际值','预测值')   
title('12月份黄连价格预测') 
xlabel('年'); 
ylabel('元/克');
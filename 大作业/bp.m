clear;
clc;

A=importdata('trainData.txt');  
[mm1,nn1]=size(A);
B=importdata('trainOutput.txt');
[m,n]=size(B);%nΪ�����ܸ���
x1=1:1:m*n;

[Pn,minp,maxp,Tn,mint,maxt]=premnmx(A,B);%��������������ݺ�Ŀ�����ݹ�һ��ΪPn��Tn[-1,1]
net=newff(minmax(Pn),[24,12,1],{'tansig','tansig','purelin'},'traingdx'); 
net.trainParam.epochs = 3000;  
net.trainparam.goal = 0.0000000000000001 ;
net.trainParam.lr = 0.00000000000000001;

net=init(net);
net = train(net,Pn,Tn); 
S = sim(net,Pn); 
O=postmnmx(S,mint,maxt);   
[outputn,outputps]=mapminmax(B);

%������Ԥ��
figure;
C=importdata('testData.txt');  
[mm,nn]=size(C);
D=importdata('testOutput.txt');
[m,n]=size(D);%nΪ�����ܸ���
x2=1:1:m*n;
[Cn,mina,maxa,Dn,minb,maxb]=premnmx(C,D);
Cnn=tramnmx(C,mina,maxa);%�������ݵĹ�һ������
Sn=sim(net,Cnn);


%�����������һ��  
BPoutput=mapminmax('reverse',Sn,outputps); 
%% ������� 
  
error=BPoutput-B; 
plot(error,'-*')  
title('������Ԥ�����') 
errorsum=sum(abs(error))  

figure  
On=postmnmx(Sn,minb,maxb);   %���һ��
plot(x2,D,x2,On); %ʵ��ֵ��Ԥ��ֵ������
legend('ʵ��ֵ','Ԥ��ֵ')   
title('2016�����۸�Ԥ��') 
xlabel('2016'); 
ylabel('Ԫ/��');
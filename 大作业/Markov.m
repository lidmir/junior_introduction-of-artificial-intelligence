clear;
clc;

%数据选择
B=importdata('markovData.txt'); 
B=B';
B=B(:);
t=length(B);  
u=2:1:t
A(u-1,1)=B(u,1)-B(u-1,1);   
m=length(A);

%统计各状态出现频数
E1=0; E2=0; E3=0; E4=0; E5=0;
for  j=1:1:m     
   if  A(j)>2
   E1=E1+1;
   elseif A(j)<=2 &&A(j)>0
   E2=E2+1;
   elseif A(j)>-1&& A(j)<=0
   E3=E3+1;
   elseif A(j)<=-1&& A(j)>-2
   E4=E4+1;
   elseif A(j)<=-2
   E5=E5+1;
   end
 end
%25种状态转移频数统计
p00=0;p01=0;p02=0;p03=0;p04=0;                       
p10=0;p11=0;p12=0;p13=0;p14=0;
p20=0;p21=0;p22=0;p23=0;p24=0;
p30=0;p31=0;p32=0;p33=0;p34=0;
p40=0;p41=0;p42=0;p43=0;p44=0;
%E1至其他状态 
for j=1:1:(m-1) 
    if A(j)>2&&A(j+1)>2
        p00=p00+1;
    elseif A(j)>2&&(A(j+1)<=2&&A(j+1)>0)
        p01=p01+1;
    elseif A(j)>2&&(A(j+1)<=0&&A(j+1)>-1)
        p02=p02+1;
    elseif A(j)>2&&(A(j+1)<=-1&&A(j+1)>-2) 
        p03=p03+1; 
    elseif A(j)>2&&(A(j+1)<=-2) 
        p04=p04+1; 
%E2至其他状态
    elseif (A(j)<=2 &&A(j)>0)&&A(j+1)>2
        p10=p10+1;
    elseif (A(j)<=2 &&A(j)>0)&&(A(j+1)<=2&&A(j+1)>0)
        p11=p11+1;
    elseif (A(j)<=2 &&A(j)>0)&&(A(j+1)<=0&&A(j+1)>-1)
        p12=p12+1;
    elseif (A(j)<=2 &&A(j)>0)&&(A(j+1)<=-1&&A(j+1)>-2) 
        p13=p13+1; 
    elseif (A(j)<=2 &&A(j)>0)&&(A(j+1)<=-2) 
        p14=p14+1; 
%E3至其他状态
    elseif (A(j)>-1&& A(j)<=0)&&A(j+1)>2
        p20=p20+1;
    elseif (A(j)>-1&& A(j)<=0)&&(A(j+1)<=2&&A(j+1)>0)
        p21=p21+1;
    elseif (A(j)>-1&& A(j)<=0)&&(A(j+1)<=0&&A(j+1)>-1)
        p22=p22+1;
    elseif (A(j)>-1&& A(j)<=0)&&(A(j+1)<=-1&&A(j+1)>-2) 
        p23=p23+1; 
    elseif (A(j)>-1&& A(j)<=0)&&(A(j+1)<=-2) 
        p24=p24+1; 
%E4至其他状态
    elseif (A(j)<=-1&& A(j)>-2)&&A(j+1)>2
        p30=p30+1;
    elseif (A(j)<=-1&& A(j)>-2)&&(A(j+1)<=2&&A(j+1)>0)
        p31=p31+1;
    elseif (A(j)<=-1&& A(j)>-2)&&(A(j+1)<=0&&A(j+1)>-1)
        p32=p32+1;
    elseif (A(j)<=-1&& A(j)>-2)&&(A(j+1)<=-1&&A(j+1)>-2) 
        p33=p33+1; 
    elseif (A(j)<=-1&& A(j)>-2)&&(A(j+1)<=-2) 
        p34=p34+1; 
%E5至其他状态
    elseif (A(j)<=-2)&&A(j+1)>2
        p40=p40+1;
    elseif (A(j)<=-2)&&(A(j+1)<=2&&A(j+1)>0)
        p41=p41+1;
    elseif (A(j)<=-2)&&(A(j+1)<=0&&A(j+1)>-1)
        p42=p42+1;
    elseif (A(j)<=-2)&&(A(j+1)<=-1&&A(j+1)>-2) 
        p43=p43+1; 
    elseif (A(j)<=-2)&&(A(j+1)<=-2) 
        p44=p44+1; 
    end
end
%一步状态转移频数矩阵
p0=[p00,p01,p02,p03,p04
    p10,p11,p12,p13,p14
    p20,p21,p22,p23,p24
    p30,p31,p32,p33,p34
    p40,p41,p42,p43,p44] 
%一步转移状态概率矩阵 
if A(m)>1
    E1=E1-1;
elseif A(m)<=1&&A(m)>0
    E2=E2-1;
elseif A(m)<=0&&A(m)>-1  
    E3=E3-1;
elseif A(m)<=-1&&A(m)>-2  
    E4=E4-1;
elseif A(m)<=-2
    E5=E5-1;
end
p1=[p00/E1,p01/E1,p02/E1,p03/E1,p04/E1
    p10/E2,p11/E2,p12/E2,p13/E2,p14/E2
    p20/E3,p21/E3,p22/E3,p23/E3,p24/E3
    p30/E4,p31/E4,p32/E4,p33/E4,p34/E4
    p40/E5,p41/E5,p42/E5,p43/E5,p44/E5] 
for i=1:1:5
    if(E1==0) 
        p1(1,:)=0;
    end
    if(E2==0) 
        p1(2,:)=0;
    end
    if(E3==0) 
        p1(3,:)=0;
    end
    if(E4==0) 
        p1(4,:)=0;
    end
    if(E5==0) 
        p1(4,:)=0;
    end
end
             
w0=[0,0,1,0,0]
 %多步转移状态概率矩阵                
w1=w0*p1;
w2=w1*p1;
w3=w2*p1;
w4=w3*p1;
w5=w4*p1;
w6=w5*p1;
w7=w6*p1;
w8=w7*p1;
w9=w8*p1;
w10=w9*p1;
w11=w10*p1;
w12=w11*p1;
w=[w1;w2;w3;w4;w5;w6;w7;w8;w9;w10;w11;w12]
temp=B(t,1);
X=zeros(1,12);
max=0;
    for j=1:1:5
    	if (max<w(1,j))
            max=w(1,j);
            switch j
                case 1
                    X(1,1)=temp+2;
                case 2
                    X(1,1)=temp+1;
                case 3
                    X(1,1)=temp;
                case 4
                    X(1,1)=temp-1;
                case 5
                    X(1,1)=temp-2;
            end
        end
    end
for i=2:1:12
    max=0;
    for j=1:1:5
    	if max<w(i,j)
            max=w(i,j);
            switch j
                case 1
                    X(1,i)=X(1,i-1)+2;
                case 2
                    X(1,i)=X(1,i-1)+1;
                case 3
                    X(1,i)=X(1,i-1);
                case 4
                    X(1,i)=X(1,i-1)-1;
                case 5
                    X(1,i)=X(1,i-1)-2;
            end
        end
    end
end
x2=1:1:12;
C=[78,76,76,75,72,72,72,70,66,66,66,62];
X
plot(x2,X,x2,C);
legend('预测值','实际值')
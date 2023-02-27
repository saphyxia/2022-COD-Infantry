#include "ahr.h"
#include "Matrix.h"

float mygetqval[9];	//���ڴ�Ŵ�����ת�����������
					 
									 
bmi088_real_data_t bmi088_real_data;
ist8310_real_data_t ist8310_real_data;
kalman_t kalman;									 
													
volatile float q0, q1, q2, q3,w1,w2,w3; // ȫ����Ԫ�������ٶȲ������
volatile uint32_t lastUpdate, now; // �������ڼ��� ��λ us									 


float P[49]={   0.0001,0,0,0,0,0,0,
	            0,0.0001,0,0,0,0,0,
			    0,0,0.0001,0,0,0,0,
			    0,0,0,0.0001,0,0,0,
			    0,0,0,0,0.0002,0,0,
			    0,0,0,0,0,0.0002,0,
			    0,0,0,0,0,0,0.0002  };//��ʼ����ֵ����ο��������︳ֵ����ƫ��

  float Q[49]={ 0.0001,0,0,0,0,0,0,
                0,0.0001,0,0,0,0,0,
				0,0,0.0001,0,0,0,0,
				0,0,0,0.0001,0,0,0,
				0,0,0,0,0.0005,0,0,		 
				0,0,0,0,0,0.0005,0,	 
				0,0,0,0,0,0,0.0005  };//����ڽǶ����ƫ������һ���������ʱ����ʱ���Ǿ͸������Ų���ֵ�������ʱ����ֵ��ʵ��ֵ���������СQ���ڲο�������  q��λΪ0.00001�ĵ�λ����
			    
float R[36]={   0.0003,0,0,0,0,0,
                0,0.0003,0,0,0,0,
			    0,0,0.0003,0,0,0,
				0,0,0,0.0002,0,0,
				0,0,0,0,0.0002,0,
				0,0,0,0,0,0.0002    };//����ڽǶ����ƫ������һ��Ԥ��ֵʱ����ʱ���Ǿ͸�������Ԥ��ֵ�������ʱ����ֵ��Ԥ��ֵ���������СR���ڲο�������  R�ڼ��ټ�У׼ʱΪ2����λ���� ������У׼ʱΪ��λ���󣬿��������ο�
			   		
float A[49],B[49],E[42],F1[36],X[49],Z[49],Ht[42],Ft[49],K[42],O[49],T[6],F[49],Y[7],P1[49],U1[36],U1t[36],D1[36],X1[36],X2[36];

float H[42]={   0,0,0,0,0,0,0,
			    0,0,0,0,0,0,0,
			    0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,													   												  
			    0,0,0,0,0,0,0,  };

float I[49]={   1,0,0,0,0,0,0,
                0,1,0,0,0,0,0,
                0,0,1,0,0,0,0,
                0,0,0,1,0,0,0,
                0,0,0,0,1,0,0,
                0,0,0,0,0,1,0,
                0,0,0,0,0,0,1   };

void GetPitchYawGxGyGz(void)
{
   
}


void IMU_getYawPitchRoll(volatile float * angles) 
{
	float q[4]; //����Ԫ��
  
    IMU_getQ(q); //����ȫ����Ԫ��

    angles[0] = -atan2(2 * q[1] * q[2] + 2 * q[0] * q[3], -2 * q[2]*q[2] - 2 * q[3] * q[3] + 1)* 180/M_PI; // yaw 
    angles[1] = -asin(-2 * q[1] * q[3] + 2 * q[0] * q[2])* 180/M_PI; // pitch
    angles[2] = atan2(2 * q[2] * q[3] + 2 * q[0] * q[1], -2 * q[1] * q[1] - 2 * q[2] * q[2] + 1)* 180/M_PI; // roll
    // if(angles[0]<0)angles[0]+=360.0f;  //�� -+180��  ת��0-360��
}

void IMU_getQ(volatile float * q) 
{
    IMU_getValues(mygetqval);	 

    //�������ǵĲ���ֵת�ɻ���ÿ��
    //���ٶȺʹ����Ʊ��� ADCֵ������Ҫת��
    IMU_AHRSupdate(mygetqval[3] * M_PI/180, mygetqval[4] * M_PI/180, mygetqval[5] * M_PI/180,
    mygetqval[0], mygetqval[1], mygetqval[2], mygetqval[6], mygetqval[7], mygetqval[8]);
        
    q[0] = q0; //���ص�ǰֵ
    q[1] = q1;
    q[2] = q2;
    q[3] = q3;
}


void IMU_getValues(volatile float * values) 
{
    BMI088_read(bmi088_real_data.gyro, bmi088_real_data.accel, &bmi088_real_data.temp);   //ѭ����ȡ���ټơ������ǡ��¶ȵ�ֵ
    ist8310_read_mag(ist8310_real_data.mag);  //��ȡ�����Ƶ�ֵ

    values[0] = bmi088_real_data.accel[0];
    values[1] = bmi088_real_data.accel[1];
    values[2] = bmi088_real_data.accel[2];
    values[3] = bmi088_real_data.gyro[0] / 16.4f;
    values[4] = bmi088_real_data.gyro[1] / 16.4f;
    values[5] = bmi088_real_data.gyro[2] / 16.4f;
    values[6] = ist8310_real_data.mag[0];
    values[7] = ist8310_real_data.mag[1];
    values[8] = ist8310_real_data.mag[2];

}


void IMU_AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) 
{
    float norm;
    float bx, bz;
    float vx, vy, vz, wx, wy, wz;
    float g=9.79973;
    float Ha1,Ha2,Ha3,Ha4,Hb1,Hb2,Hb3,Hb4;
    float e1,e2,e3,e4,e5,e6;
    float halfT;

    float q0q0 = q0*q0;
    float q0q1 = q0*q1;
    float q0q2 = q0*q2;
    float q0q3 = q0*q3;
    float q1q1 = q1*q1;
    float q1q2 = q1*q2;
    float q1q3 = q1*q3;
    float q2q2 = q2*q2;
    float q2q3 = q2*q3;
    float q3q3 = q3*q3;

    bx = 0.5513;  //����ط��Ǹõ����Ĵų�
    bz = 0.8353; 
    /*************����������ʱ��********************/	
    now = Get_Time_Micros();  //   ��ȡ��λ  us
    if(now<lastUpdate)  //��ʱ�����
    { 
    halfT =  ((float)(now + (0xffffffff- lastUpdate)) / 2000000.0f);
    }
    else	{
    halfT =  ((float)(now - lastUpdate) / 2000000.0f);
    }
    lastUpdate = now;	//����ʱ��

    /*************����������ʱ��********************/		
    norm = invSqrt(ax*ax + ay*ay + az*az); //���ٶ�ƽ������֮һ
    ax = ax * norm*g; //���ٶȹ�һ��
    ay = ay * norm*g;
    az = az * norm*g;

    norm = invSqrt(mx*mx + my*my + mz*mz);   //������ƽ������֮һ        
    mx = mx * norm; //�ų�ǿ�ȹ�һ��
    my = my * norm;
    mz = mz * norm;

    gx=gx-w1;//���ٶ� w1 w2 w3 �ǽ��ٶȲ������
    gy=gy-w2;//���ٶ� w1 w2 w3 �ǽ��ٶȲ������
    gz=gz-w3;//���ٶ� w1 w2 w3 �ǽ��ٶȲ������

    Ha1=2*-q2*g; /*�����ת�����һ��Ԥ�ȸ�ֵ���ѣ��Ͳ����ھ�����ֱ�Ӽ���*/
    Ha2=2*q3*g;
    Ha3=2*-q0*g;
    Ha4=2*q1*g;	

    Hb1=2*(bx*q0-bz*q2);
    Hb2=2*(bx*q1+bz*q3);
    Hb3=2*(-bx*q2-bz*q0);
    Hb4=2*(-bx*q3+bz*q1);
    //��ת״̬����ĸ�ֵ
    H[0]=Ha1;	H[1]= Ha2;	H[2]= Ha3;	H[3]= Ha4;
    H[7]= Ha4;	H[8]=-Ha3;	H[9]= Ha2;	H[10]=-Ha1;
    H[14]=-Ha3;	H[15]=-Ha4;	H[16]= Ha1;	H[17]= Ha2;
    H[21]= Hb1;	H[22]= Hb2;	H[23]= Hb3;	H[24]= Hb4;
    H[28]= Hb4;	H[29]=-Hb3;	H[30]= Hb2;	H[31]=-Hb1;
    H[35]=-Hb3;	H[36]=-Hb4;	H[37]= Hb1;	H[38]= Hb2;

    //����ط���Ҫ������״̬���£�����֮��ļ��㣬���е��㷨���档��һ���ڼ�����ת����ĺ��棬��������һ��Ч�����ܹ������Ǹ�
    q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
    q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
    q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
    q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;  

    //��Ԫ����һ
    norm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q0 = q0 * norm;
    q1 = q1 * norm;
    q2 = q2 * norm;
    q3 = q3 * norm;
    //F����Ϊ��dt���ֺ�ľ���
    F[0]=1;			F[1]=-gx*halfT;	F[2]=-gz*halfT;	F[3]=-gz*halfT;	F[4]=0; 	F[5]=0; 	F[6]=0;
    F[7]=gx*halfT;	F[8]=1;			F[9]=gz*halfT;	F[10]=-gy*halfT;F[11]=0; 	F[12]=0; 	F[13]=0;
    F[14]=gy*halfT;	F[15]=-gz*halfT;F[16]=1;		F[17]=gx*halfT;	F[18]=0; 	F[19]=0;	F[20]=0;
    F[21]=gz*halfT;	F[22]=gy*halfT;	F[23]=-gx*halfT;F[24]=1;		F[25]=0; 	F[26]=0; 	F[27]=0;
    F[28]=0;		F[29]=0;		F[30]=0;		F[31]=0;		F[32]=1;	F[33]=0;	F[34]=0;
    F[35]=0;		F[36]=0;		F[37]=0;		F[38]=0;		F[39]=0;	F[40]=1;	F[41]=0;
    F[42]=0;		F[43]=0;		F[44]=0;		F[45]=0;		F[46]=0;	F[47]=0;	F[48]=1;

    /***************�ڶԳ�ʼ����H,F׼�����Ժ󣬿�ʼ�������˲�*********/
    //�ڶ���  ����������Э�������  Pk- = A*Pk-1*AT + Q  
    //ע�⣬����������Բ���DSP�⣬����̫�鷳�ˣ���д�ܶ��������������ʹ�������Ͽ�Դ��������㺯���������ϸ��վ���������ɽ�������
    MatrixMultiply(F,7,7,P,7,7,A );	//A=F*P  �����Ǵӹ�ʽ���潫F���ӦA��״̬��ת����A������F���Գ�ʼЭ�����һ����ת���󣬲�Ҫ�����ˡ� ����F����P����һ��7X7�������� �˻�Ҳ��7x7��һ������
    MatrixTranspose(F,7,7,Ft);	  //Fת��F'  ��ʽ����ת�ã������Ƚ�F��ת��������
    MatrixMultiply(A,7,7,Ft,7,7,B); // B=F*P*F'   B������ A*Pk-1*AT�˷���һ����ת����
    MatrixAdd( B,Q,P1,7,7 ); //   Pk- = A*Pk-1*AT + Q   �������㹫ʽ  ��B�������Q����ֵ����P1���������P1��һ��7x7�ľ����ڿ��������㵱�У�һ��Ҫ�ǵ�ÿһ������ľ������У���Ȼ���������ʱ�����׳�����

    //����������������ļ�������ǿ�����������Ϊ��Ҫ�Ĳ���	 K(k) = Pk-*H(k)T/(H*Pk-*HT + VK*R*VKT)   ����VkΪ��λ����Ϊ1
    // H*Pk-*HT + VK*R*VKT + R����
    MatrixTranspose(H,6,7,Ht);	  //Ht = H'  H��һ��6X7����Ϊʲô���������F����һ����7x7�ľ����أ���Ϊ�����H������һ���ſɱȾ�������Ԫ�����ſɱȾ��������6x7��������������һ��3x4����F��һ��4x4����һ������
    MatrixMultiply(P1,7,7,Ht,7,6,E ); //E=P*H'  �����p���������Э�������E������P��Hת�þ���ĳ˻���ת���� һ��7x7�������6x7�������յó���Ҳ��һ��6x7����
    MatrixMultiply(H,6,7,E,7,6,F1 ); //	 F1=H*P*H'  ����������(AB)C=A(BC)  ����F1�������H*Pk-*HT�ĳ˻�  6X6
    MatrixAdd(F1,R,X,6,6 );           //X=F1+R   �������R����һ���ĳ����������

    //����Ҳ����⿨��������ϵ������ �������˲���UD�ֽ��˲��㷨
    //����ǰ��UD�ֽ� ��ֹ��ɢ  P=UD[U]T  UD�ֽⷨ�����������㿴�� U��D�ֱ���һ�������Ǿ���ͶԽǾ��� ��һ���������ԽǺ����Ƿֽ�
    UD(X,6,U1,D1);  //��X����ֳ���������Խ���	   
    MatrixTranspose(U1,6,6,U1t);//����������ת��
    MatrixMultiply(U1,6,6,D1,6,6,X1); //X1=U1*D1  ��ʽ����ǰ�벿��
    MatrixMultiply(X1,6,6,U1t,6,6,X2); //X2=U1*D1*U1t  ����ԭ����
    MatrixInverse(X2,6,0);	//��X2����  ������������Ϊ������A/B�͵���A*B���棬��Ҫע�����ֻ�з����Ȳ������棬���������ܹ������һ����һ����������ǳ�����󣬿�Դ�����������ķ���  ��ABAT=A,��B��A����  X2��һ��6x6����
    MatrixMultiply(E,7,6,X2,6,6,K ); //����������ϵ��K  Ϊһ��7x6�ľ���

    /*****************************************************************/	

    //״̬��ת����֮���Ԥ��ֵ
    vx = 2*(q1q3 - q0q2)*g;//ax ��Ԥ��ֵ
    vy = 2*(q0q1 + q2q3)*g;//ay ��Ԥ��ֵ
    vz = (q0q0 - q1q1 - q2q2 + q3q3)*g;//az ��Ԥ��ֵ
    wx = 2*bx*(0.5f - q2q2 - q3q3) + 2*bz*(q1q3 - q0q2);//mx ��Ԥ��ֵ
    wy = 2*bx*(q1q2 - q0q3) + 2*bz*(q0q1 + q2q3);//my ��Ԥ��ֵ
    wz = 2*bx*(q0q2 + q1q3) + 2*bz*(0.5f - q1q1 - q2q2); //mz ��Ԥ��ֵ 


    e1=ax-vx;
    e2=ay-vy;
    e3=az-vz;//���ٶ�ƫ��
    e4=mx-wx;
    e5=my-wy;
    e6=mz-wz;//�ų�ƫ��

    T[0]=e1;
    T[1]=e2;
    T[2]=e3;
    T[3]=e4;
    T[4]=e5;
    T[5]=e6; //ƫ�ֵ����T����׼����һ���ļ���

    //���Ĳ� ������� Xk = Xk- + K(k)(Zk - Hxk)  
    MatrixMultiply(K,7,6,T,6,1,Y );  //YΪһ��7x1����

    //������Ԫ��
    q0= q0+Y[0];//������Ԫ��
    q1= q1+Y[1];//������Ԫ��
    q2= q2+Y[2];//������Ԫ��
    q3= q3+Y[3];//������Ԫ��
    w1= w1+Y[4];//���½��ٶ�ƫ��
    w2= w2+Y[5];//���½��ٶ�ƫ��
    w3= w3+Y[6];//���½��ٶ�ƫ��

    //���岽���������Э������� Pk = (I - K(k)*H)*Pk
    MatrixMultiply(K,7,6,H,6,7,Z); //K(k)*H��ֵ��Z		Ϊ7x7����
    MatrixSub(I,Z,O,7,7 );//O=I-K*H O����Ϊ��λ����I��ȥZ����  7x7����
    MatrixMultiply(O,7,7,P1,7,7,P);  //���0����P1�ó�P ����״̬ p1Ϊǰ���������Э�������P�տ�ʼΪ�Զ���ֵ���ٴε��ڴ˿�״̬��֮���������£����ڴ�ǰ״̬���������˲�����

    /*��Ԫ����һ�� ��֤��һ��ʹ�õ���Ԫ���ǵ�λ��Ԫ�����ó���Ԫ�������Խ��������ں�*/
    norm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q0 = q0 * norm;
    q1 = q1 * norm;
    q2 = q2 * norm;
    q3 = q3 * norm;

}

float invSqrt(float x)
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

uint32_t Get_Time_Micros(void)
{
	return TIM2->CNT;
}

















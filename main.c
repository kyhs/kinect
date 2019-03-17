#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"


int ude_y=0;//�r�̂���ʒu��y���W
int body_width[765];
double ave[131];
int sitamax,uemin;
int leftmin,rightmax;
int ballx=0;
int balloff=1;
int v=7;

int test(void);
int bodyWidth(img *user_img);
int bodyHeight(img *user_img);
int beamMode(int i);//i:�t���[���ԍ�
void createBeam(int i);
//void ballRoll();�o�͂�
void outputAndBall(int i);
void okuyuki(int i);//���s�����͑��ϐ��ł���Ă���̂�void�ɂ���///�Ȃ��A���s������double�̔z��ave[]�ɓ����Ă���
void small(int i);

int main(void){
	char image_filename[256];
	char user_filename[256];
	char output_filename[256];
	int i;
	int plusedflame=0,check=0;
	img *image_img,*user_img;
	/*for(i=0;i<=150;i++){
		sprintf(image_filename, "image/%05d.jpg", i);
		image_img=(img *)malloc(sizeof(img));
		ReadImg(image_filename,image_img);
		sprintf(output_filename,"output/%05d.png",i);
		WritePng(output_filename, image_img);
		free(image_img);
		if(i%5==0)
			printf("%d\n",i);
	}*/
	
	for(i=455;i<=765;i++){//�������烁�C��151~~~~~~~~~~
		if(i%50==0)//�i�s���\��
			printf("now...%d\n",i);

		user_img=(img *)malloc(sizeof(img));
		sprintf(user_filename,"user/%05d.jpg",i);
		ReadImg(user_filename,user_img);
		body_width[i]=bodyWidth(user_img);
		bodyHeight(user_img);//������͒P�Ɏ��s���邾��(���ϐ���ω�������)�ŁA�Ԃ�l�͗��p���Ȃ�
		if(i>155&&i<226){//�ŏ��̃W�����v
			outputAndBall(i);
		}
		
		//�r�[���̂Ƃ���́A�{�[����`�悵����A�ォ��r�[�����㏑������::outputAndBall->createBeam<====�t///****
		if(i==227||i==285||i==320||i==455||i==500||i==650){
			ballx=0;
			balloff=1;
		}
		if(i==227)
			v=6;
		if(i>=227&&i<285){
			
			outputAndBall(i);
			if((body_width[i]-body_width[i-20]>40)&&(i-check>20)){
				printf("ude ok :frame %d\n",i);
				i=beamMode(i);
				check=i;
			}
			else
				outputAndBall(i);
		}//�����܂�ok�i�v���Ԓ���, �r�[���ƃ{�[���̓����蔻��j

		if(i==285)
			v=25;
		if(i>=285&&i<320)//�W�����v
			outputAndBall(i);
		if(i==300)
			balloff=0;

		if(i>=320&&i<=453){//�O�i�A�W�����v�A��i
			v=5;
			okuyuki(i);
			printf("depth:%lf\n",ave[i-320]);
			//outputAndBall(i);
			if(i>350&&ave[i-320]<100){//�ύX�O(ave[i-320]-ave[i-320-15]>5)
				printf("zensin:%d\n",i);
				small(i);
			}
		}

		if(i>=455&&i<500){
			v=5;
			//outputAndBall(i);
			
			if((body_width[i]-body_width[i-20]>40)&&(i-check>20)){
				printf("ude ok :frame %d\n",i);
				i=beamMode(i);
				check=i;
			}
			else
				outputAndBall(i);
		}

		if(i>=500&&i<580){
			if(v!=0)
				v=5;
			outputAndBall(i);
			if(i==540)
				v=0;
		}

		if(i>=580&&i<650){
			//outputAndBall(i);
			if((body_width[i]-body_width[i-20]>40)&&(i-check>20)){
				printf("ude ok :frame %d\n",i);
				i=beamMode(i);
				check=i;
			}
		}

		if(i==650){//ending650���ڂ݂̂��g�p����ȍ~
			int j=i;//j:�o�͗p�t���[���ԍ�
			int h,w;
			while(1){
				if(j>=650&&j<670){
					image_img=(img *)malloc(sizeof(img));
					sprintf(image_filename,"image/%05d.jpg",i);
					ReadImg(image_filename,image_img);
					for(h=0;h<160;h++){
						for(w=0;w<680;w++){
							image_img->data[h][w].r=0;
							image_img->data[h][w].g=0;
							image_img->data[h][w].b=0;
						}
					}
					sprintf(output_filename,"output/%05d.png",j);
					WritePng(output_filename, image_img);
					free(image_img);
				}
				else if(j>=670&&j<690){
					image_img=(img *)malloc(sizeof(img));
					sprintf(image_filename,"image/%05d.jpg",i);
					ReadImg(image_filename,image_img);
					for(h=0;h<320;h++){
						for(w=0;w<680;w++){
							image_img->data[h][w].r=0;
							image_img->data[h][w].g=0;
							image_img->data[h][w].b=0;
						}
					}
					sprintf(output_filename,"output/%05d.png",j);
					WritePng(output_filename, image_img);
					free(image_img);
				}
				else if(j>=690&&j<710){
					image_img=(img *)malloc(sizeof(img));
					sprintf(image_filename,"image/%05d.jpg",i);
					ReadImg(image_filename,image_img);
					for(h=0;h<480;h++){
						for(w=0;w<680;w++){
							image_img->data[h][w].r=0;
							image_img->data[h][w].g=0;
							image_img->data[h][w].b=0;
						}
					}
					sprintf(output_filename,"output/%05d.png",j);
					WritePng(output_filename, image_img);
					free(image_img);
				}
				j++;
				if(j==710)
					break;
			}
			break;

		}
		free(user_img);
	}
	printf("end");
	
	return 0;
}

int test(void){//���X���C���������A���͂ǂ�������Ă΂�Ă��Ȃ�
	char image_filename[256];
	char depth_filename[256];
	char user_filename[256];
	char output_filename[256];
	int i;
	//int body_width,pre_body_width=0;
	img *image_img,*user_img,*depth_img,*output_img;
	int plusedflame=0,check=0;
	
	
	for(i=0;i<=765;i++){//beam:::~285
	
		user_img=(img *)malloc(sizeof(img));
		image_img=(img *)malloc(sizeof(img));
		/*���ꂼ��̃t�@�C������ǂݍ���ł���(for���[�v����邽�тɏ㏑��)*/
		sprintf(image_filename, "image/%05d.jpg", i);
		sprintf(depth_filename,"depth/%05d.jpg",i);
		sprintf(user_filename,"user/%05d.jpg",i);
		
		ReadImg(user_filename,user_img);//�l���̈�̉摜��ǂݍ���
		////////////////////////////////////////////////////ReadImg(image_filename,image_img);
		body_width[i]=bodyWidth(user_img);//�̂̕������߂�
					//if(i>20) printf("%d\n",body_width[i-20]);
		
		//////////////////////height��check
		//////////////////////
		//printf("%d\n",bodyHeight(user_img));
		//////////////////////
		

		//	outputAndBall(i);
		if(i>155&&i<220)
			outputAndBall(i);
		////�e�X�g���ԒZ�k�̂��߁A��U�R�����g�A�E�g
		if(i==155)
			printf("NOT skip ball\n");

		
		if(i>=114&&(body_width[i]-body_width[i-20]>40)&&(i-check>20)){//�肪�˂��o���ꂽ��(20�t���[���Ŕ��f),114:�ŏ���100�t���[���قǂ͐l����F���ł��ĂȂ�
			printf("ude ok :frame %d\n",i);
			plusedflame=beamMode(i);//�i�񂾃t���[����
			outputAndBall(i);/////////***************+++++++++++++++++++now
			//i+=plusedflame;��
			i=plusedflame;//beamMode()�̏��������ɂ��ύX
			check=i;
		}
		
		ude_y=0;
		
		bodyHeight(user_img);//width�̕��͂��łɂ��Ƃ܂��Ă���

		if(i>=320&&i<=450){
			okuyuki(i);
			printf("depth:%lf\n",ave[i-320]);
			if(i>350&&ave[i-320]<100){//�ύX�O(ave[i-320]-ave[i-320-15]>5)
				printf("zensin:%d\n",i);
				small(i);
			}
		}
		
		
		output_img=(img *)malloc(sizeof(img));
		ReadImg(image_filename,output_img);//////////////////////////////////////////��

		//WritePng("output.png", tmp2);
		sprintf(output_filename,"output/%05d.png",i);
		/////////////////////////////////////////////////////////////////////////WritePng(output_filename,output_img);//�o��
		
		
		//pre_body_width=body_width;//���̃��[�v�֍s���̂ŁApre���X�V
		free(user_img);
		free(image_img);
		free(output_img);
		
		
		if(i%50==0)//�i�s���\��
			printf("now...%d\n",i);
	}
	
	//printf("%c",image_filename[3]);//=>"g"
	return 0;
}



//�̂̕����v�Z//////////////////////////�e�X�g�ς�
int bodyWidth(img *user_img){

	int i,j;
	int left=0,right=0,width=0,widthmax=0;
	leftmin=640,rightmax=0;
	int body=0;//0:�̂̊O�A1:�̂̒�
	ude_y=0;
	
	for(i=0;i<user_img->height;i++){
		//prewidth=width;
		for(j=0;j<user_img->width;j++){
			if(body==0&&(user_img->data[i][j].r!=0||user_img->data[i][j].g!=0||user_img->data[i][j].b!=0)){//1�s�ɂ�1�x�̂ݍX�V
				left=j;//�̂̍��[
				body=1;
				//ude_y=i;
				if(left<leftmin){
					leftmin=left;
					ude_y=i;
				}
			}
			if(user_img->data[i][j].r!=0&&user_img->data[i][j].g!=0&&user_img->data[i][j].b!=0){//body�Ɉˑ����Ȃ����ƂŐ����X�V
				right=j;
				if(right>rightmax)
					rightmax=right;
			}
		}
		body=0;
		
		if(widthmax<rightmax-leftmin)
			widthmax=rightmax-leftmin;
	}


	
	//printf("\n%d,%d,%d   -20:",right,left,widthmax);//�`�F�b�N�p�@main�Ɂh:�h�ȍ~������
	return widthmax;
}



//�̂̍���::�e�X�g�ς�
int bodyHeight(img *user_img){
	int i,j;
	//int left=0,right=0,width=0,leftmin=640,rightmax=0,widthmax=0;
	int ue=-1,sita=0,heightmax=0;
	uemin=480,sitamax=0;
	int body=0;//�c�����Ɍ���0:�̂̊O�A1:�̂̒�
	int bodyx=0;//�������Ɍ���1�x�ł��̂̈ꕔ���F��������1
	
	
	for(i=0;i<user_img->height;i++){
		bodyx=0;
		for(j=0;j<user_img->width;j++){
			if(user_img->data[i][j].r!=0||user_img->data[i][j].g!=0||user_img->data[i][j].b!=0){
				if(uemin>i)
					uemin=i;
				body=1;
				bodyx=1;
				break;//�����Ɏ��̍s��
			}
		}
		//1�s���I�������
		if(bodyx==0&&body==1){//���̉��[��艺�Ȃ��
			sitamax=i;
			//printf("%d %d ",uemin,sitamax);
			return sitamax-uemin;
		}
	}
	
	heightmax=sitamax-uemin;
	//printf("\n%d,%d,%d   -20:",right,left,widthmax);//�`�F�b�N�p�@main�Ɂh:�h�ȍ~������

	return heightmax;
}


/*�Ԃ�l�͂��̊Ԃɏ��������t���[����*/
int beamMode(int i){
	int firstframe=i;
	img *user_imgB;
	char user_filename[256];

	
	int udey[80];
	
	printf("beam mode!!!!!!!!!frame:%d\n",i);
	while(1){//���[�v����邲�ƂɎ��̃t���[����
		user_imgB=(img *)malloc(sizeof(img));
		sprintf(user_filename,"user/%05d.jpg",i);
		ReadImg(user_filename,user_imgB);//�l���̈�̉摜��ǂݍ���
		
		bodyWidth(user_imgB);//ude_y�X�V���Ă���
		udey[i-firstframe]=ude_y;//�r�̂��鍂�����ۑ������
		printf("%d\n",ude_y);
		outputAndBall(i);
		if(i-firstframe>17){
			if(abs(udey[i-firstframe]-udey[i-firstframe-15])>70){//�r�̍�����50�ȏ�ω�������r�[���o��::abs()@stdlib.h
				printf("!!!beam on!!\n");
				///�������܂Ńe�X�g�ς�///
				
				createBeam(i);//outputAndBall(i);
				i+=22;//createBeam�ł�22�t���[���g����̂�
				
				free(user_imgB);
				break;
			}
		}
		
		//printf("%d\n",frame);
		i++;
		if(i-firstframe>=75){//5�b�ȏ�ω��Ȃ�
			free(user_imgB);
			break;
		}
		free(user_imgB);
	}
	ude_y=0;
	printf("off%d\n",i);
	return i;
}

int beamx=-1;//�r�[���̍��[��x���W
int beamoff=1;
int xxx=0;
void createBeam(int i){//�r�[���͉�ʒ��S����o����A�����͌������č������Œ�//i:���݂̃t���[����

	int j;
	img *beamed;
	char img_filename[256];
	char output_filename[256];
	
	//7�t���[��������������΂�(���Ԓ���)
	for(j=0;j<=7;j++,i++){
		beamed=(img *)malloc(sizeof(img));
		sprintf(img_filename,"image/%05d.jpg",i);
		ReadImg(img_filename,beamed);
		sprintf(output_filename,"output/%05d.png",i);
		WritePng(output_filename, beamed);
		free(beamed);
		outputAndBall(i);
	}
	
	if(beamoff==1){
		for(j=0;j<=15;j++,i++){
			beamed=(img *)malloc(sizeof(img));
			sprintf(img_filename,"image/%05d.jpg",i);
			ReadImg(img_filename,beamed);
			int h,a;//a:�r�[���̉���
			for(h=100;h<400;h++){
				//��������20px/frame�̑����Ńr�[���͐i��
				beamx=320-20*j;
				for(a=0;a<=10;a++){
					beamed->data[h][beamx+a].r=255;
					beamed->data[h][beamx+a].g=0;
					beamed->data[h][beamx+a].b=0;
				}
				
			}
			sprintf(output_filename,"output/%05d.png",i);
			WritePng(output_filename, beamed);
			xxx=1;
			outputAndBall(i);
			xxx=0;
			free(beamed);
		}
	}
}

/*///////////�摜���o�͂��邽�߂����̊֐���p�ӂ��A�{�[����������ŕ`�悷�遫
void ballRoll(){//�{�[����������]�����Ă���/��Fy=390  ***�Ⴆ�΃r�[��()�̒��ł��摜���o�͂���Ƃ��Ƀ{�[����`�悷��K�v������
	int radius;
	img *beamed;
	char img_filename[256];
	char output_filename[256];
	
	
}*/


void outputAndBall(int i){
	if(i%10==0)
	printf("%d\n",i);
	img *image;
	img *user;
	char img_filename[256];
	char output_filename[256];
	char user_filename[256];
	image=(img *)malloc(sizeof(img));
	user=(img *)malloc(sizeof(img));
	if(xxx==0)
		sprintf(img_filename,"image/%05d.jpg",i);/////////////////////////////createbeam��outputandball�Ƃŏ㏑���h�~�̈�
	else
		sprintf(img_filename,"output/%05d.png",i);
	sprintf(user_filename,"user/%05d.jpg",i);
	ReadImg(img_filename,image);
	ReadImg(user_filename,user);

	
	int l2;//�{�[���̒��S����̋����̓��A12�łȂ��A����[
	
	//�{�[�����o��(2�ȏ�͖���)
	int radius=50;//�����a
	int h,w;
	if(balloff==1){//�{�[�����o���^�C�~���O�͎w��(����B�e�ς݂̂���)
		int a=radius;
		for(h=390-radius;h<390;h++){
			//if(ballx-25<0)
			//	ballx=0;
			//if(ballx-a<0)	a=0;
			for(w=ballx-a;w<ballx+50;w++){
				l2=(ballx-w)*(ballx-w)+(365-h)*(365-h);//365:�{�[���̒��S��y���W
				//printf("check now\n");
				if(l2<=525){//525:�{�[���̒��a��2��
					//printf("create  ball at (%d,355)\n",ballx);
					if(w>=0){
						image->data[h][w].r=0;
						image->data[h][w].g=0;
						image->data[h][w].b=255;
						
						if(user->data[h][w].r!=0&&v!=0){//�{�[���Ɛl�����Ԃ�����
							printf("ball-person touch:%d\n",i);
							balloff=0;
						}
						if(i==262||i==488||i>=625){
							printf("ball-beam touch:%d\n",i);
							beamx=-1;
							balloff=0;
							//beamoff=0;
						}
					}
				}
			}
		}
	}

	ballx+=v;//�{�[���̈ړ����x
	sprintf(output_filename,"output/%05d.png",i);
	WritePng(output_filename, image);
	free(image);
	free(user);
}


//double ave[131];//320~450*******************************************************���s���̊J�n�A�I���ԍ�����@main    �ŏ��Ɉړ�
//�r��������ɂ��B->uemin,sitamax,leftmin,rightmax���ŐV
void okuyuki(int i){
	img *depth;
	char depth_filename[256];
	depth=(img *)malloc(sizeof(img));
	outputAndBall(i);////////////////////////main����ړ�
	sprintf(depth_filename,"depth/%05d.jpg",i);
	ReadImg(depth_filename,depth);
	
	int h,w;
	
	int sum=0;

	for(h=uemin;h<=sitamax;h++){
		for(w=leftmin;w<=rightmax;w++){
			//depth�摜�͌덷���傫���̂ŁA�l���̈���ŉ��s�����̕��ϒl���o��<-�D�F�̏ꍇ�Argb�͑S�ē����l�Ȃ̂ŁAr�݂̂ɑ΂��ĕ��ς��o���B
			sum+=depth->data[h][w].r;
		}
	}
	int s=(sitamax-uemin)*(rightmax-leftmin);//�l���̈��(�l�p)�ʐ�
	printf("sum,s=%d,%d",sum,s);
	ave[i-320]=(double)sum/s;
	free(depth);
}

void small(int i){
	img *image;
	img *output;
	char image_filename[256];
	char output_filename[256];
	image=(img *)malloc(sizeof(img));
	output=(img *)malloc(sizeof(img));
	sprintf(image_filename,"output/%05d.png",i);//�㏑��suru
	ReadImg(image_filename,image);
	ReadImg(image_filename,output);
	
	int h,w;
	for(h=uemin;h<=480-uemin/2;h++){
		for(w=leftmin;w<=rightmax;w++){
			output->data[h][w].r=60;
			output->data[h][w].g=60;
			output->data[h][w].b=60;
		}
	}
	//depth�𑪂�������ɌĂяo�����̂ŁAleftmin,etc�͂��̂܂ܗ��p
	for(h=uemin;h<=sitamax;h++){
		for(w=leftmin;w<=rightmax;w++){
			
			if(h%2==0)
				output->data[480-h/2][w]=image->data[480-h][w];//�c�����ɔ�����
		}
	}
	if(i==375){
		balloff=0;
	}
	//check
	//WritePng("smallcheck.png",output);//===>ok
	sprintf(output_filename,"output/%05d.png",i);
	WritePng(output_filename, output);
	//outputAndBall(i);
	free(image);
	free(output);
	
}

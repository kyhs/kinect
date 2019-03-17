#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"


int ude_y=0;//腕のある位置のy座標
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
int beamMode(int i);//i:フレーム番号
void createBeam(int i);
//void ballRoll();出力で
void outputAndBall(int i);
void okuyuki(int i);//奥行き情報は大域変数でやっているのでvoidにした///なお、奥行き情報はdoubleの配列ave[]に入っている
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
	
	for(i=455;i<=765;i++){//ここからメイン151~~~~~~~~~~
		if(i%50==0)//進行情報表示
			printf("now...%d\n",i);

		user_img=(img *)malloc(sizeof(img));
		sprintf(user_filename,"user/%05d.jpg",i);
		ReadImg(user_filename,user_img);
		body_width[i]=bodyWidth(user_img);
		bodyHeight(user_img);//こちらは単に実行するだけ(大域変数を変化させる)で、返り値は利用しない
		if(i>155&&i<226){//最初のジャンプ
			outputAndBall(i);
		}
		
		//ビームのところは、ボールを描画した後、上からビームを上書きする::outputAndBall->createBeam<====逆///****
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
		}//ここまでok（要時間調整, ビームとボールの当たり判定）

		if(i==285)
			v=25;
		if(i>=285&&i<320)//ジャンプ
			outputAndBall(i);
		if(i==300)
			balloff=0;

		if(i>=320&&i<=453){//前進、ジャンプ、後進
			v=5;
			okuyuki(i);
			printf("depth:%lf\n",ave[i-320]);
			//outputAndBall(i);
			if(i>350&&ave[i-320]<100){//変更前(ave[i-320]-ave[i-320-15]>5)
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

		if(i==650){//ending650枚目のみを使用これ以降
			int j=i;//j:出力用フレーム番号
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

int test(void){//元々メインだった、今はどこからも呼ばれていない
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
		/*それぞれのファイル名を読み込んでいく(forループが回るたびに上書き)*/
		sprintf(image_filename, "image/%05d.jpg", i);
		sprintf(depth_filename,"depth/%05d.jpg",i);
		sprintf(user_filename,"user/%05d.jpg",i);
		
		ReadImg(user_filename,user_img);//人物領域の画像を読み込む
		////////////////////////////////////////////////////ReadImg(image_filename,image_img);
		body_width[i]=bodyWidth(user_img);//体の幅を求めた
					//if(i>20) printf("%d\n",body_width[i-20]);
		
		//////////////////////heightのcheck
		//////////////////////
		//printf("%d\n",bodyHeight(user_img));
		//////////////////////
		

		//	outputAndBall(i);
		if(i>155&&i<220)
			outputAndBall(i);
		////テスト時間短縮のため、一旦コメントアウト
		if(i==155)
			printf("NOT skip ball\n");

		
		if(i>=114&&(body_width[i]-body_width[i-20]>40)&&(i-check>20)){//手が突き出された時(20フレームで判断),114:最初の100フレームほどは人物を認識できてない
			printf("ude ok :frame %d\n",i);
			plusedflame=beamMode(i);//進んだフレーム数
			outputAndBall(i);/////////***************+++++++++++++++++++now
			//i+=plusedflame;↓
			i=plusedflame;//beamMode()の書き換えによる変更
			check=i;
		}
		
		ude_y=0;
		
		bodyHeight(user_img);//widthの方はすでにもとまっている

		if(i>=320&&i<=450){
			okuyuki(i);
			printf("depth:%lf\n",ave[i-320]);
			if(i>350&&ave[i-320]<100){//変更前(ave[i-320]-ave[i-320-15]>5)
				printf("zensin:%d\n",i);
				small(i);
			}
		}
		
		
		output_img=(img *)malloc(sizeof(img));
		ReadImg(image_filename,output_img);//////////////////////////////////////////仮

		//WritePng("output.png", tmp2);
		sprintf(output_filename,"output/%05d.png",i);
		/////////////////////////////////////////////////////////////////////////WritePng(output_filename,output_img);//出力
		
		
		//pre_body_width=body_width;//次のループへ行くので、preを更新
		free(user_img);
		free(image_img);
		free(output_img);
		
		
		if(i%50==0)//進行情報表示
			printf("now...%d\n",i);
	}
	
	//printf("%c",image_filename[3]);//=>"g"
	return 0;
}



//体の幅を計算//////////////////////////テスト済み
int bodyWidth(img *user_img){

	int i,j;
	int left=0,right=0,width=0,widthmax=0;
	leftmin=640,rightmax=0;
	int body=0;//0:体の外、1:体の中
	ude_y=0;
	
	for(i=0;i<user_img->height;i++){
		//prewidth=width;
		for(j=0;j<user_img->width;j++){
			if(body==0&&(user_img->data[i][j].r!=0||user_img->data[i][j].g!=0||user_img->data[i][j].b!=0)){//1行につき1度のみ更新
				left=j;//体の左端
				body=1;
				//ude_y=i;
				if(left<leftmin){
					leftmin=left;
					ude_y=i;
				}
			}
			if(user_img->data[i][j].r!=0&&user_img->data[i][j].g!=0&&user_img->data[i][j].b!=0){//bodyに依存しないことで随時更新
				right=j;
				if(right>rightmax)
					rightmax=right;
			}
		}
		body=0;
		
		if(widthmax<rightmax-leftmin)
			widthmax=rightmax-leftmin;
	}


	
	//printf("\n%d,%d,%d   -20:",right,left,widthmax);//チェック用　mainに”:”以降がある
	return widthmax;
}



//体の高さ::テスト済み
int bodyHeight(img *user_img){
	int i,j;
	//int left=0,right=0,width=0,leftmin=640,rightmax=0,widthmax=0;
	int ue=-1,sita=0,heightmax=0;
	uemin=480,sitamax=0;
	int body=0;//縦方向に見て0:体の外、1:体の中
	int bodyx=0;//横方向に見て1度でも体の一部が認識されれば1
	
	
	for(i=0;i<user_img->height;i++){
		bodyx=0;
		for(j=0;j<user_img->width;j++){
			if(user_img->data[i][j].r!=0||user_img->data[i][j].g!=0||user_img->data[i][j].b!=0){
				if(uemin>i)
					uemin=i;
				body=1;
				bodyx=1;
				break;//即座に次の行へ
			}
		}
		//1行見終わった後
		if(bodyx==0&&body==1){//足の下端より下ならば
			sitamax=i;
			//printf("%d %d ",uemin,sitamax);
			return sitamax-uemin;
		}
	}
	
	heightmax=sitamax-uemin;
	//printf("\n%d,%d,%d   -20:",right,left,widthmax);//チェック用　mainに”:”以降がある

	return heightmax;
}


/*返り値はこの間に消化したフレーム数*/
int beamMode(int i){
	int firstframe=i;
	img *user_imgB;
	char user_filename[256];

	
	int udey[80];
	
	printf("beam mode!!!!!!!!!frame:%d\n",i);
	while(1){//ループが回るごとに次のフレームへ
		user_imgB=(img *)malloc(sizeof(img));
		sprintf(user_filename,"user/%05d.jpg",i);
		ReadImg(user_filename,user_imgB);//人物領域の画像を読み込む
		
		bodyWidth(user_imgB);//ude_y更新していく
		udey[i-firstframe]=ude_y;//腕のある高さが保存される
		printf("%d\n",ude_y);
		outputAndBall(i);
		if(i-firstframe>17){
			if(abs(udey[i-firstframe]-udey[i-firstframe-15])>70){//腕の高さが50以上変化したらビーム出す::abs()@stdlib.h
				printf("!!!beam on!!\n");
				///↑ここまでテスト済み///
				
				createBeam(i);//outputAndBall(i);
				i+=22;//createBeamでは22フレーム使われるので
				
				free(user_imgB);
				break;
			}
		}
		
		//printf("%d\n",frame);
		i++;
		if(i-firstframe>=75){//5秒以上変化なし
			free(user_imgB);
			break;
		}
		free(user_imgB);
	}
	ude_y=0;
	printf("off%d\n",i);
	return i;
}

int beamx=-1;//ビームの左端のx座標
int beamoff=1;
int xxx=0;
void createBeam(int i){//ビームは画面中心から出され、向きは向かって左方向固定//i:現在のフレーム数

	int j;
	img *beamed;
	char img_filename[256];
	char output_filename[256];
	
	//7フレーム処理をせず飛ばす(時間調整)
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
			int h,a;//a:ビームの横幅
			for(h=100;h<400;h++){
				//横方向に20px/frameの速さでビームは進む
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

/*///////////画像を出力するためだけの関数を用意し、ボールもそちらで描画する↓
void ballRoll(){//ボールを左から転がしてくる/底：y=390  ***例えばビーム()の中でも画像を出力するときにボールを描画する必要性あり
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
		sprintf(img_filename,"image/%05d.jpg",i);/////////////////////////////createbeamとoutputandballとで上書き防止の為
	else
		sprintf(img_filename,"output/%05d.png",i);
	sprintf(user_filename,"user/%05d.jpg",i);
	ReadImg(img_filename,image);
	ReadImg(user_filename,user);

	
	int l2;//ボールの中心からの距離の二乗、12でなく、えるつー
	
	//ボールを出す(2個以上は無い)
	int radius=50;//※直径
	int h,w;
	if(balloff==1){//ボールを出すタイミングは指定(動画撮影済みのため)
		int a=radius;
		for(h=390-radius;h<390;h++){
			//if(ballx-25<0)
			//	ballx=0;
			//if(ballx-a<0)	a=0;
			for(w=ballx-a;w<ballx+50;w++){
				l2=(ballx-w)*(ballx-w)+(365-h)*(365-h);//365:ボールの中心のy座標
				//printf("check now\n");
				if(l2<=525){//525:ボールの直径の2乗
					//printf("create  ball at (%d,355)\n",ballx);
					if(w>=0){
						image->data[h][w].r=0;
						image->data[h][w].g=0;
						image->data[h][w].b=255;
						
						if(user->data[h][w].r!=0&&v!=0){//ボールと人がかぶったら
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

	ballx+=v;//ボールの移動速度
	sprintf(output_filename,"output/%05d.png",i);
	WritePng(output_filename, image);
	free(image);
	free(user);
}


//double ave[131];//320~450*******************************************************奥行きの開始、終了番号注意@main    最初に移動
//腕を見た後にやる。->uemin,sitamax,leftmin,rightmaxが最新
void okuyuki(int i){
	img *depth;
	char depth_filename[256];
	depth=(img *)malloc(sizeof(img));
	outputAndBall(i);////////////////////////mainから移動
	sprintf(depth_filename,"depth/%05d.jpg",i);
	ReadImg(depth_filename,depth);
	
	int h,w;
	
	int sum=0;

	for(h=uemin;h<=sitamax;h++){
		for(w=leftmin;w<=rightmax;w++){
			//depth画像は誤差が大きいので、人物領域内で奥行き情報の平均値を出す<-灰色の場合、rgbは全て同じ値なので、rのみに対して平均を出す。
			sum+=depth->data[h][w].r;
		}
	}
	int s=(sitamax-uemin)*(rightmax-leftmin);//人物領域の(四角)面積
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
	sprintf(image_filename,"output/%05d.png",i);//上書きsuru
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
	//depthを測った直後に呼び出されるので、leftmin,etcはそのまま利用
	for(h=uemin;h<=sitamax;h++){
		for(w=leftmin;w<=rightmax;w++){
			
			if(h%2==0)
				output->data[480-h/2][w]=image->data[480-h][w];//縦方向に半分に
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

#include<bits/stdc++.h>

using namespace std;

char data[200];
char name[10],maps[4][20];
int fuelamount,lvl,speed,updatecount,score,c=0,r=2,times;


void encode(){
	//char data[200];
	int p = 0;
	for(int i = 0; i < 4 ;i++){
		for(int j = 0; j < 20; j++){
			data[p] = maps[i][j];
			p++;
		}
	data[p++] = '%';
	}
	// map

	int i = 0;
	while(i < strlen(name)){
		data[p] = name[i];
		i++;
		p++;
	}
	data[p++] = '%';
	// name
    char buf[20];

    sprintf(buf,"%d",fuelamount);
    strcat(data,buf);
    p+=strlen(buf);
	data[p++] = '%';
	// fuelamount

	sprintf(buf,"%d",lvl);
	strcat(data,buf);
	p+=strlen(buf);
	data[p++] = '%';
	// level

	sprintf(buf,"%d",speed);
	strcat(data,buf);
	p+=strlen(buf);
	data[p++] = '%';
	// speed


	sprintf(buf,"%d",60-updatecount);
	strcat(data,buf);
	p+=strlen(buf);
	data[p++] = '%';
	// remaining length of level

	sprintf(buf,"%d",c);
	strcat(data,buf);
	p+=strlen(buf);
	data[p++] = '%';
	sprintf(buf,"%d",r);
	strcat(data,buf);
	p+=strlen(buf);
	data[p++] = '%';
	// plane position

    sprintf(buf,"%d",times);
	strcat(data,buf);
	p+=strlen(buf);
	data[p++] = '%';
	// time

	sprintf(buf,"%d",score);
	strcat(data,buf);
	p+=strlen(buf);
	data[p++] = '%';
	// score

}

void decode(){
    int p=0,q=0;
    char buf[20];
    for(int i=0;i<4;i++){
        for(int j=0;j<20;j++){
            maps[i][j]=data[p++];
        }
        p++;
    }

    while(data[p]!='%'){
        buf[q]=data[p];
        p++;
        q++;
    }
    p++;
    q=0;
    strcpy(name,buf);

    while(data[p]!='%'){
        buf[q]=data[p];
        p++;
        q++;
    }
    p++;
    q=0;
    fuelamount=atoi(buf);

    while(data[p]!='%'){
        buf[q]=data[p];
        p++;
        q++;
    }
    p++;
    q=0;
    lvl=atoi(buf);

    while(data[p]!='%'){
        buf[q]=data[p];
        p++;
        q++;
    }
    p++;
    q=0;
    speed=atoi(buf);

    while(data[p]!='%'){
        buf[q]=data[p];
        p++;
        q++;
    }
    p++;
    q=0;
    updatecount = 60 - atoi(buf);

    while(data[p]!='%'){
        buf[q]=data[p];
        p++;
        q++;
    }
    p++;
    q=0;
    c=atoi(buf);

    while(data[p]!='%'){
        buf[q]=data[p];
        p++;
        q++;
    }
    p++;
    q=0;
    r=atoi(buf);

    while(data[p]!='%'){
        buf[q]=data[p];
        p++;
        q++;
    }
    p++;
    q=0;
    times=atoi(buf);

    while(data[p]!='%'){
        buf[q]=data[p];
        p++;
        q++;
    }
    p++;
    q=0;
    score=atoi(buf);

}

int main(){
    memset(data,0,sizeof data);
    name[0]='S';
    name[1]='D';
    name[2]='R';
    for(int i=0;i<4;i++){
        for(int j=0;j<20;j++){
            maps[i][j]='j';
            //cout<<maps[i][j];
        }
    }
    speed=69;
    score=1234;
    updatecount=33;
    lvl=3;
    fuelamount=5;
    times=100;
    encode();
    for(int i=0;i<strlen(data);i++){
        cout<<data[i];
    }


    cout<<endl;
    speed=score=updatecount=lvl=fuelamount=times=0;
    memset(maps,0,sizeof maps);
    memset(name,0,sizeof name);
    decode();
    cout<<speed<<endl;
    cout<<times<<endl;
    cout<<lvl<<endl;
    cout<<c<<endl;
    cout<<r<<endl;
    cout<<score<<endl;
    for(int i=0;i<strlen(name);i++){
        cout<<name[i];
    }

    return 0;
}

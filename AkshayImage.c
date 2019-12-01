#include <stdio.h>
#include <string.h>
#define kernel_size 3

int pixel[360][250]; //width and height are manually taken by taking a glance at the pgm file.
int padded[450][450];
int fmat[448][448];
int pool[446][446];

void convert_to_binary()
{
	int i,j;
	for(i=0;i<360;i++)
	{
		for(j=0;j<250;j++)
		{
			padded[i][j]=padded[i][j]>128 ? 255:0;
		}
	}
	FILE* f;
	f=fopen("D:\\binarymona.pgm","wb");
	fprintf(f,"P2\n");
	fprintf(f,"%d %d\n",450,450);
	fprintf(f,"255\n");
	//int resm[kernel_size][kernel_size];
	for(i=0;i<446;i++)
	{
		for(j=0;j<446;j++)
		{
			fprintf(f,"%d ",padded[i][j]);
		}
	//	printf("\n");
		fprintf(f,"\n");
	}
	fclose(f);
}

int maxres(int x,int y)
{
	int i,j,max=-1;
	for(i=x;i<x+kernel_size;i++)
	{
		for(j=y;j<y+kernel_size;j++)
		{
			if(max<fmat[i][j]){
				max=fmat[i][j];
			}
		}
	}
	return max;
}

void maxpooling()
{
	int r=0,c=0;
	int i,j;
	for(i=0;i<448;i++)
	{
		for(j=0;j<448;j++)
		{
			printf("%d ",fmat[i][j]);
		}
		printf("\n");
	}
	for(i=0;i<448-kernel_size && r<446;i++)
	{
		c=0;
		for(j=0;j<448-kernel_size && c<446;j++)
		{
			pool[r][c++]=maxres(i,j);
		}
		r++;
	}
	FILE* f;
	f=fopen("D:\\pooledone.pgm","wb");
	fprintf(f,"P2\n");
	fprintf(f,"%d %d\n",446,446);
	fprintf(f,"255\n");
	//int resm[kernel_size][kernel_size];
	for(i=0;i<446;i++)
	{
		for(j=0;j<446;j++)
		{
			fprintf(f,"%d ",pool[i][j]);
		}
	//	printf("\n");
		fprintf(f,"\n");
	}
	fclose(f);
}

int multiply(int kernel[kernel_size][kernel_size],int x,int y)
{
	int res=0;
	int i,j,k;
	for(i=0;i<kernel_size;i++)
	{
		for(j=0;j<kernel_size;j++)
		{
			if(kernel[i][j])
			{
				if(x+i<450 && y+j<450)
				res+=padded[x+i][y+j];
			}
		}
	}
	
	/*FILE* f;
	char ext[]=".pgm";
	char filename[100]="D:\\write";
	strncat(filename,&c,1);
	int last=0;
	while(filename[last++]!='\0');
	last--;
	for(j=0;i<ext[j]!='\0';last++,j++)
	{
		filename[last]=ext[j];
	}
	filename[last]='\0';
	//puts(filename);
	f=fopen(filename,"wb");
	fprintf(f,"P2\n");
	fprintf(f,"%d %d\n",kernel_size,kernel_size);
	fprintf(f,"255\n");
	//int resm[kernel_size][kernel_size];
	for(i=0;i<kernel_size;i++)
	{
		for(j=0;j<kernel_size;j++)
		{
		//	printf("%d ",kernel[i][j]);
			if(kernel[i][j] && x+i<450 && y+j<450)
			{
				printf("%d ",padded[x+i][y+j]);
				fprintf(f,"%d ",padded[x+i][y+j]);
			}
			else
			{
				printf("255 ");
				fprintf(f,"%d ",255);
			}
		}
		printf("\n");
		fprintf(f,"\n");
	}
	fclose(f);*/
	return res;
}

void convoluteandwrite()
{
	int kernel[kernel_size][kernel_size]={{1,0,1},{0,1,0},{1,0,1}};
	int stride=1;
	int x=0,y=0,i,j,r=0,c=0;
//	int fmat[448][448];
	//memset(padded,255,450*450*sizeof(int));
	for(i=0;i<450;i++)
	{
		for(j=0;j<450;j++)
		{
			padded[i][j]=255;
		}
	}
	for(i=45;i<405;i++)
	{
		for(j=100;j<350;j++)
		{
			padded[i][j]=pixel[(i+360)-405][(j+250)-350];
		}
	}
	//char c='a';
	
	for(i=0;i<450-kernel_size && r<448;i+=stride)
	{
		c=0;
		for(j=0;j<450-kernel_size && c<448;j+=stride)
		{
			int result=multiply(kernel,i,j);
			fmat[r][c++]=result;
		//	printf("%d ",result);
		  	printf("Executing...\n");
		}
	//printf("\n");
		r++;
	}
}

main()
{
	FILE* fp;
	fp=fopen("D:\\mona.pgm","rb");
	char d[1000];
	int cntl=1,row=0,i,j,col=0;
	char* token;
	while(fgets(d,1000,fp)!=NULL && row<360)
	{
		
		if(cntl>4)
		{
			//printf("%d\n",strlen(d));
			
			
//			d[last+1]='\0';
		    //if(cntl==5)
		    //   printf("Character:%sgap\n",d);
		//	fputs(d,stdout);
		    //printf("Length: %d\n",strlen(d));
			col=0;
			for(i=1;d[i]!='\0' && col<250;)
			{
				
				while(d[i]!=' ' && d[i]!='\n')
				{
					
					//printf("%d\n",i);
					//printf("%c",d[i]);
					pixel[row][col]=pixel[row][col]*10+(d[i]-48);
					i++;
				}
			
				i+=1;
				col+=1;
			}
			row++;
		}
		cntl++;
	}
	
	for(i=0;i<360;i++)
	{
	//	printf("i:%d\n",i);
		for(j=0;j<250;j++)
		{
		//	printf("%d ",pixel[i][j]);
		}
		//printf("\n");
	}
	convoluteandwrite();
	maxpooling();
	convert_to_binary();
	fclose(fp);
	fp=NULL;
}

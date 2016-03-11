/*rafft.c     pwe: 02/06/13 
Takes rtlsdr bin files applies FFT in blocks and averages over the input data length
       Command format:- rafft <infile> <outfile> <No. FFT points>*/

#include <stdio.h>
#include <stdlib.h>
#include <graph.h>
#include <math.h>

#define SWAP(a,b) tempr=(a); (a)=(b); (b)=tempr
#define PI (6.28318530717959/2.0)
#define PTS 256


int flag=0,da[2048];
long int file_end,count,p_num;
float dats[4096],datr[4096],sig_pow,nois_pow;
unsigned char ucha;

FILE *fpto;

void sum_dat(void);
void out_dat(void);
void four(float[],int,int);


main(argc,argv)

int argc;
char *argv[];

{

long int t,s,ss;

FILE *fptr;
/*check command line arguments*/
if(argc !=4)
  { printf("Format: rafft <Infile>  <Outfile> <No. FFT Points>");exit(0);}

_clearscreen(_GCLEARSCREEN);

if((fptr=fopen(argv[1],"rb"))==NULL)
   {printf("Can't open file %s. ",argv[1]);exit(0);}

pts=atol(argv[3]);
fpto = fopen(argv[2],"w");

/*find length of input file*/
fseek(fptr,0L,SEEK_END);
file_end=(ftell(fptr)+1);

p_num=(long int)(file_end/pts/2);

printf("Number of Bytes = %ld   No. of FFTs=%ld\n",file_end-1,p_num);
fclose(fptr);
fptr=fopen(argv[1],"rb");


/*read input file,decode I and q, put in data file apply fft, apply running average. At end of input file, output text file with averaged data*/

 {
 for(ss=0;ss<p_num;ss++)
    {

 for(s=0;s<2*pts;s++)
	{
	ucha=getc(fptr);	 
				 
	dats[s]=(float)ucha+0.0; 
	if(dats[s]>127)
	    dats[s]=(dats[s]-128)/128.0;
	else dats[s]=(dats[s]-128)/128.0;

	count++;
	}
	
/*take fourier transform*/
	four(dats-1,pts,-1);

	sum_dat();
    }
}
out_dat();
fclose(fptr);
fclose(fpto);
printf("\nInfile=%s    Outfile=%s   FFT Points=%d\n",argv[1],argv[2],atol(argv[3]));
exit(0);
}



void out_dat(void)
{
int tt;
float opp;

for(tt=0;tt<pts;tt++){
if(tt<(pts/2))opp=(float)datr[tt+pts/2];
	   else  opp=(float)datr[tt-pts/2];

fprintf(fpto,"%d    %3.3f\n",(tt-pts/2),(float)opp/(float)p_num);
}
fclose(fpto);
}


void sum_dat(void)
{
int tt;
float opp;

for(tt=0;tt<pts;tt++){
datr[tt]=datr[tt]+(float) (dats[2*tt]*dats[2*tt]+dats[2*tt+1]*dats[2*tt+1]);

}
}


/*fast fourier transform routine*/
void four(data,nn,isign)
 float data[];
 int nn,isign;
 {
  int n,mmax,m,j,istep,i,a;
  double wtemp,wr,wpr,wpi,wi,theta;
  float tempr,tempi;
  n=nn<<1;
  j=1;
  for(i=1;i<n;i+=2)
     {
     if(j>i){
	     SWAP(data[j],data[i]);
	     SWAP(data[j+1],data[i+1]);
	     }
     m=n>>1;
     while(m>=2 && j>m){
			j-=m;
			m>>=1;
			}
     j+=m;
     }
  mmax=2;

  while(n>mmax)
    {
     istep=2*mmax;
     theta=6.28318530717959/(isign*mmax);
     wtemp=sin(0.5*theta);
     wpr=-2.0*wtemp*wtemp;
     wpi=sin(theta);
     wr=1.0;
     wi=0.0;
     for(m=1;m<mmax;m+=2){
       for(i=m;i<=n;i+=istep){
	  j=i+mmax;
	  tempr=wr*data[j]-wi*data[j+1];
	  tempi=wr*data[j+1]+wi*data[j];
	  data[j]=data[i]-tempr;
	  data[j+1]=data[i+1]-tempi;
	  data[i]+=tempr;

	  data[i+1]+=tempi;
 if(j<0)j=0;

			      }
       wr=(wtemp=wr)*wpr-wi*wpi+wr;
       wi=wi*wpr+wtemp*wpi+wi;
			  }
     mmax=istep;
/* printf("%d    %d    %d    %d    %d\n",i,n,m,PTS,istep);*/
     }
  if(isign==1){for(a=0;a<2*pts;a++){
  data[a]=data[a]/pts;}}
 }

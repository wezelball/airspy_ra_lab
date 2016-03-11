/*RATAnal.c     pweast: 25/08/13
Takes rtlsdr bin files applies FFT in blocks and averages over the input
data length, divides signal <infileS> FFT blocks by reference <infileR>
FFT blocks plots a basic ratio graph and outputs a text file of the final averaged result
Command format:- RATAnal <infileR> <infileS <outfile> <fft points> */

#include <stdio.h>
#include <stdlib.h>
#include <graph.h>
#include <math.h>

#define SWAP(a,b) tempr=(a); (a)=(b); (b)=tempr
#define PI (6.28318530717959/2.0)
#define PTS 256


long int file_end,file2_end,count,p_num,coun=0;
float dats[4096],datr[4096],dats2[4096],datr2[4096],dato1[4096],dato2[4096];
unsigned char ucha,ucha2;


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
FILE *fptr2;
/*check command line arguments*/
if(argc !=5)
  { printf("Format: RATAnal <InfileRef>  <InfileSignal> <Outfile> <No. FFT Points>");exit(0);}

_clearscreen(_GCLEARSCREEN);


if((fptr=fopen(argv[1],"rb"))==NULL)
   {printf("Can't open file %s. ",argv[1]);exit(0);}
if((fptr2=fopen(argv[2],"rb"))==NULL)
   {printf("Can't open file %s. ",argv[2]);exit(0);}

pts=atol(argv[4]);

fpto = fopen(argv[3],"w");

/*find lengths of input files*/
fseek(fptr,0L,SEEK_END);
file_end=(ftell(fptr)+1);
fseek(fptr2,0L,SEEK_END);
file2_end=(ftell(fptr2)+1);

if(file_end != file2_end)
   {printf("Infiles unequal length");exit(0);}

p_num=(long int)(file_end/pts/2);

printf("Number of Bytes = %ld   No. of FFTs=%ld\n",file_end-1,p_num);
fclose(fptr);


fptr=fopen(argv[1],"rb");
fptr2=fopen(argv[2],"rb");

/*read input files, decode I and Q, put in data file apply fft,
apply running average, divide signal fft averages by reference fft averages.
At end of input file, output text file with averaged data*/

   _setvideomode(_VRES2COLOR);

 {

 for(ss=0;ss<p_num;ss++)
    {

 for(s=0;s<2*pts;s++)
	{
	ucha=getc(fptr);
	ucha2=getc(fptr2);
	dats[s]=(float)ucha+0.0;	
	dats2[s]=(float)ucha2+0.0;

	if(dats[s]>128)
	    dats[s]=(dats[s]-127.5)/128.0;
	else dats[s]=(dats[s]-127.5)/128.0;

	if(dats2[s]>128)
	    dats2[s]=(dats2[s]-127.5)/128.0;
	else dats2[s]=(dats2[s]-127.5)/128.0;

	count++;
	
	}

	
/*take fourier transforms*/
	four(dats-1,pts,-1);
	four(dats2-1,pts,-1);

	coun=coun+1;	
	sum_dat();

    }

}
out_dat();
fclose(fptr);
fclose(fpto);
printf("\nInfileR=%s    InfileS=%s    Outfile=%s   FFT Points=%d\n",argv[1],argv[2],argv[3],atol(argv[4]));

exit(0);
}

void out_dat(void)
{
int tt;
float opp;

for(tt=0;tt<pts;tt++){
	if(tt<(pts/2)){ opp=(float)sqrt(datr2[tt+pts/2]/datr[tt+pts/2]);

	   }
	else { opp=(float)sqrt(datr2[tt-pts/2]/datr[tt-pts/2]);

	   }
fprintf(fpto,"%d    %3.7f    %3.7f\n",(tt-pts/2),(float)(opp));
}

fclose(fpto);
}


void sum_dat(void)
{
int tt;
float opp,re1,im1,re2,im2;
if(coun%10000==0) {    _clearscreen(_GCLEARSCREEN);}
if(coun>1){
for(tt=0;tt<pts;tt++){
re1=(float) (dats[2*tt+1]*dato1[2*tt]-dats[2*tt]*dato1[2*tt+1]);
im1=(float) (dats[2*tt]*dato1[2*tt]+dats[2*tt+1]*dato1[2*tt+1]);
datr[tt]=datr[tt]+(re1*re1+im1*im1);

re2=(float) (dats2[2*tt+1]*dato2[2*tt]-dats2[2*tt]*dato2[2*tt+1]);
im2=(float) (dats2[2*tt]*dato2[2*tt]+dats2[2*tt+1]*dato2[2*tt+1]);

datr2[tt]=datr2[tt]+(re2*re2+im2*im2);


if(coun%10000==0)

if(tt<pts/2)_setpixel((int)(10+400.0*(float)(tt)/(float)pts),(int)(470-470*sqrt(datr2[tt+pts/2]/(float)datr[tt+pts/2])));
else   _setpixel((int)(10+400.0*(float)tt/(float)pts),(int)(470-470*sqrt(datr2[tt-pts/2]/(float)datr[tt-pts/2])));

}
}

for(tt=0;tt<2*pts;tt++) {
 dato1[tt]=dats[tt];
 dato2[tt]=dats2[tt];

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
 }
  if(isign==1){for(a=0;a<2*pts;a++){
  data[a]=data[a]/pts;}}
 }

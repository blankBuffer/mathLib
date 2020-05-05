#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

class DoubleArrayObj{
	public:
		int size;
		double* array;
		DoubleArrayObj(){}
		DoubleArrayObj(double* array,int size){
			this->array = array;
			this->size = size;
		}
        void println(){
            for(int i = 0;i<size;i++) printf("%lf , ",array[i]);
            printf("\n");
        }
};

class Func{
	private:
		double searchSignFlip(double pos,double jumpSize){
			if(out(pos)>0){
				if(out(pos+jumpSize)>0) while(out(pos+jumpSize)>0) jumpSize*=2;
				else{
					while(out(pos+jumpSize)<0) jumpSize*=0.5;
					jumpSize*=2;
				}
			}
			else{
				if(out(pos+jumpSize)<0) while(out(pos+jumpSize)<0) jumpSize*=2;
				else{
					while(out(pos+jumpSize)>0) jumpSize*=0.5;
					jumpSize*=2;
				}
			}
			return jumpSize;
		}
	public:
		virtual double out(double x) = 0;
		virtual ~Func(){}
		double bisection(double min,double max,int cy){
			if(out(max)>out(min)){
				for(int i = 0;i<cy;i++){
					double mid = (min+max)*.5;
					if(out(mid)>0) max = mid;
					else min = mid;
				}
			}else{
				for(int i = 0;i<cy;i++){
					float mid = (min+max)*.5;
					if(out(mid)<0) max = mid;
					else min = mid;
				}
			}
			return min;
		}

		double searchSolution(double pos,double dir,int cy){
			double jumpSize = searchSignFlip(pos, dir);
			return bisection(pos+jumpSize*0.5, pos+jumpSize, cy);
		}
		void plot(int wid,int hei,double xMin,double xMax){
			double step = (xMax-xMin)/(double)wid;
			double x = xMin;
			char imageBuffer[hei][wid];
			double heightBuffer[wid];
			//init image buffer
			for(int j = 0;j<hei;j++){
				for(int i = 0;i<wid;i++){
					imageBuffer[j][i] = ' ';
				}
			}
			//init height buffer
			double currentOut = out(x);
			double min = currentOut,max = currentOut;
			for (int i = 0; i < wid; i++) {
				currentOut = out(x);
				heightBuffer[i] = currentOut;
				if(currentOut>max) max = currentOut;
				else if(currentOut<min) min = currentOut;
				x+=step;
			}
			double range = max-min;
			double nmin=min-range/3.0;
			double nmax=max+range/3.0;
			double nrange = nmax-nmin;

			//scale and plot
			for (int i = 0; i < wid; i++) {
				heightBuffer[i] = (((heightBuffer[i]-nmin)/(nrange))*hei);
				imageBuffer[(int)(heightBuffer[i])][i] = '#';
			}

			//x axis
			int yIn = (int)((-nmin/(nrange))*hei);
			if(yIn>-1&&yIn<hei){
				for(int i = 0;i<wid;i++) imageBuffer[yIn][i] = '-';
			}

			//printBuffer
			printf("\n");
			for(int j = hei-1;j>-1;j--){
				printf("|");
				for(int i = 0;i<wid;i++){
					printf("%c",imageBuffer[j][i]);
				}
				printf("|\n");
			}
			printf("\n");

		}
};

class Poly : public Func{
	public:
		double* c;
		int size;
		Poly(double* array,int size){
			c = array;
			this->size = size;
		}
		Poly(DoubleArrayObj* o){
			c = o->array;
			size = o->size;
		}

		double out(double x){
			double sum = 0;
			double prod = 1;
			for(int i = 0;i<size;i++){
				sum+=c[i]*prod;
				prod*=x;
			}
			return sum;
		}
		void solve(int cy,DoubleArrayObj* outPut){
			if(size<2) return;
			//contructing derivatives in 2d array
			int listSize = size-1;
			double eq[listSize][size];
			//clear
			for(int j = 1;j<listSize;j++) for(int i = 0;i<size;i++) eq[j][i] = 0;
			//og
			for(int i = 0;i<size;i++) eq[0][i] = c[i];
			//derive list
			for(int j = 1;j<listSize;j++){
				for(int i = 1;i<size;i++){
					eq[j][i-1] = eq[j-1][i]*i;
				}
			}
            //___________________________________


			//old solutionList
			int numberOfSolutions = 0;
			double solutionsMem[size-1];

            //new solutionList
            int numberOfSolutions2 = 0;
            double solutionsMem2[size-1];

			//solving the linear equasion to start

			solutionsMem[0] = -eq[listSize-1][0]/eq[listSize-1][1];
			numberOfSolutions++;


			//quadratic and beyond
			for(int i = listSize-2;i>-1;i--){

				Poly p(eq[i],size-i);
                //left----------------------------------
                //left solution exists if f(leftMost solution of deriv)*(leadingCo)<0 and if degree even
                //if degree odd f(leftMost solution of deriv)*(leadingCo)>0

				double prod = p.out(solutionsMem[0])*p.c[p.size-1];
                bool leftExist = false;

                int even = p.size%2==1;
				if(even&&prod<0) leftExist = true;
				else if(!even&&prod>0) leftExist = true;

                if(leftExist){
                    numberOfSolutions2++;
                    solutionsMem2[0] = p.searchSolution(solutionsMem[0],-1,cy);
                }
                //middle--------------------------------
                //middle solution if f(left)*f(right)<0
                for (int j = 0;j<numberOfSolutions-1; j++) {
                    prod = p.out(solutionsMem[j])*p.out(solutionsMem[j+1]);
                    if(prod<0){
                        numberOfSolutions2++;
                        solutionsMem2[numberOfSolutions2-1] = p.bisection(solutionsMem[j],solutionsMem[j+1], cy);
                    }
                }
                //right-----------------------------------
                prod = p.out(solutionsMem[numberOfSolutions-1])*p.c[p.size-1];
                bool rightExist = false;
                if(prod<0) rightExist = true;
                if(rightExist){
                    numberOfSolutions2++;
                    solutionsMem2[numberOfSolutions2-1] = p.searchSolution(solutionsMem[numberOfSolutions-1],1,cy);
                }
                //
                //copy solutionMem2 to solutionMem
                for(int j = 0;j<numberOfSolutions2;j++) solutionsMem[j] = solutionsMem2[j];
                numberOfSolutions = numberOfSolutions2;
                numberOfSolutions2 = 0;

			}
            outPut->size = numberOfSolutions;
            for(int i = 0;i<numberOfSolutions;i++) outPut->array[i] = solutionsMem[i];

		}
		~Poly(){}
};

struct winsize size;

void printSpacer(){
	for(int i = 0;i<size.ws_col;i++) printf("_");
	printf("\n");
}
void printInfoText(){
	printf(
    	"polynomial solver by Ben Currie\n"
    	"limited to roots that cross y axis\n"
    	"function that does work: x^4-x^3-5*x^2+5*x+1\n"
    	"function that does not work : x^2-4*x+4 : never crosses y axis\n"
	);
}
int main() {
	//get console window size
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    //info
	printSpacer();
	printInfoText();
    //decision to keep doing more
	char dec[2];
    dec[0] = 'y';
    while (dec[0]=='y'||dec[0]=='Y') {
		printSpacer();
        //get user Input for polynomial
        int len = -1;
        while(len<0){
            printf("degree: ");
            scanf("%d",&len);
        }
        len++;
        double m[len];
        for(int i = 0;i<len;i++){
            printf("x^%d: ",i);
            scanf("%lf",&m[i]);
        }
        //make polynomial object
        Poly p(m,len);

        //get precision
        int prec = -1;
        while(prec<0){
            printf("precision (16-128): ");
            scanf("%d",&prec);
        }
        //solve
        double res[len];
        DoubleArrayObj result;
        result.array = res;
        p.solve(prec,&result);
		result.println();
		//show plot?
		printf("show plot? (Y/y): ");
		char showPoly[2];
		scanf("%s", showPoly);
		if(showPoly[0]=='y'||showPoly[0]=='Y') if(result.size>1) p.plot(size.ws_col/2,size.ws_col/4,result.array[0],result.array[result.size-1]);
        //scan if they want to do another one
		printSpacer();
        printf("solve new polynomial? (Y/y): ");
        scanf("%s",dec);
    }
	return 0;
}

#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <vector>
#include <cstdint>

constexpr
int
W = 1280,H = 1024;
constexpr
double
RX=30.,RY=30.,RZ=300.,
XMAX=20.,XMIN=-20.,
YMAX=20.,YMIN=-20.,
STEP1=0.4, STEP2=0.01;
double sinc(double x){
	if(x==0) return 1.;
	return sin(x)/x;
}

double ff(double x,double y){
	return sinc(sqrt(x*x+y*y));
}

#pragma pack(push,1)
struct TGA{
	uint8_t id_len;
	uint8_t pal_type;
	uint8_t img_type;
	uint8_t pal_desc[5];
	uint16_t xpos,ypos;
	uint16_t w,h;
	uint8_t bpp;
	uint8_t img_desc;
};
#pragma pack(pop)

void plot(std::vector<uint32_t>& pixels, int x, int y, uint32_t c,std::vector<int>& h)
{
	if (x < 0 || x >= W)
		return;
	if ( y > h[x])
		return;
	h[x]=y;
	if(y<0)
		return;
	if ( (int)(y * W + x) <= (int)pixels.size())
		pixels[y*W + x] = c;

}

int main(){
	int sx=0,sy=0;
	double z;
	std::fstream f;
	TGA h {};
	std::vector<int> hor;
	hor.resize(W);
	for(auto && a:hor)
		a=H;
	std::vector <uint32_t> p;
	p.resize(W*H);
	for(auto && c:p)
		c=0xFFCC9293;

	h.img_type=2;
	h.w=W;
	h.h=H;
	h.bpp=32;
	h.img_desc=0x20;

	for(double i=XMAX;i>XMIN;i=i-STEP1){
		for(double j=YMAX;j>YMIN;j=j-STEP2){
			z=ff(i,j);
			sx = int( W / 2 - RX * i * cos( M_PI / 6 ) + RY * j * cos( M_PI/ 6 ) );
			sy = int( H / 2 + RX * i * sin( M_PI / 6 ) + RY * j * sin( M_PI / 6 ) - RZ * z);
			plot(p, sx, sy, 0xffffffff,hor);
		}
	}
	for(auto && a:hor)
			a=H;
	for(double j=YMAX;j>YMIN;j=j-STEP1){
			for(double i=XMAX;i>XMIN;i=i-STEP2){
				z=ff(i,j);
				sx = int( W / 2 - RX * i * cos( M_PI / 6 ) + RY * j * cos( M_PI/ 6 ) );
				sy = int( H / 2 + RX * i * sin( M_PI / 6 ) + RY * j * sin( M_PI / 6 ) - RZ * z);
				plot(p, sx, sy, 0xffffffff,hor);
			}
		}



f.open("output.tga",std::ios::out|std::ios::binary);
f.write(reinterpret_cast<char*>(&h),sizeof(TGA));
f.write(reinterpret_cast<char*>(&p[0]),W*H*4);
f.close();
return 0;
}





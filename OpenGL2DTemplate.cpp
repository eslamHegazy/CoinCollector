#include <stdio.h>
#include <math.h>
#include <random>
#include <glut.h>
#include <time.h>

float scr_h, scr_w, win_h, win_w, win_x, win_y;
float pl_x, pl_y, pl_w, pl_h;
int num_of_lanes;
int maze_th_x, maze_th_y;
int bridge_width;
float* lane_borders;
float  deg = 0;
int* x1;
int* x2;
int* x3;
int num_of_lane_borders;
float  lane_height, lane_border_height;

float player_width, player_height, player_x, player_y, player_speed;

void Display();
void dist();
//void drawRect(int x, int y, int w, int h, float r, float g, float b);
//void drawRect(float x, float y, float w, float h, float r, float g, float b);
void init(int n_lanes);
void drawRect(float x, float y, float w, float h, float r, float g, float b);
void drawCircle(int x, int y, float r);
void drawMazeBorder(int x_thickness, int y_thickness);
void drawLanes();
void randomBridges();
void handleCollisions(float dx, float dy);
void frees();
void Key(unsigned char key, int x, int y);
//void KeyUp(unsigned char key, int x, int y);
//void Mouse(int button, int state, int x, int y);
void Timer(int value);
/*
class lane_border {
public:
	static float lbr, lbg, lbb;
public:
	static float br, bg, bb;
public:
	static double height, width;
	double y, x1 ,x2, x3;
public:
	lane_border(double y) :y(y) {
		set_bridge_random();
	}
public:
	static void set_height(double lane_border_height)  {
		height = lane_border_height;
	}
public:
	static void set_width(double lane_border_width) {
		width = lane_border_width;
	}
public:
	static void set_lane_border_colors(float r, float g, float b) {
		lbr = r; lbg = g; lbb = b;
	}
public:
	static void set_bridge_colors(float r, float g, float b) {
		br = r; bg = g; bb = b;
	}
	void set_bridge_random() {
		x1 = 0;
		x2 = rand() % (int)(win_w - width + 1);
		x3 = x2 + width;
	}
	void draw() {
		drawRect(x1, y, (x2 - x1), height, lbr, lbg, lbb);
		drawRect(x2, y, (x3 - x2), height, br, bg, bb);
		drawRect(x3, y, ((pl_x+pl_w) - x3), height, lbr, lbg, lbb);
	}
};
*/

void main(int argc, char** argr) {
	glutInit(&argc, argr);
	int n_lanes = 5;
	init(n_lanes);
	

	glutCreateWindow("Wonderful Game");
	glutDisplayFunc(Display);
	//glutDisplayFunc(dist);

	glutKeyboardFunc(Key);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	gluOrtho2D(0.0, win_w, 0.0, win_h);
	glutTimerFunc(0, Timer, 0); // sets the Timer handler function; which runs every `Threshold` milliseconds (1st argument)

	glutMainLoop();

	frees();
}

void init(int n_lanes) {
	srand(time(NULL));
	scr_h = glutGet(GLUT_SCREEN_HEIGHT);
	scr_w = glutGet(GLUT_SCREEN_WIDTH);
	//printf("%dx%d\n", glutGet(GLUT_SCREEN_WIDTH),glutGet(GLUT_SCREEN_HEIGHT));
	//printf("%dx%d\n", glutGet(GLUT_SCREEN_WIDTH_MM), glutGet(GLUT_SCREEN_HEIGHT_MM));
	win_h = (scr_h * 0.8f);
	win_w = (scr_h * 3.0f / 4.0f);
	win_x = (scr_w - win_w) /2.0f;
	win_y = (scr_h - win_h) /2.0f;


	//glutInitWindowSize(300, 300);
	glutInitWindowSize(win_w, win_h);
	glutInitWindowPosition(win_x, win_y);
	
	pl_x = maze_th_x = (win_w * 0.06);
	pl_y = maze_th_y = (win_h * 0.06); //pl_y-=2;
	pl_w = win_w - 2.0f * pl_x;
	pl_h = win_h - 2.0f * pl_y;
	//maze_th_x = maze_th_y = win_w < win_h ? (0.05 * win_w) : (0.05 * win_h);

	printf("%f %f %f %f\n", maze_th_x, maze_th_y, win_w, win_h);

	num_of_lanes = n_lanes;
	num_of_lane_borders = num_of_lanes - 1;
	bridge_width = (int)(0.2 * pl_w);
	
	lane_height = pl_h / (num_of_lanes + 1);
	lane_border_height = lane_height / (num_of_lanes - 1);

	lane_borders = new float[num_of_lane_borders];
	x1 = new int[num_of_lane_borders];
	x2 = new int[num_of_lane_borders];
	x3 = new int[num_of_lane_borders];

	player_width = 0.12 * pl_w;
	player_height = 0.6 * lane_height;
	player_x = pl_x + 10;
	player_y = pl_y + 0.1 * lane_height;
	player_speed = 1;//0;

	randomBridges();

}

void drawRect(float x, float y, float w, float h, float r, float g, float b) {
	glColor3f(r, g, b);
	//printf("%f %f %f %f\n", x, y, w, h);
	glBegin(GL_POLYGON);
	glVertex2f(x, y);
	glVertex2f(x + w, y);
	glVertex2f(x + w, y + h);
	glVertex2f(x, y + h);
	glEnd();
}
void drawCircle(int x, int y, float r) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	GLUquadric* quadObj = gluNewQuadric();
	gluDisk(quadObj, 0, r, 50, 50);
	glPopMatrix();
}

void drawCoin() {
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(450, 450, 0);
	glScalef(0.5f, 0.5f, 1.0f);
	GLUquadric* quadObj = gluNewQuadric();
	gluDisk(quadObj, 25, 60, 50, 50);
	glPopMatrix();
	
}

void drawMazeBorder(int maze_th_x, int maze_th_y) {
	float r=0, g=0, b=0 ;
	drawRect(0, 0, win_w, maze_th_y, r, g, b );
	drawRect(0, 0, maze_th_x, win_h, r, g, b );
	//printf("\n%d %d", win_w, win_h);
	//int x_s = win_w - maze_th_y;
	//printf("\n\n%d", x_s);
	//drawRect(558, 0, maze_th_x, win_h, r, g, b);
	drawRect(win_w-maze_th_x, 0, maze_th_x, win_h, r, g, b);
	drawRect(0, win_h-maze_th_y, win_w, maze_th_y, r, g, b);
	
	r = 0.3f; g = 0.3f; b = 0.3f;
	int rad = ((maze_th_x < maze_th_y ? maze_th_x : maze_th_y) - 4)/3;
	int xys = 2;
	int xc = xys + rad;
	glColor3f(r, g, b);
	int y1 = xys + rad;
	int y2 = win_h - (maze_th_y) + xys + rad;
	while (xc + rad < win_w) {
		//printf("I am here\n");
		//printf("%f %f %d", xc + rad, y1, rad);
		drawCircle(xc , y1, rad);
		drawCircle(xc , y2, rad);
		xc += ((rad*2 ));
	}
	int yc = maze_th_y + rad;
	int x1 = xys + rad;
	int x2 = win_w - (maze_th_x)+xys + rad;
	while (yc + rad < pl_y + pl_h) {
		drawCircle(x1, yc + rad, rad);
		drawCircle(x2, yc + rad, rad);
		yc += ((rad * 2));
	}
}

void drawLanes() {
	//num_of_lane_borders = num_of_lanes-1;
	
	float cur_y = pl_y;
	//printf("cur_y=%d, lane_height=%f, lane_border_height=%f\n", cur_y, lane_height, lane_border_height);
	//printf("num_of_lanes=%d, num_of_lane_borders=%d\n", num_of_lanes, num_of_lane_borders);
	float lr = 0.9f, lg = 0.8f, lb =0.8f;
	float lbr = 0.7, lbg = 0.4, lbb = 0.1;
	float br = 0.5f, bg = 1.0f, bb =0.0f;
	drawRect(pl_x, pl_y, pl_w, pl_h, lr, lg, lb);
	//while (num_of_lanes-- > 0) {
	//	//drawRect(pl_x, cur_y, pl_w, lane_height, lr, lg, lb);
	//	cur_y += lane_height;
	//	if (num_of_lane_borders-- > 0) {
	//		drawRect(pl_x, cur_y, pl_w, lane_border_height, lbr, lbg, lbb);
	//		cur_y += lane_border_height;
	//	}
	//}
	printf("num_of_lane_borders=%d, lane_height=%f\n", num_of_lane_borders, lane_height);
	for (int i = 0; i < num_of_lane_borders; i++) {
		cur_y += lane_height;
		lane_borders[i] = cur_y;
		//drawRect(pl_x, cur_y, pl_w, lane_border_height, lbr, lbg, lbb);
		int x1i = x1[i];
		int x2i = x2[i];
		int x3i = x3[i];
		//printf("x1 x2 x3: %d %d %d\n", x1i, x2i, x3i);
		drawRect(pl_x+x1i, cur_y, (x2i - x1i), lane_border_height, lbr, lbg, lbb);
		drawRect(pl_x+x2i, cur_y, (x3i - x2i), lane_border_height, br, bg, bb);
		drawRect(pl_x+x3i, cur_y, (pl_w - x3i), lane_border_height, lbr, lbg, lbb);
		cur_y += lane_border_height;
	}
}

void randomBridges() {
	printf("pl_x=%f, pl_w=%f\n", pl_x, pl_w);
	for (int i = 0; i < num_of_lane_borders; i++) {
		x1[i] = 0;
		int rv = rand();
		/*int rv1 = rv % (int)(pl_w);
		int rv2 = rv % (int)(pl_w - bridge_width + 1);
		printf("%d %d %d\n", rv, rv1, rv2);*/
		x2[i] = rand() % (int)(pl_w - bridge_width ); // -10);//the 10 is some arbitary number to give space for x3
		x3[i] = x2[i] + bridge_width;
	}

}

void handleCollisions(float dx, float dy) {

}

void Display() {
	glClear(GL_COLOR_BUFFER_BIT);
	/*
	printf("scr_w=%d, scr_h=%d\n", scr_w, scr_h);
	printf("win_w=%d, win_h=%d\n", win_w, win_h);
	printf("pl_w=%d, pl_h=%d\n", pl_w, pl_h);
	printf("pl_x=%d, pl_y=%d\n", pl_x, pl_y);
	printf("mz_x=%d, mz_y=%d\n", maze_th_x, maze_th_y);
	*/
	drawMazeBorder(maze_th_x, maze_th_y);
	drawLanes();//15);
	
	glPushMatrix();
	glTranslatef(250, 80, 0);
	glRotatef(deg, 0.0f, 0.0f, 1.0f);
	glTranslatef(-250, -80, 0);
	glColor3f(0.7f, 0.7f, 0.7f);
	//drawCircle(250, 80, 25);
	drawRect(235, 65, 25, 25, 0.7f, 0.7f, 0.7f);
	glColor3f(0.83f, 0.69f, 0.21f);
	drawRect(240, 70, 15, 15, 0.83f, 0.69f, 0.21f);
	//glTranslatef(280, 115, 0);
	//glTranslatef(-280, -110, 0);
	//drawRect(250, 80, 60, 30, 0, 0, 0);
	
	glPopMatrix();

	drawRect(player_x, player_y, player_width, player_height, 1.0f, 0, 0);

	drawCoin();
	
	glFlush();
}

void Key(unsigned char key, int x, int y) {
	float dx = 0; float dy = 0;
	switch (key) {
	case 'w': case 'W': 
		dy += player_speed;
		break;
	case 's': case 'S':
		dy -= player_speed;
		break;
	case 'd': case 'D':
		dx += player_speed;
		break;
	case 'a': case 'A':
		dx -= player_speed;
		break;
	}
	handleCollisions(dx, dy);
}


void Timer(int value) {
	
	if (deg >= 360) deg = 0;
	deg += 5.0f;
	/*if (deg >= 0 && deg < 5) {
		deg += 0.1f;
	}
	else if ((deg >= 5 && deg < 10) || (deg>=135 && deg<145)) {
		deg += 0.2f;
	}
	else if ((deg >= 10 && deg < 20) || (deg>=125 && deg<135)) {
		deg += 0.5f;
	}
	else if ((deg >= 20 && deg < 30) || (deg>=115 && deg < 125)) {
		deg += 1.0f;
	}
	else if (deg == 30) {
		deg = 115;
	}
	else if (deg >= 210 && deg < 270) {
		deg += 10;
	}
	else {
		deg += 5;
	}*/
	/*
	if (deg >= 0 && deg < 90)
		deg += 15;
	else if (deg >= 90 && deg < 270) {
		deg += 31;
	}
	else {
		deg += 15;
	}
	*/
	// ask OpenGL to recall the display function to reflect the changes on the window
	glutPostRedisplay();

	// recall the Timer function after 20 seconds (20,000 milliseconds)
	glutTimerFunc(100, Timer, 0);
}



void frees() {
	if (!lane_borders) delete [] lane_borders;
	if (!x1) delete[] x1;
	if (!x2) delete[] x2;
	if (!x3) delete[] x3;

}

void drawCandy() {
	int X = 10; int Y = 20;
	//glBegin(GL_POLYGON);
	//drawRect(50, 50, 50, 50, 0.5, 0.5, 0.5);
	
	
}

void dist() {
	drawCandy();
	glFlush();
}
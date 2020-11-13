#include <stdio.h>
#include <string.h>
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
int* x1;
int* x2;
int* x3;
int num_of_lane_borders, num_coins, num_pu;
float  lane_height, lane_border_height;
int game_timer;
int* coins_x, *coins_y;
float coin_width, coin_height;
float  coins_deg;

int* pu_x, * pu_y, * pu_t;
float pu_width, pu_height;
float pu1_speedfactor;
char* pu1_str = "You have ultimate speed for 5 seconds";
int pu1_flag;

float player_width, player_height, player_x, player_y, player_speed;
float goal_w, goal_h, goal_x, goal_y;


int score = 0, win = 0;;

void Display();
void won();
void lost();
void dist();
//void drawRect(int x, int y, int w, int h, float r, float g, float b);
//void drawRect(float x, float y, float w, float h, float r, float g, float b);
void init(int n_lanes);
void drawRect(float x, float y, float w, float h, float r, float g, float b);
void drawCircle(int x, int y, float r);
void drawMazeBorder(int x_thickness, int y_thickness);
void drawLanes();
void goalPos();
void randomBridges();
void randomCoins();
void randomPowerups();
int notfree(float x1, float x2, float y1, float y2);
int collision(int p1x1, int p1x2, int p1y1, int p1y2,int p2x1, int p2x2, int p2y1, int p2y2);
void handleCollisions(float dx, float dy);

void frees();
void Key(unsigned char key, int x, int y);
//void KeyUp(unsigned char key, int x, int y);
//void Mouse(int button, int state, int x, int y);
void Timer(int value);
void RandomPos(int value);
void gameTimer(int value);

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
	glutTimerFunc(0, RandomPos, 0); // sets the Timer handler function; which runs every `Threshold` milliseconds (1st argument)
	glutTimerFunc(0, gameTimer, 0); // sets the Timer handler function; which runs every `Threshold` milliseconds (1st argument)
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

	num_coins = 4;
	coin_height = lane_height * 0.5;
	coin_width = coin_height;

	num_pu = 5;
	pu_width = 54;
	pu_height = 35;

	lane_borders = new float[num_of_lane_borders];
	x1 = new int[num_of_lane_borders];
	x2 = new int[num_of_lane_borders];
	x3 = new int[num_of_lane_borders];
	coins_x = new int[num_coins];
	coins_y = new int[num_coins];
	pu_x = new int[num_pu];
	pu_y = new int[num_pu];
	pu_t = new int[num_pu];
	pu1_speedfactor = 5;

	player_width = 0.12 * pl_w;
	player_height = 0.6 * lane_height;
	player_x = pl_x + 10;
	player_y = pl_y + 0.1 * lane_height;
	player_speed = 2;//0;

	
	goal_h = 0.5 * lane_height;//20;
	goal_w = goal_h;
	//goalPos();
	goal_x = pl_x + pl_w - 10 - goal_w;
	goal_y = pl_y + (lane_height+lane_border_height) * (num_of_lanes-1) + 0.25 * lane_height;
	
	game_timer = 45;
	
	randomBridges();
	randomCoins();
	randomPowerups();
	
}


//this is the method used to print text in OpenGL
//there are three parameters,
//the first two are the coordinates where the text is display,
//the third coordinate is the string containing the text to display
void print(int x, int y, char* string)
{
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
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

void drawOutline(float x, float y, float w, float h, float r, float g, float b) {
	glColor3f(r, g, b);
	//printf("%f %f %f %f\n", x, y, w, h);
	glBegin(GL_LINE_LOOP);
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
	//glPushMatrix();
	//glTranslatef(450, 450, 0);
	//glScalef(0.5f, 0.5f, 1.0f);
	GLUquadric* quadObj = gluNewQuadric();
	gluDisk(quadObj, 0, coin_width/2, 50, 50);
	glColor3f(0.9f, 0.5f, 0.9f);
	glBegin(GL_LINES);
	glVertex3f(-coin_width/2, 0, 0);
	glVertex3f(coin_width/2, 0, 0);
	glEnd();
	//glPopMatrix();
	
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
	
	glColor3f(0.4f, 0.3f, 0.75f);
	glBegin(GL_LINE_STRIP);
	glVertex3f(pl_x / 3, pl_y / 3, 0);
	glVertex3f(pl_x / 3, win_h - pl_y / 3, 0);
	glVertex3f(win_w - pl_x / 3, win_h - pl_y / 3, 0);
	glVertex3f(win_w - pl_x / 3, pl_y / 3, 0);
	glVertex3f(pl_x / 3, pl_y / 3, 0);
	/*glVertex3f(5, 5, 0);
	glVertex3f(150, 5, 0);
	glVertex3f(150, 150, 0);*/
	glEnd();

	//r = 0.3f; g = 0.3f; b = 0.3f;
	//int rad = ((maze_th_x < maze_th_y ? maze_th_x : maze_th_y) - 4)/3;
	//int xys = 2;
	//int xc = xys + rad;
	//glColor3f(r, g, b);
	//int y1 = xys + rad;
	//int y2 = win_h - (maze_th_y) + xys + rad;
	//while (xc + rad < win_w) {
	//	//printf("I am here\n");
	//	//printf("%f %f %d", xc + rad, y1, rad);
	//	drawCircle(xc , y1, rad);
	//	drawCircle(xc , y2, rad);
	//	xc += ((rad*2 ));
	//}
	//int yc = maze_th_y + rad;
	//int x1 = xys + rad;
	//int x2 = win_w - (maze_th_x)+xys + rad;
	//while (yc + rad < pl_y + pl_h) {
	//	drawCircle(x1, yc + rad, rad);
	//	drawCircle(x2, yc + rad, rad);
	//	yc += ((rad * 2));
	//}
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
	//printf("num_of_lane_borders=%d, lane_height=%f\n", num_of_lane_borders, lane_height);
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
		//TODO:Outline
		cur_y += lane_border_height;
	}
}

void drawCoins() {
	for (int i = 0; i < num_coins; i++) {
		if (coins_x[i] == -1) continue;
		glPushMatrix();
		glTranslatef(coin_width/2+coins_x[i], coin_height/2+coins_y[i], 0);
		glRotatef(coins_deg, 0, 0, 1.0f);
		//glScalef(coin_width, coin_height, 1);
		drawCoin();
		glPopMatrix();
	}
}
void drawPowerup0(){
	glColor3f(0.3f, 0.7f, 0.1f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0, 0);
	glVertex2f(54, 0);
	glVertex2f(27, 35);
	glEnd();
}
void FuncPowerup0() {//Key ; opens a bridge in front of the player
	int i = 0;
	while (i < num_of_lane_borders && player_y + player_height>pl_y + lane_borders[i]) {
		printf("%f < %f\n", player_y + player_height, pl_y + lane_borders[i]);
		i++;
	}
	printf("%d\n", i);
	if (i == num_of_lane_borders) return;
	float mar = (bridge_width - player_width) / 2;
	int x2_nb = player_x - pl_x - mar;
	if (x2_nb < 0) x2_nb = 0;
	int x3_nb = x2_nb + bridge_width;
	if (x3_nb > pl_w) {
		x3_nb = pl_w;
		x2_nb = x3_nb - bridge_width;
	}
	x2[i] = x2_nb;
	x3[i] = x3_nb;
}
void drawPowerup1() {
	glColor3f(0.3f, 0.1f, 0.8f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0, 0);
	glVertex2f(54, 0);
	glVertex2f(27, 35);
	glEnd();
}

void goalPos() {
	float ht = lane_height + lane_border_height;
	
	int lane_num = num_of_lanes-1;
	int x = pl_x; int y = ht*lane_num;
	while (1) {
		int x = pl_x + rand() % (int)(pl_w - goal_w);
		int y = pl_y + ht * lane_num + 0.25 * lane_height;
		if (!notfree(x, x + goal_w, y, y + goal_h)) {
			goal_x = x;
			goal_y = y;
			break;
		}
	}
	
}
void drawGoal() {
	glPushMatrix();
	glColor3f(0.5f, 0.5f, 0.5f);
	//printf("Drawing goal: %f %f %f %f", goal_x, goal_y, goal_w, goal_h);
	glTranslatef(goal_x + goal_w/2, goal_y + goal_h/2, 0);
	glScalef(goal_w / 20 , goal_h / 20, 0);
	glBegin(GL_POLYGON);
	glVertex3f(-10, -10, 0);
	glVertex3f(10, -10, 0);
	glVertex3f(10, 10, 0);
	glVertex3f(-10, 10, 0);
	glEnd();
	glPopMatrix();
}
void stopFuncPowerup1(int val) {
	pu1_flag = 0;
	player_speed /= pu1_speedfactor;
}
void FuncPowerup1() {//Increase speed of the player for a specific amount of time
	player_speed *= pu1_speedfactor;
	pu1_flag = 1;
	
	glutTimerFunc(5000, stopFuncPowerup1, 0);
}

void drawPowerups() {
	for (int i = 0; i < num_pu; i++) {
		if (pu_x[i] == -1) continue;
		glPushMatrix();
		glTranslatef(pu_x[i] , pu_y[i], 0);
		//glScalef(coin_width, coin_height, 1);
		pu_t[i] ? drawPowerup0() : drawPowerup1();
		glPopMatrix();
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

void randomCoins() {
	float ht = lane_height + lane_border_height;
	for (int i = 0; i < num_coins; i++) {
		int lane_num = rand() % num_of_lanes;
		int x = pl_x + rand() % (int)(pl_w - coin_width);
		int y = pl_y + ht * lane_num + 0.25 * lane_height;
		if (notfree(x, x + coin_width, y, y + coin_height)) {
			printf("Wrong Guess: x=%d y=%d\n", x, y);
			i--;
		}
		else {
			coins_x[i] = x;
			coins_y[i] = y;
		}
	}

}
void randomPowerups() {
	float ht = lane_height + lane_border_height;
	for (int i = 0; i < num_pu; i++) {
		if (pu_x[i] == -1) continue;
		int lane_num = rand() % (i%2?num_of_lanes:(num_of_lanes-1));
		
		printf("lane num = %d\n", lane_num);
		int x = pl_x + rand() % (int)(pl_w - pu_width);
		int y = pl_y + ht * lane_num + 0.25 * lane_height;
		if (notfree(x, x + pu_width, y, y + pu_height)) {
			printf("PU:Wrong Guess: x=%d y=%d\n", x, y);
			i--;
		}
		else {
			pu_x[i] = x;
			pu_y[i] = y;
			pu_t[i] = i % 2;
		}
	}

}

int notfree(float x1, float x2, float y1, float y2) {
	int res = 0;
	/* Check for Coins*/
	for (int i = 0; i < num_coins; i++) {
		res += collision(x1, x2, y1, y2, coins_x[i], coins_x[i] + coin_width, coins_y[i], coins_y[i] + coin_height);
	}
	res += collision(x1, x2, y1, y2, player_x, player_x + player_width, player_y, player_y + player_height);
	
	/* Check For Powerups*/
	for (int i = 0; i < num_pu; i++) {
		res += collision(x1, x2, y1, y2, pu_x[i], pu_x[i] + pu_width, pu_y[i], pu_y[i] + pu_height);
	}
	/* Check for the player*/
	res += collision(x1, x2, y1, y2, player_x, player_x + player_width, player_y, player_y + player_height);

	/* Check For Goal*/
	res += collision(x1, x2, y1, y2, goal_x, goal_x+goal_w, goal_y, goal_y+goal_h);

	return res;
}

int collision(int p1x1, int p1x2, int p1y1, int p1y2,
	int p2x1, int p2x2, int p2y1, int p2y2) {
	return p1x1<p2x2 && p1x2>p2x1 &&
		p1y1<p2y2 && p1y2>p2y1;
}
void handleCollisions(float dx, float dy) {
	float old_x1 = player_x;
	float old_y1 = player_y;
	float new_x1 = old_x1 + dx;
	float new_y1 = old_y1 + dy;
	float old_x2 = old_x1 + player_width;
	float new_x2 = new_x1 + player_width;
	float old_y2 = old_y1 + player_height;
	float new_y2 = new_y1 + player_height;
	/* Collisions with maze borders*/
	if (new_x1 < pl_x) {
		new_x1 = pl_x;
	}
	else if (new_x2 > pl_x + pl_w) {
		new_x1 = pl_x + pl_w - player_width;
	}
	if (new_y1 < pl_y) {
		new_y1 = pl_y;
	}
	else if (new_y2 > pl_y + pl_h) {
		new_y1 = pl_y + pl_h - player_height;
	}

	/* Collisions with lane borders*/
	int coll = 0;
	
	for (int i = 0; i < num_of_lane_borders && !coll; i++) {
		int x1i = x1[i]+pl_x;
		int x2i = x2[i]+pl_x;
		int x3i = x3[i]+pl_x;
		float yb1 = lane_borders[i];
		float yb2 = yb1 + lane_border_height;
		int coll1 = collision(new_x1, new_x2, new_y1, new_y2, x1i, x2i, yb1, yb2);
		int coll2 = collision(new_x1, new_x2, new_y1, new_y2, x3i, pl_x + pl_w, yb1, yb2);
		int coll3 = collision(old_x1, old_x2, old_y1, old_y2, x2i, x3i, yb1, yb2);
		
		if (dx == 0) {
			if (coll1 || coll2) {
				if (dy > 0) {
					new_y1 = yb1 - player_height;
					new_y2 = yb1;
				}
				else {
					new_y1 = yb2;
					new_y2 = yb2 + player_height;
				}
			}
			//else {
			//	//handling too much speed that the player may cross the border without overlapping with it
			//	if(old_y1<yb1&& new_y1>yb2) {
			//		new_y1 = yb1 - player_height;
			//	}
			//	else if (old_y1 > yb2 && new_y1 < yb1) {
			//		new_y1 = yb2;
			//	}
			//}
		}

		/*
		if ( (old_y<yb1 && new_y>yb1) || (old_y+player_height<yb1 && new_y+player_height>yb1 )){
			if ((old_x <= x2i && new_x < x2i) || (new_x + player_width > x3i)) {
				new_y = yb1 - player_height;
				coll++;
			}
		}
		if ((old_x >= x2i && old_x + player_width <= x3i)) {
			if (new_x + player_width > x3i) {
				if (old_y<yb2 && old_y + player_height>yb1) {
					new_x = x3i - player_width;
					coll++;
				}
			}
			else if (new_x < x2i) {
				if (old_y<yb2 && old_y + player_height>yb1) {
				new_x = x3i - player_width;
				coll++;
				}
			}
		}
		*/

		if (dy == 0) {
			if (coll3) {
				if (coll1) {
					//printf("coll1: new_x1=%f new_x2=%f x1i=%d x2i=%d\n",new_x1,new_x2,x1i,x2i);
					printf("coll1: new_y1=%f new_y2=%f yb1=%f yb2=%f\n", new_y1, new_y2, yb1, yb2);
					new_x1 = x2i;
					new_x2 = x2i + player_width;
				}

				if (coll2) {
					//printf("coll1: new_x1=%f new_x2=%f x1i=%d x2i=%d\n", new_x1, new_x2, x1i, x2i);
					printf("coll2: new_y1=%f new_y2=%f yb1=%f yb2=%f\n", new_y1, new_y2, yb1, yb2);
					new_x1 = x3i - player_width;
					new_x2 = x3i;
				}
			}
		}
	}
	
	/* Collecting Coins*/
	for (int i = 0; i < num_coins; i++) {
		if (coins_x[i] == -1) continue;
		if (collision(new_x1, new_x2, new_y1, new_y2, coins_x[i], coins_x[i] + coin_width, coins_y[i], coins_y[i] + coin_height)) {
			score++;
			coins_x[i] = coins_y[i] = -1;	
		}
	}

	/* Getting a Powerup*/
	for (int i = 0; i < num_pu; i++) {
		if (pu_x[i] == -1) continue;
		if (collision(new_x1, new_x2, new_y1, new_y2, pu_x[i], pu_x[i]+pu_width, pu_y[i], pu_y[i] + pu_height)) {
			pu_t[i] ? FuncPowerup1() : FuncPowerup0();
			pu_x[i] = pu_y[i] = pu_t[i]= -1;
		}
	}
	/* Acquiring Goal */
	if (collision(new_x1, new_x2, new_y1, new_y2, goal_x, goal_x + goal_w, goal_y, goal_y + goal_h)) {
		win = 1;
		glutDisplayFunc(won);
	}

	player_x = new_x1;
	player_y = new_y1;
}

void Display() {

	glClear(GL_COLOR_BUFFER_BIT);

	drawMazeBorder(maze_th_x, maze_th_y);
	drawLanes();//15);
	
	drawCoins();
	drawPowerups();
	drawGoal();
	

	drawRect(player_x, player_y, player_width, player_height, 1.0f, 0, 0);
	
	glColor3f(1, 0, 0);
	char* p0s[20];
	sprintf((char*)p0s, "Score: %d", score);
	if (pu1_flag) {
		sprintf((char*)p0s, "%s  %s",(char*)p0s, pu1_str);
	}
	print(pl_x, win_h - 30, (char*) p0s);

	char* tim[20];
	sprintf((char*)tim, "Remaining Time:%d seconds", game_timer);
	print(pl_x, 3, (char*)tim);

	
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
	
	if (coins_deg == 360) coins_deg = 0;
	coins_deg += 5.0f;

	// ask OpenGL to recall the display function to reflect the changes on the window
	glutPostRedisplay();

	// recall the Timer function after 20 seconds (20,000 milliseconds)
	glutTimerFunc(100, Timer, 0);
}

void RandomPos(int value) {
	randomBridges();
	randomPowerups();
	handleCollisions(0, 0);
	glutTimerFunc(12000, RandomPos,0);
}
void gameTimer(int value) {
	if (game_timer) {
		game_timer--;
	}
	else {//ended
		if (!win) {
			glutDisplayFunc(lost);
		}
	}
	glutTimerFunc(1000, gameTimer, 0);
}

void frees() {
	if (!lane_borders) delete [] lane_borders;
	if (!x1) delete[] x1;
	if (!x2) delete[] x2;
	if (!x3) delete[] x3;
	if (!coins_x) delete[] coins_x;
	if (!coins_y) delete[] coins_y;
	if (!pu_x) delete[] pu_x;
	if (!pu_y) delete[] pu_y;
	if (!pu_t) delete[] pu_t;
}

void drawCandy() {
	int X = 10; int Y = 20;
	//glBegin(GL_POLYGON);
	//drawRect(50, 50, 50, 50, 0.5, 0.5, 0.5);
	
	
}

void won() {
	glClear(GL_COLOR_BUFFER_BIT);
	char* str = new char[100];
	sprintf(str, "Congrats !  Yoo have won with score=%d", score);
	print(pl_x, pl_h - pl_y, (char*)str);
	glFlush();
}
void lost() {
	glClear(GL_COLOR_BUFFER_BIT);
	char* str = new char[100];
	sprintf(str, "Game Over!  Time ended   Your score:%d", score);
	print(pl_x, pl_h - pl_y, (char*)str);
	glFlush();
}

void dist() {
	drawCandy();
	glFlush();
}
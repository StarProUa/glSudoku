#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

int screenWidth = 630, screenHeight = 630;

#define MapSize 9
#define StaticNum 20 // number of static digits during generation

GLFWwindow *window;

int global_x; // x coord cursor
int global_y; // y coord cursor

struct Map
{
	bool staticCell; // cell that contains a static number
	int cell; // cell that contains a simple number
};

Map map[MapSize][MapSize]; // map in matrix view

bool checkWin; // if true, the player wins

int searchSection(int coord) // знаходить границю секції певного числа
{
	coord++;

	float temp = coord / (float) 3;

	if(temp <= 1) return 2;
	else if(temp > 1 && temp <= 2) return 5;
	else if(temp > 2) return 8;

	return 0;
}

bool Detect(const int &num, const int &i, const int &j) // checking the intersection of duplicate numbers in a row and column
{
	if(num != 0)
	{
		for(int k = 0; k < MapSize; k++)
		{
			if(num == map[k][j].cell && k != i) return true;  // horizontal check
			if(num == map[i][k].cell && k != j) return true;  // vertical check
		}
	}

	return false;
}

bool checkCells() // checking whether all the numbers are arranged correctly
{
	for(int i = 0; i < MapSize; i++)
	{
		for(int j = 0; j < MapSize; j++)
		{
			int num = map[i][j].cell;

			return Detect(num, i, j);
		}
	}
	return false;
}

bool checkAmount() // check if the entire field is filled with numbers
{
	int count = 0;
	for(int i = 0; i < MapSize; i++)
	{
		for(int j = 0; j < MapSize; j++)
		{
			if(map[i][j].cell != 0)
			{
				count++ ;
			}
		}
	}
	return 81 - count == 0;
}

bool checkSection(const int &num, const int &lim_x, const int &lim_y) // searches for the boundaries of the section in which the number is located
{
	for(int i = lim_x - 2; i <= lim_x; i++)
	{
		for(int j = lim_y - 2; j <= lim_y; j++)
		{
			if(num == map[i][j].cell)
			{
				return true;
			}
		}
	}
	return false;
}

void NewGame()
{
	checkWin = false;

	for(int i = 0; i < MapSize; i++) // clear field
	{
		for(int j = 0; j < MapSize; j++)
		{
			map[i][j].cell = 0;
			map[i][j].staticCell = false;
		}
	}

	for(int i = 0; i < StaticNum; i++) // generate of static numbers on the filed
	{
		int rand_x = rand()% MapSize;
		int rand_y = rand()% MapSize;

		if(map[rand_x][rand_y].cell == 0)
		{
			// look for the boundaries of the section in which the number is located
			int lim_x = searchSection(rand_x);
			int lim_y = searchSection(rand_y);

			int rand_num;

			do
			{
				rand_num = rand()% 9;
			}
			while(checkSection(rand_num, lim_x, lim_y)
					 || Detect(rand_num, rand_x, rand_y)); // checking if the random satisfies the conditions

			map[rand_x][rand_y].cell = rand_num;
			map[rand_x][rand_y].staticCell = true; // indicate that the number is constant
		}
		else // otherwise, do an additional iteration
		{
			i--;
		}
	}
}

void drawLine(float x1, float y1, float x2, float y2) // draw line)
{
	glVertex2f(x1,y1);
	glVertex2f(x2,y2);
}

void showCount(int num) // draw digits
{
	switch(num)
	{
		case 1: // yellow
			glColor3f(1,1,0);
			break;

		case 2: // green
			glColor3f(0,1,0);
			break;

		case 3: // red
			glColor3f(1,0,0);
			break;

		case 4: // azure
			glColor3f(0,0,1);
			break;

		case 5: // blue
			glColor3f(0,1,1);
			break;

		case 6: // pink
			glColor3f(1,0,1);
			break;

		case 7: // black
			glColor3f(0,0,0);
			break;

		case 8: // light-azure
			glColor3f(0,0.5,1);
			break;

		case 9: // orange
			glColor3f(1,0.65,0);
			break;
	}

	glPushMatrix();

	glLineWidth(3);
	glBegin(GL_LINES);
		if((num != 1) && (num != 4)) drawLine(0.3,0.85,0.7,0.85);
		if((num != 0) && (num != 1) && (num != 7)) drawLine(0.3,0.5,0.7,0.5);
		if((num != 1) && (num != 4) && (num != 7)) drawLine(0.3,0.15,0.7,0.15);
		if((num != 5) && (num != 6)) drawLine(0.7,0.5,0.7,0.85);
		if(num != 2) drawLine(0.7,0.5,0.7,0.15);
		if((num != 1) && (num != 2) && (num != 3) && (num != 7)) drawLine(0.3,0.5,0.3,0.85);
		if((num == 0) || (num == 2) || (num == 6) || (num == 8)) drawLine(0.3,0.5,0.3,0.15);
	glEnd();

	glPopMatrix();
}

void setField(int x, int y) // selection cursor
{
	glPushMatrix();

	glTranslatef(x, y, 0);
	glLineWidth(4);
	glColor3f(1,0,0);
	glBegin(GL_LINE_LOOP);
	drawLine(0.1, 0.1, 0.1, 0.9);
	drawLine(0.9, 0.9, 0.9, 0.1);
	glEnd();

	glPopMatrix();
}

void emptyField() // draw field
{
	glPushMatrix();

	glTranslatef(0.1,0.1,0);
	glScalef(0.8,0.8,0);
	glBegin(GL_POLYGON);
	drawLine(0, 0, 0, 1);
	drawLine(1, 1, 1, 0);
	glEnd();

	glPopMatrix();
}

void Cells() // draw cell
{
	glPushMatrix();
	glLineWidth(3);
	glColor3f(0,0,0);
	for(int i = 0; i < 4; i++)
	{
		glTranslatef(3,0,0);
		glBegin(GL_LINES);
		drawLine(0,0,0,9);
		glEnd();
		if(i == 1)
		{
			glRotatef(90,0,0,1);
			glTranslatef(0,-3,0);
		}
	}
	glPopMatrix();
}

void setCell(const int &num) // sets a digit to a specific cell
{
	if(map[global_x][global_y].staticCell == false) map[global_x][global_y].cell = num;
}

void Keyboard(GLFWwindow *window, int key, int scode, int action, int mode) // keystroke processing
{
		 if(key == GLFW_KEY_1 && action == GLFW_PRESS) setCell(1);
	else if(key == GLFW_KEY_2 && action == GLFW_PRESS) setCell(2);
	else if(key == GLFW_KEY_3 && action == GLFW_PRESS) setCell(3);
	else if(key == GLFW_KEY_4 && action == GLFW_PRESS) setCell(4);
	else if(key == GLFW_KEY_5 && action == GLFW_PRESS) setCell(5);
	else if(key == GLFW_KEY_6 && action == GLFW_PRESS) setCell(6);
	else if(key == GLFW_KEY_7 && action == GLFW_PRESS) setCell(7);
	else if(key == GLFW_KEY_8 && action == GLFW_PRESS) setCell(8);
	else if(key == GLFW_KEY_9 && action == GLFW_PRESS) setCell(9);

	if(key == GLFW_KEY_SPACE && action == GLFW_PRESS) NewGame();

	if(key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		for(int j = 0; j < 9; j++)
		{
			for(int i = 0; i < 9; i++)
			{
				map[i][j].cell = 0;
				map[i][j].staticCell = false;
			}
		}
	}
}

void Draw() // main draw
{
	for(int i = 0; i < MapSize; i++)
	{
		for(int j = 0; j < MapSize; j++)
		{
			if(checkWin)
			{
				glColor3f(0.5,1,0.5);
			}
			else
			{
				if(map[i][j].staticCell) glColor3f(0.39,0.39,0.39);
				else glColor3f(0.5,0.5,0.5);
			}
			glPushMatrix();

			glTranslatef(i,j,0);

			emptyField();

			if(map[i][j].cell > 0)
			{
				showCount(map[i][j].cell);
			}
			if(checkAmount() && !checkCells())
			{
				cout << "You Win!" << endl;
				checkWin = true;
			}

			glPopMatrix();
		}
	}

	setField(global_x, global_y);
	Cells();
}

void getPos(GLFWwindow *window, double Xpos, double Ypos) // get cursor coords
{
	global_x = int(Xpos/(screenWidth/MapSize));
	global_y = int((MapSize ) - Ypos/(screenHeight/MapSize));
	//cout << "x = " << global_x << " y = " << global_y << endl;
}

int main()
{
	srand(time(NULL));

	if(!glfwInit())
	{
		cout << "Failed to init library";
		return -1;
	}

	window = glfwCreateWindow(screenWidth, screenHeight, "Sudoku", nullptr, nullptr);

	if(!window)
	{
		cout << "Error to create window";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glClearColor(0.9,0.9,0.9,1);
	glOrtho(0, MapSize, 0, MapSize, 0, 1);

	glfwSetKeyCallback(window, Keyboard);
	glfwSetCursorPosCallback(window, getPos);

	glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
	{
		screenWidth = width;
		screenHeight = height;

		glViewport(0, 0, width, height);
	});

	NewGame();

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		Draw();

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

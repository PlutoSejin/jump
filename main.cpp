#include <bangtal.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define TPS 50
#define G 3


SceneID scene1, scene2, scene3;
ObjectID title, button1, button2, button3, button4, game_over;
ObjectID player, ground, wall1, wall2;
ObjectID traps[6];

TimerID timer1;

bool isStarted = false;
bool arr[6];

int x_speed = 0;
int y_speed = 0;
int x = 0;
int y = 0;
int t = 0;
int screenY = 120;
int jump_counter = 0;



void end() {
	isStarted = false;
	enterScene(scene3);
}

bool isCollided() {
	if (!arr[(y - 11) / 160 - t] && x <= - 176) return true;
	if (arr[(y - 11) / 160 - t] && x >= 176) return true;
	if (!arr[(y + 84) / 160 - t] && x <= -201) return true;
	if (arr[(y + 84) / 160 - t] && x >= 201) return true;
	return false;
}

void draw() {
	locateObject(player, scene2, 590 + x, y - screenY + 345);
	locateObject(ground, scene2, 0, 120 - screenY);
	for (int i = 0; i < 6; i++) {
		locateObject(traps[i], scene2, arr[i] ? 857 : 398, (t + i) * 160 - screenY + 360);
	}
}

void tick(TimerID timer) {
	if (y <= 0 && y_speed < 0) {
		if (x_speed < 0) setObjectImage(player, "Images/player_sit_left.png");
		else setObjectImage(player, "Images/player_sit_right.png");
		x_speed = 0;
		y_speed = 0;
		y = 0;
		jump_counter = 0;
	}
	else if (jump_counter != 0 && y > 0) {
		y_speed -= G;
		if ((x <= -200 && x_speed < 0) || (x >= 200 && x_speed > 0)) {
			x_speed = 0;
			y_speed = 0;
			jump_counter = 0;
			if (x < 0) setObjectImage(player, "Images/player_left.png");
			else setObjectImage(player, "Images/player_right.png");
		}
	}
	x += x_speed;
	y += y_speed;
	if (screenY < y + 120) screenY = y + 120;
	if (y + 360 < screenY || isCollided()) end();

	if (screenY - 360 > t * 160 + 160) {
		t++;
		ObjectID n = traps[0];
		for (int i = 0; i < 5; i++) {
			arr[i] = arr[i + 1];
			traps[i] = traps[i + 1];
		}
		traps[5] = n;
		arr[5] = rand() % 2;
		if (arr[5]) setObjectImage(traps[5], "Images/trap_right.png");
		else setObjectImage(traps[5], "Images/trap_left.png");
	}

	draw();

	if (isStarted) {
		setTimer(timer1, 1.f / TPS);
		startTimer(timer1);
	}
}

void initalize() {
	srand(time(0));

	x_speed = 0;
	y_speed = 0;
	x = 0;
	y = 0;
	t = 0;

	setObjectImage(player, "Images/player_sit_right.png");
	jump_counter = 0;
	screenY = 120;

	for (int i = 0; i < 6; i++) {
		arr[i] = rand() % 2;
		if (arr[i]) setObjectImage(traps[i], "Images/trap_right.png");
		else setObjectImage(traps[i], "Images/trap_left.png");
		locateObject(traps[i], scene2, arr[i] ? 857 : 400, 240 + i * 360);
	}
}

void start() {
	isStarted = true;
	initalize();
	enterScene(scene2);
	setTimer(timer1, 1.0 / TPS);
	startTimer(timer1);
}

void mouseCallback(ObjectID object, int evtx, int evty, MouseAction action) {
	if (object == button1) {
		if (!isStarted) {
			start();
		}
	}
	else if (object == button2) {
		if (!(x <= -200 || jump_counter == 2)) {
			setObjectImage(player, "Images/player_jump_left.png");
			jump_counter++;
			x_speed = -15;
			y_speed = 35;
		}
	}
	else if (object == button3) {
		if (!(x >= 200 || jump_counter == 2)) {
			setObjectImage(player, "Images/player_jump_right.png");
			jump_counter++;
			x_speed = 15;
			y_speed = 35;
		}
	}
	else if (object == button4) {
		enterScene(scene1);
	}
}

int main() {
	setMouseCallback(mouseCallback);
	setTimerCallback(tick);
	timer1 = createTimer(1.0 / TPS);


	scene1 = createScene("main", "Images/background.png");


	title = createObject("Images/title.png");
	locateObject(title, scene1, 360, 400);
	showObject(title);

	button1 = createObject("Images/button1.png");
	locateObject(button1, scene1, 451, 100);
	showObject(button1);
	


	scene2 = createScene("game", "Images/background.png");


	wall1 = createObject("Images/wall.png");
	locateObject(wall1, scene2, 0, 0);
	showObject(wall1);

	wall2 = createObject("Images/wall.png");
	locateObject(wall2, scene2, 880, 0);
	showObject(wall2);

	ground = createObject("Images/ground.png");
	locateObject(ground, scene2, 0, 0);
	showObject(ground);

	player = createObject("Images/player_sit_right.png");
	locateObject(player, scene2, 620, 100);
	showObject(player);


	button2 = createObject("Images/button2.png");
	locateObject(button2, scene2, 900, 100);
	showObject(button2);

	button3 = createObject("Images/button3.png");
	locateObject(button3, scene2, 1000, 100);
	showObject(button3);

	for (int i = 0; i < 6; i++) {
		traps[i] = createObject("trap_left.png");
		showObject(traps[i]);
	}



	scene3 = createScene("game over", "Images/background.png");

	game_over = createObject("Images/game_over.png");
	locateObject(game_over, scene3, 360, 400);
	showObject(game_over);

	button4 = createObject("Images/button4.png");
	locateObject(button4, scene3, 584, 160);
	showObject(button4);

	startGame(scene1);
}

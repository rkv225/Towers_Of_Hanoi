#include<stdio.h>
#include<conio.h>
#include<graphics.h>
#include<dos.h>

//declaring the functions
void selectionMenu();
void drawPegs();
void initDisk();
void initPegs();
void drawDisk(int);
void moveDisk(int, int, int);
struct diskDef calcDest(int, int);
void autoTowers(int, int, int, int);
void manualTowers();
void showMouse();
struct coor getCoordinates();
int identifyPeg();
int getPeg();
int pegOfDisk(int);
int identifyDisk();
int getDisk();
int topDisk(int);

//mouse in and out
union REGS in, out;

//count of the number of disk on a peg
int diskCount[4];

//total number of disks 
int totalNumDisks;

//maxheight of the display
int maxHeight = 50;

//delay variable
int delayVal = 5;

//definition of a disk or a peg
struct diskDef
{
	int left;
	int top;
	int right;
	int bottom;
};

//declaring disks
struct diskDef disk[9];

//declaring the pegs
struct diskDef peg[4];

//structure of coordinates
struct coor
{
	int x;
	int y;
};

//selection menu
void selectionMenu()
{
	int i;
	struct coor point;
	cleardevice();
	setfillstyle(SOLID_FILL, 15);
	outtextxy(250, 100, "CHOOSE OPERATING MODE :");
	setcolor(0);
	bar(50, 150, 300, 300);
	outtextxy(100, 220, "AUTOMATIC MODE");
	bar(350, 150, 600, 300);
	outtextxy(400, 220, "MANUAL MODE");
	setcolor(15);
	while (!kbhit())
	{
		point = getCoordinates();
		if (point.x>50 && point.x<300 && point.y>150 && point.y<300)
		{
			for (i = 0; i < 4; i++)
				diskCount[i] = 0;
			printf("auto");
			cleardevice();
			printf("Enter the number of disks:\n");
			scanf("%d", &totalNumDisks);
			diskCount[1] = totalNumDisks;
			cleardevice();
			drawPegs();
			initDisk();
			autoTowers(totalNumDisks, 1, 3, 2);
			getch();
			break;
		}
		if (point.x>350 && point.x<600 && point.y>150 && point.y<300)
		{
			for (i = 0; i < 4; i++)
				diskCount[i] = 0;
			printf("manual");
			cleardevice();
			printf("Enter the number of disks:\n");
			scanf("%d", &totalNumDisks);
			diskCount[1] = totalNumDisks;
			cleardevice();
			drawPegs();
			initDisk();
			manualTowers();
			break;
		}
	}
}

//drawing pegs 
void drawPegs()
{
	int i;
	setfillstyle(CLOSE_DOT_FILL, 15);
	initPegs();
	for (i = 1; i < 4; i++)
	{
		bar(peg[i].left, peg[i].top, peg[i].right, peg[i].bottom);//drawing peg
	}
	setfillstyle(SOLID_FILL, 15);
	outtextxy(100, 380, "Peg A");
	outtextxy(300, 380, "Peg B");
	outtextxy(500, 380, "Peg C");
}

//function to initialise the pegs
void initPegs()
{	
	//peg 1
	peg[1].left = 20;
	peg[1].top = 350;
	peg[1].right = 200;
	peg[1].bottom = 370;
	//peg 2
	peg[2].left = 230;
	peg[2].top = 350;
	peg[2].right = 410;
	peg[2].bottom = 370;
	//peg 3
	peg[3].left = 440;
	peg[3].top = 350;
	peg[3].right = 620;
	peg[3].bottom = 370;
}

//function to initialise the disks on peg
void initDisk()
{
	int i;
	for (i = totalNumDisks; i>0; i--)
	{
		disk[i].left = 20 + (10 * (totalNumDisks - i + 1));
		disk[i].top = 350 - (25 * (totalNumDisks - i + 1));
		disk[i].right = 200 - (10 * (totalNumDisks - i + 1));
		disk[i].bottom = 370 - (25 * (totalNumDisks - i + 1));
		drawDisk(i);
	}
}

//function for drawing disk
void drawDisk(int num)
{
	setfillstyle(SOLID_FILL, num);//setting color of the disk
	bar(disk[num].left, disk[num].top, disk[num].right, disk[num].bottom);
}

//function to move disk from one peg to other
void moveDisk(int dn, int fromPeg, int toPeg)
{
	int i, k;
	struct diskDef tmpDisk = calcDest(dn, toPeg);

	//moving the disk up
	while (disk[dn].top >= maxHeight)
	{
		disk[dn].top--;
		disk[dn].bottom--;
		cleardevice();
		drawPegs();
		for (i = 1; i <= totalNumDisks; i++)
		{
			drawDisk(i);
		}
		delay(delayVal);
	}

	if (fromPeg - toPeg < 0)//move right
	{
		while (disk[dn].left <= tmpDisk.left)
		{
			disk[dn].left++;
			disk[dn].right++;
			cleardevice();
			drawPegs();
			for (i = 1; i <= totalNumDisks; i++)
			{
				drawDisk(i);
			}
			delay(delayVal);
		}
	}
	else//move left
	{
		while (disk[dn].left >= tmpDisk.left)
		{
			disk[dn].left--;
			disk[dn].right--;
			cleardevice();
			drawPegs();
			for (i = 1; i <= totalNumDisks; i++)
			{
				drawDisk(i);
			}
			delay(delayVal);
		}
	}

	//move down
	while (disk[dn].top <= tmpDisk.top)
	{
		disk[dn].top++;
		disk[dn].bottom++;
		cleardevice();
		drawPegs();
		for (i = 1; i <= totalNumDisks; i++)
		{
			drawDisk(i);
		}
		delay(delayVal);
	}

	//increase the count of disk on the destination peg
	diskCount[toPeg]++;

	//decrease the count of disk on the source peg
	diskCount[fromPeg]--;
}

//function to calculate destination of the disk
struct diskDef calcDest(int dn, int toPeg)
{
	int a, b;
	struct diskDef temp;
	switch (toPeg)
	{
	case 1: a = 20;
		b = 200;
		break;
	case 2: a = 230;
		b = 410;
		break;
	case 3: a = 440;
		b = 620;
		break;
	}
	temp.left = a + (10 * (totalNumDisks - dn + 1));
	temp.right = b - (10 * (totalNumDisks - dn + 1));
	temp.top = 325 - (diskCount[toPeg] * 25);
	temp.bottom = 345 - (diskCount[toPeg] * 25);
	return temp;
}

//function to show mouse pointer on the screen
void showMouse()
{
	in.x.ax = 1;
	int86(51, &in, &out);
}

//function to get the coordinates of the left click
struct coor getCoordinates()
{
	struct coor temp;
	in.x.ax = 3;
	while (1)
	{
		int86(51, &in, &out);
		if (out.x.bx == 1)
		{
			temp.x = out.x.cx;
			temp.y = out.x.dx;
			delay(50);
			return temp;
		}
	}
}

//function to identify which peg is clicked
int identifyPeg()
{
	int i;
	struct coor point = getCoordinates();
	//identify which peg or return 0 for none 
	for (i = 1; i < 4; i++)
	{
		if (point.x>peg[i].left && point.x<peg[i].right && point.y>peg[i].top && point.y<peg[i].bottom)
		{
			return i;
		}
	}
	return 0;
}

//function to identify which disk is clicked
int identifyDisk()
{
	int i;
	struct coor point = getCoordinates();
	//identify which disk is clicked
	for (i = 1; i <= totalNumDisks; i++)
	{
		if (point.x>disk[i].left && point.x<disk[i].right && point.y>disk[i].top && point.y<disk[i].bottom)
		{
			return i;
		}
	}
	return 0;
}

//function for getting the peg
int getPeg()
{
	int peg;
	while (1)
	{
		peg = identifyPeg();
		if (peg != 0)
		{
			return peg;
		}
	}
}

//function for getting disk
int getDisk()
{
	int dsk;
	while (1)
	{
		dsk = identifyDisk();
		if (dsk != 0)
		{
			return dsk;
		}
	}
}

//identify the peg of the disk
int pegOfDisk(int md)
{
	int i;
	for (i = 1; i < 4; i++)
	{
		if (disk[md].left > peg[i].left && disk[md].right < peg[i].right)
		{
			return i;
		}
	}
	return 0;
}

//function to find the top most disk on the peg
int topDisk(int p)
{
	int x, y, i;
	//calculating coordinates of top disk of the peg
	x = (peg[p].left + peg[p].right) / 2;
	y = 350 - (25 * diskCount[p]) + 10;
	//identify disk
	for (i = 1; i <= totalNumDisks; i++)
	{
		if (x>disk[i].left && x<disk[i].right && y>disk[i].top && y<disk[i].bottom)
		{
			return i;
		}
	}
	return totalNumDisks + 1;
}


//manual tower of hanoi implementation
void manualTowers()
{
	int movingDisk, toDisk, toPeg, fromPeg, i, exit;
	struct coor exitButton;
	while (1)
	{
		exit = 0;
		setfillstyle(SOLID_FILL, 11);
		bar(20, 30, 70, 50);
		outtextxy(25, 40, "EXIT");
		exitButton = getCoordinates();
		if (exitButton.x > 20 && exitButton.x < 70 && exitButton.y>30 && exitButton.y < 50)
		{
			exit = 1;
			break;
		}
		if (diskCount[2] >= totalNumDisks || diskCount[3] >= totalNumDisks)
		{
			break;
		}
		movingDisk = getDisk();
		toPeg=getPeg();
		fromPeg = pegOfDisk(movingDisk);
		toDisk = topDisk(toPeg);
		if (toDisk > movingDisk)
			moveDisk(movingDisk, fromPeg, toPeg);
		else
			outtextxy(250, 50, "WRONG MOVE");
	}
	if (exit == 0)
	{
		outtextxy(250, 100, "DISKS MOVED SUCCESSFULLY");
		getch();
	}
}

//automatic tower of hanoi implementation
void autoTowers(int num, int fp, int tp, int ap)
{
	if (num == 1)
	{
		moveDisk(1, fp, tp);
		return;
	}
	autoTowers(num - 1, fp, ap, tp);
	moveDisk(num, fp, tp);
	autoTowers(num - 1, ap, tp, fp);
}

void main()
{
	int g = DETECT, m;
	initgraph(&g, &m, "C:\\TC\\bgi");
	outtextxy(250, 100, "TOWER OF HANOI");
	outtextxy(200, 150, "MADE BY : RISHABH KUMAR VARSHNEY");
	getch();
	showMouse();
	while (!kbhit())
	{
		selectionMenu();
	}
	getch();
}
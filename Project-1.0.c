//ME101 Project-1.0
//Daniel Salter
void getText(string fileName);
void goHome();
void buttonPress(bool &flag);
void populateXY();
void letterA();
void letterB();
void letterC();
void letterD();
void letterE();
void letterF();
void letterG();
void letterH();
void letterI();
void letterJ();
void letterK();
void letterL();
void letterM();
void letterN();
void letterO();
void letterP();
void letterQ();
void letterR();
void letterS();
void letterT();
void letterU();
void letterV();
void letterW();
void letterX();
void letterY();
void letterZ();
void letter0();
void letter1();
void letter2();
void letter3();
void letter4();
void letter5();
void letter6();
void letter7();
void letter8();
void letter9();
void letterDivide();
void letterAsterix();
void letterPlus();
void letterMinus();
void letterEquals();
void letterPeriod();
void letterComma();
void vt(int multiple);
void ht(int multiple);
void diag(int multiple1, int multiple2);
void armUp();
void armDown();
void goTo(int index);
void writeLetter(int i);
void calibrate(bool &flag);
void erase(bool &flag);
//declare functions for later usage.

char characters[56];
int x[56];
int y[56];
/*vectors would have been used had they been available in robotc
arrays are global since they will be used by many functions
motorA handles y(vertical) movement, motorC handles x(horizontal)
movement and motorB handles the writing arm*/

task main()
{
	SensorType[S1] = sensorTouch;
	SensorType[S2] = sensorTouch;
	//for the purposes of this project only two sensors needed
	string fileName = "toWrite.txt";
	bool flag = true;
	//variable for string for purposes of passing to function

	getText(fileName);
	populateXY();
	calibrate(flag);
	//run all preliminary functions to prepare for main writing loop

	for (int i = 0; i < 56 && flag; i++)
	{//write until max array length
		if (!(characters[i]))
			flag = false;//should end function if character[i] is empty
		else if (characters[i] == ' ')
			;//do nothing, go to next index in array
		else
		{//goes to appropriate location on board and writes letter
		 	goTo(i);
			writeLetter(i);
			goHome();
			buttonPress(flag);
		}//after letter, goes to origin and waits for potential buttonpress
	}//check for button press

	playSound(soundFastUpwardTones);
	sleep(1000);
//play sound when finished
}

void getText(string fileName)
{
	TFileHandle myFileHandle;//keeps track of file
	TFileIOResult IOResult;//nonzero if no errors
	short int fileSize = 56;

	char incomingChar = ' ';

	OpenRead(myFileHandle, IOResult, fileName, fileSize);
//opens the file for reading only
	for (int i = 0; i < fileSize; i++)
	{
		ReadByte(myFileHandle, IOResult, incomingChar);
//reads a single character from file, proceeds sequentially
		characters[i] = incomingChar;
	}
	Close(myFileHandle, IOResult);
}//adapted from http://www.robotc.net/wikiarchive/NXT_Functions_File_Access

void buttonPress(bool &flag)
{//after every letter is written will wait to check for button press.
	wait1Msec(750);
	int n = nNxtButtonPressed;

	if (n == 3)
	{
		displayString(6, "Press button",);
		displayString(7, "3 to resume",);
		while(nNxtButtonPressed == 3){}
		while(nNxtButtonPressed != 3){}
		while(nNxtButtonPressed == 3){}
		eraseDisplay();
	}//pause program

	else if (n == 1)
	{
		displayString(3, "Erasing Display",);
		erase(flag);
		displayString(3, "Done!",);
		wait1Msec(500);
		eraseDisplay();
	}//erase board

	else
	{
		;
	}//do nothing

}

void goHome()
{//go to origin.
	armUp();
	motor[motorA] = (85);
	//equation to determine motor speed ensures robot
	//moves at same speed in x and y
	while(SensorValue[S1] != 1){}
//moves vertically first until sensor touches
	motor[motorA] = 0;

	motor[motorC] = 30;
	while(SensorValue[S2] != 1){}
//moves horizontally until second sensor touches
	motor[motorC] = 0;
}

void populateXY()
{/*populate coordinate list with numbers
	purpose is to keep track of where the robot
	will be for each character.*/
	int index = 0;
	for (int i = 1; i <= 7; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			x[index] = -(j * 1.67) * (360 / 5.0625);
			y[index] = -(15 - (i * 2)) * (360 / 1.0625);
			index++;
		}
	}

}

void vt(int multiple)
{//vertical movement, motorA.
	int posNeg = -1;
	nMotorEncoder[motorA] = 0;
	//reset motor encoder so that any movement is measured relative
	//to initial position
	if (multiple < 0)
	{
		posNeg = 1;
	}

	motor[motorA] = (posNeg * 75);
	while(abs(nMotorEncoder[motorA]) < abs(85 * multiple)){}
//85 is approximately the angle of rotation that moves the arm 0.25"
	motor[motorA] = 0;
	wait1Msec(250);
}

void ht(int multiple)
{//horizontal movement, motorC.
	int posNeg = -1;
	nMotorEncoder[motorC] = 0;
	//reset motor encoder so that any movement is measured relative
	//to initial position
	if (multiple < 0)
	{
		posNeg = 1;
	}

	motor[motorC] = (15 * posNeg);
	while(abs(nMotorEncoder[motorC]) < abs(18 * multiple)){}
//18 is approximately the angle of rotation that moves the arm 0.25"
	motor[motorC] = 0;
	wait1Msec(250);
}

void diag(int multiple1, int multiple2)
{//y then x.
	int posNeg1 = -1;
	int posNeg2 = -1;
	nMotorEncoder[motorA] = 0;
	nMotorEncoder[motorC] = 0;

	if (multiple1 < 0)
	{
		posNeg1 = 1;
	}
	if (multiple2 < 0)
	{
		posNeg2 = 1;
	}//default motor dir is neg but changes to pos if multiple is neg
	//this means the motor needs to go backwards if multiple is neg

	//attempting to make motor speeds equal each other roughly
	//via guess and check
	motor[motorA] = (posNeg2 * 75);
	motor[motorC] = (posNeg1 * 15);
	while(abs(nMotorEncoder[motorA]) < abs(85 * multiple1) ||
		abs(nMotorEncoder[motorC]) < abs(18 * multiple2)){}
	motor[motorA] = 0;
	motor[motorC] = 0;
	wait1Msec(500);
//runs until at least 1 encoder reaches appropriate distance
//other encoder should approximately be at the correct dist too
}

void armUp()
{//raise writing arm.
	nMotorEncoder[motorB] = 0;
	motor[motorB] = -15;
	while(nMotorEncoder[motorB] > -90){}
	motor[motorB] = 0;
}

void armDown()
{//lower motor arm.
	nMotorEncoder[motorB] = 0;
	motor[motorB] = 15;
	while(nMotorEncoder[motorB] < 90){}
	motor[motorB] = 0;
}

void goTo(int index)
{/*go to position in x and y arrays at index
	items in arrays are coordinates (x,y) in
	degrees, motors run until they reach the index.*/
	nMotorEncoder[motorA] = 0;
	nMotorEncoder[motorC] = 0;

	motor[motorA] = -(85);
	while(nMotorEncoder[motorA] >= y[index]){}
	motor[motorA] = 0;

	motor[motorC] = -(20);
	while(nMotorEncoder[motorC] >= x[index]){}
	motor[motorC] = 0;

}

void calibrate(bool &flag)
{//allow marker or eraser to be attached
	//then run program.
	goHome();
	displayString(3, "Attach tool and",);
	displayString(4, "press button",);
	displayString(5, "to continue",);
	displayString(6, "1 to erase",);
	displayString(7, "3 to continue",);
	while(nNxtButtonPressed == -1){}
	if (nNxtButtonPressed == 1)
	{
		while(nNxtButtonPressed == 1){}
		eraseDisplay();
		erase(flag);
	}//if button 1 is pressed, erase display
	else if (nNxtButtonPressed == 3)
		while(nNxtButtonPressed == 3){}
//if button 3 is pressed, run program
	eraseDisplay();
}

void erase(bool &flag)
{//allow eraser to be attached and then wipes
	//entire board
	nMotorEncoder[motorA] = 0;
	nMotorEncoder[motorC] = 0;
	displayString(3, "Attach eraser and",);
	displayString(4, "press button 3",);
	displayString(4, "to continue",);
	while(nNxtButtonPressed != 3){}
	while(nNxtButtonPressed == 3){}

	eraseDisplay();
	displayString(0, "Erasing Board",);
	armDown();

	for (int i = 0; nMotorEncoder[motorA] >= -(85 * 60); i++)
	{//loops while robot is <= 15" above origin in y axis
		motor[motorA] = -(85);
		while(nMotorEncoder[motorA] >= -(85 * 3 * i)){}
		motor[motorA] = 0;//move the robot vertically by 1.5"

		motor[motorC] = -(25);
		while(nMotorEncoder[motorC] >= -(18 * 53)){}
		motor[motorC] = (25);
		while(nMotorEncoder[motorC] <= 15){}
		motor[motorC] = 0;//move robot horizontally across the board & back
	}

	eraseDisplay();
	displayString(0, "Done!",);
	wait1Msec(1000);
	eraseDisplay();

	goHome();
	flag = false;

}

void writeLetter(int i)
{/*the following statements are horrifying but necessary
	arranged in order of most frequently  used characters
	english language https://en.oxforddictionaries.com
	/explore/which-letters-are-used-most*/
	if (characters[i] == 'E')
		letterE();
	else if (characters[i] == 'T')
		letterT();
	else if (characters[i] == 'A')
		letterA();
	else if (characters[i] == 'I')
		letterI();
	else if (characters[i] == 'N')
		letterN();
	else if (characters[i] == 'O')
		letterO();
	else if (characters[i] == 'S')
		letterS();
	else if (characters[i] == 'H')
		letterH();
	else if (characters[i] == 'R')
		letterR();
	else if (characters[i] == 'D')
		letterD();
	else if (characters[i] == 'L')
		letterL();
	else if (characters[i] == 'U')
		letterU();
	else if (characters[i] == 'C')
		letterC();
	else if (characters[i] == 'M')
		letterM();
	else if (characters[i] == 'F')
		letterF();
	else if (characters[i] == 'W')
		letterW();
	else if (characters[i] == 'Y')
		letterY();
	else if (characters[i] == 'G')
		letterG();
	else if (characters[i] == 'P')
		letterP();
	else if (characters[i] == 'B')
		letterB();
	else if (characters[i] == 'V')
		letterV();
	else if (characters[i] == 'K')
		letterK();
	else if (characters[i] == 'Q')
		letterQ();
	else if (characters[i] == 'J')
		letterJ();
	else if (characters[i] == 'X')
		letterX();
	else if (characters[i] == 'Z')
		letterZ();
	else if (characters[i] == '0')
		letter0();
	else if (characters[i] == '1')
		letter1();
	else if (characters[i] == '2')
		letter2();
	else if (characters[i] == '3')
		letter3();
	else if (characters[i] == '4')
		letter4();
	else if (characters[i] == '5')
		letter5();
	else if (characters[i] == '6')
		letter6();
	else if (characters[i] == '7')
		letter7();
	else if (characters[i] == '8')
		letter8();
	else if (characters[i] == '9')
		letter9();
	else if (characters[i] == '*')
		letterAsterix();
	else if (characters[i] == '.')
		letterPeriod();
	else if (characters[i] == '+')
		letterPlus();
	else if (characters[i] == '-')
		letterMinus();
	else if (characters[i] == '/')
		letterDivide();
	else if (characters[i] == '=')
		letterEquals();
	else if (characters[i] == ',')
		letterComma();
	else
		;//do nothing
}

/*All characters done based off of http://static3.bigstockphoto.
com/thumbs/7/3/9/large1500/93768752.jpg and translated to
distances (to be detailed in report) and each letter utilizes
horizontal, diagonal, and vertical movement functions
by providing a multiple of 0.25" desired*/
void letterN()
{
    vt(1);
    ht(1);
    armDown();
    vt(6);
    armUp();
    vt(-1);
    armDown();
    diag(4,-4);
    vt(7);
}

void letterO()
{
    vt(1);
    ht(3);
    armDown();
    ht(1);
    diag(1,1);
    vt(3);
    diag(-1,1);
    ht(-1);
    diag(-1,-1);
    vt(-3);
    diag(1,-1);
}

void letterPeriod()
{
    vt(1);
    ht(3);
    armDown();
}

void letterPlus()
{
    vt(4);
    ht(1);
    armDown();
    ht(4);
    armUp();
    vt(1);
    ht(-2);
    armDown();
    vt(-4);
}

void letterP()
{
    vt(1);
    ht(1);
    armDown();
    vt(6);
    ht(3);
    diag(1,-1);
    diag(-1,-1);
    ht(-3);
}

void letterQ()
{
    vt(1);
    ht(5);
    armDown();
    ht(-3);
    diag(-1,1);
    vt(3);
    diag(1,1);
    ht(2);
    diag(1,-1);
    vt(-4);
    diag(-2,2);
}

void letterR()
{
    vt(1);
    ht(1);
    armDown();
    vt(6);
    ht(3);
    diag(1,-1);
    diag(-1,-1);
    ht(-3);
    armUp();
    ht(3);
    armDown();
    diag(1,-1);
}

void letterS()
{
    ht(1);
    armDown();
    ht(3);
    diag(1,1);
    diag(-1,1);
    ht(-2);
    diag(-1,1);
    diag(1,1);
    ht(3);
}

void letterT()
{
    ht(3);
    armDown();
    vt(6);
    armUp();
    ht(2);
    armDown();
    ht(-4);
}

void letterU()
{
    vt(6);
    ht(1);
    armDown();
    vt(-4);
    diag(1,-1);
    ht(2);
    diag(1,1);
    vt(4);
}

void letterV()
{
    vt(6);
    ht(1);
    armDown();
    vt(-4);
    diag(2,-2);
    diag(2,2);
    vt(4);
}

void letterW()
{
    vt(7);
    ht(1);
    armDown();
    vt(-6);
    diag(2,2);
    diag(2,-2);
    vt(6);
}

void letterX()
{
    ht(2);
    armDown();
    diag(3,4);
    armUp();
    ht(-4);
    armDown();
    diag(3,-4);
}

void letterY()
{
    vt(6);
    ht(2);
    armDown();
    diag(2,-2);
    vt(-2);
    armUp();
    vt(3);
    armDown();
    diag(2,2);
}

void letterZ()
{
    vt(7);
    ht(2);
    armDown();
    ht(3);
    diag(-4,-4);
    ht(3);
}

void letterComma()
{
	vt(1);
	ht(2);
	armDown();
	diag(1,1);
}

void letterDivide()
{
	ht(2);
	armDown();
	diag(5,5);
}

void letterD()
{
	vt(2);
	ht(2);
	armDown();
	ht(3);
	diag(1,1);
	vt(3);
	diag(-1,1);
	ht(-3);
	vt(-7);
}

void letterE()
{
	vt(1);
	ht(7);
	armDown();
	ht(-4);
	vt(6);
	ht(4);
	armUp();
	vt(-3);
	ht(-1);
	armDown();
	ht(-3);
}

void letterEquals()
{
	vt(3);
	ht(2);
	armDown();
	ht(2);
	armUp();
	vt(1);
	armDown();
	ht(-2);
}

void letterF()
{
	vt(1);
	ht(3);
	armDown();
	vt(6);
	ht(4);
	armUp();
	vt(-3);
	ht(-1);
	armDown();
	ht(-3);
}

void letterG()
{
	vt(3);
	ht(4);
	armDown();
	vt(1);
	ht(2);
	vt(-2);
	diag(-1,-1);
	ht(-2);
	diag(-1,1);
	vt(4);
	diag(1,1);
	ht(2);
	diag(1,-1);
}

void letterH()
{
	vt(1);
	ht(3);
	armDown();
	vt(6);
	armUp();
	ht(4);
	armDown();
	vt(-6);
	armUp();
	vt(3);
	armDown();
	ht(-4);
}

void letterI()
{
	vt(1);
	ht(4);
	armDown();
	vt(6);
}

void letterJ()
{
	vt(2);
	armDown();
	diag(1,-1);
	ht(2);
	diag(1,1);
	vt(5);
}

void letterK()
{
	ht(1);
	armDown();
	vt(6);
	armUp();
	ht(4);
	armDown();
	vt(-2);
	diag(-1,-1);
	ht(-3);
	armUp();
	ht(3);
	armDown();
	diag(1,-1);
}

void letterL()
{
	vt(7);
	ht(2);
	armDown();
	vt(-6);
	ht(3);
}

void letterMinus()
{
	vt(3);
	ht(2);
	armDown();
	ht(2);
}

void letterM()
{
	vt(1);
	ht(1);
	armDown();
	vt(6);
	diag(2,-2);
	diag(2,2);
	vt(-6);
}

void letter0()
{
  ht(2);
  armDown();
  ht(1);
  diag(1,1);
  vt(4);
  diag(-1,1);
  ht(-1);
  diag(-1,-1);
  vt(-4);
  diag(1,-1);
}

void letter1()
{
	ht(1);
	armDown();
	ht(4);
	armUp();
	ht(-2);
	armDown();
	vt(6);
	diag(-2,-2);
}

void letter2()
{
	ht(5);
	armDown();
	ht(-4);
	diag(2,2);
	ht(1);
	diag(1,1);
	diag(-1,1);
	ht(-2);
	diag(-1,-1);
}

void letter3()
{
	vt(3);
	ht(1);
	armDown();
	diag(1,-1);
	ht(2);
	diag(1,1);
	diag(-1,1);
	ht(-1);
	armUp();
	ht(1);
	armDown();
	diag(1,1);
	vt(1);
	diag(-1,1);
	ht(-2);
	diag(-1,-1);
}

void letter4()
{
	vt(2);
	ht(4);
	armDown();
	vt(6);
	diag(-3,-3);
	ht(4);
}

void letter5()
{
	vt(3);
	ht(1);
	armDown();
	diag(1,-1);
	ht(2);
	diag(1,1);
	vt(1);
	diag(-1,1);
	ht(-3);
	vt(2);
	ht(4);
}

void letter6()
{
	vt(7);
	ht(5);
	armDown();
	diag(-1,1);
	ht(-2);
	diag(-1,-1);
	vt(-3);
	diag(1,-1);
	ht(2);
	diag(1,1);
	diag(-1,1);
	ht(-3);
}

void letter7()
{
	vt(1);
	ht(1);
	armDown();
	vt(2);
	diag(2,2);
	vt(1);
	ht(-4);
}

void letter8()
{
	vt(3);
	ht(1);
	armDown();
	diag(1,-1);
	ht(3);
	diag(1,1);
	diag(-1,1);
	ht(-3);
	armUp();
	ht(3);
	armDown();
	diag(1,1);
	diag(-1,1);
	ht(-3);
	diag(-1,-1);
	diag(1,-1);
	diag(-1,-1);
}

void letter9()
{
	vt(1);
	ht(1);
	armDown();
	diag(1,-1);
	ht(2);
	diag(1,1);
	vt(4);
	diag(-1,1);
	ht(-2);
	diag(-1,-1);
	diag(1,-1);
	ht(3);
}

void letterA()
{
	diag(1,1);
	armDown();
	vt(5);
	diag(1,1);
	ht(2);
	diag(1,-1);
	vt(-5);
	armUp();
	vt(2);
	armDown();
	ht(-4);
}

void letterAsterix()
{
	vt(2);
	ht(1);
	armDown();
	diag(4,4);
	armUp();
	ht(-2);
	armDown();
	vt(-4);
	armUp();
	ht(2);
	armDown();
	diag(-4,4);
	armUp();
	vt(-2);
	armDown();
	ht(4);
}

void letterB()
{
	ht(2);
	vt(1);
	armDown();
	ht(3);
	diag(1,1);
	diag(-1,1);
	ht(-3);
	armUp();
	ht(3);
	armDown();
	diag(1,1);
	diag(-1,1);
	ht(-3);
	vt(-6);
}

void letterC()
{
	vt(3);
	ht(6);
	armDown();
	diag(-1,-1);
	ht(-2);
	diag(-1,1);
	vt(3);
	diag(1,1);
	ht(2);
	diag(1,-1);
}

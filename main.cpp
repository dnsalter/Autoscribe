//Daniel Salter
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

string getFile();

void getText(string name, vector<char> &characters);
void cleanList(vector<char> &characters);
void toUpper(vector<char> &characters);
void replaceInvalid(vector<char> &characters);
void fullSentence(vector<char> &characters);
void writeText(vector<char> &characters);
//define all functions so that main will be first
//helpful so that logical flow of code is seen before functions

int main()
{//run all functions and define characters array. 
    vector<char> characters;
    getText(getFile(), characters);
    cleanList(characters);
    writeText(characters);

    cout << "Clean text-file created" << endl;

	return 0;
}

void cleanList(vector<char> &characters)
{//runs collection of list cleaning functions. 
    toUpper(characters);
    replaceInvalid(characters);
    fullSentence(characters);
}

void toUpper(vector<char> &characters)
{//takes list and replaces lowercase letters with uppercase. 
    for (int i(0); i < characters.size(); i++)
    {
        characters[i] = (char)toupper((int)characters[i]);
    }/*casts character as int, uses built in function to convert
       ASCII to uppercase then casts it back to character*/
}

void replaceInvalid(vector<char> &characters)
{//takes list and replaces invalid characters with placeholder.
    for (int i(0); i < characters.size(); i++)
    {
        if (((int)characters[i] >= 65 && (int)characters[i] <= 90)
            || ((int)characters[i] >= 42 && (int)characters[i] <= 57)
            || (int)characters[i] == 32)//checks if within acceptable ASCII range
        {
            ;
        }//does nothing, easier to check if a character is within a good range than if it's not

        else
        {
            characters[i] = ' ';
        }//replaces character with arbitrary placeholder if not in good range of ASCII values
    }
}

void fullSentence(vector<char> &characters)
{/*finishes cleaning up array by ensuring array consists of full sentences when possible, full words
    when not, and characters when none of the above is possible.*/
    bool flag(false);
    int index(0), vecSize(characters.size());

    for (int i(characters.size()-1); i >= 0 && flag == false; i--)
    {//loop from end of array as opposed to start
        if ((int)characters[i] == 46)
        {
            flag = true;
            index = i+1;
        }
    }//checks if period in unclean array and sets index equal to position

    if (not(flag))
    {
        for (int i(characters.size()-1); i >= 0 && flag == false; i--)
        {
            if ((int)characters[i] == 32)
            {
                flag = true;
                index = i;
                cout << index;
            }
        }
    }//if no period then checks for last occurring space and sets index equal to position

    if (flag)
    {
        for (int i(index); i < vecSize; i++)
        {
            characters.erase(characters.begin() + index);
        }
    }//replaces all characters following index only if one of the above scenarios occurs
}

void writeText(vector<char> &characters)
{/*compiles text into string and writes to file. Can change to write individual characters
    if needed.*/
    string name("clean.txt"), sentence("");
    ofstream myfile;//opens for read only
    myfile.open(name.c_str());//c_str is needed to open
    if(myfile.is_open())//loops while file is open
    {
        for (int i(0); i < characters.size(); i++)
        {
            sentence += characters[i];
        }//compiles clean array into string

        myfile << sentence;//writes sentence to file
        myfile.close();
    }//file closes, loop ends
}

string getFile()
{//returns name of text-file for later use. 
    string textFile("");

    cout << "Enter the name of the text file you wish to print: ";
    cin >> textFile;

    textFile += ".txt";
//concatenates .txt with name of text-file
    return textFile;
}

void getText(string name, vector<char> &characters)
{//reads textfile and appends characters to characters vector. 
    string line(" ");
    int i(0);
    char ch(' ');//
    ifstream myfile;//opens for read only
    myfile.open(name.c_str());//c_str is needed to open
    if(myfile.is_open())//loops while file is open
    {
        while(getline(myfile, line))
        {//retrieves text line-by-line
            for (; i < line.size(); i++)
            {//parses string and appends characters (as string)
                ch = line[i];
                if (i < 56)
                {
                    characters.push_back(ch);
                }
            }
        }
        myfile.close();
    }//file closes, loop ends
}

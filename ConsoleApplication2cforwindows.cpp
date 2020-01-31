// ConsoleApplication2cforwindows.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h>

/* use of defines - can make things easier to read as it hides elements that are already well proven and understood */
#define PRINT_REPLACE(eric) replace2(pattern, sizeof pattern, eric, sizeof eric)
#define REPLACE_CHAR '#'
char pattern[] = {"this is the ##.## pattern"};

/***************************************************************************************************************************************/
char lpattern[sizeof pattern];/* this is declared at this level for work done by replace - it has a number of disadvantages doing this */

/* easist to use but dangerous in some ways and requires shared data which is difficlut to manage */
char* dangerous_replace(const char* p1, const char* p3) /* we make these const as we do not want to alter the data that is pointed too */
{
    int i;/* delare this here but it can be done in the for loop if we are allowing c++ */
    for (i = 0; i < sizeof pattern; i++,p1++)
    {
		/*if (*p1 != '#')  This is what the next line does - generally the one line is harder to debug but easier to read 
		{
			lpattern[i] = *p1;
		}
		else
		{
			lpattern[i] = *p3;
			p3++;
		}*/

        lpattern[i] = (*p1 != '#') ? *p1 : *p3++;
    }
    return lpattern;/* we can only do this as we have used a globally (in scope) defined memory space allocated to lpattern - its on the heap*/
}


/* Simplest to read string replacement printf inside the call */
/* this could be one way to do it if the actual printing is an end product - rather than the building of a string
	However the printing is now done inline and hence there is a high overhead in running the function which may be a problem in a real time system
	In the real time system the printing may be done in a different task to avoid the system holding up the execution of the main operation just for prints
	*/
void dangerous_replace1(const char* p1, const char* p3)
{
	int i;
	for (i = 0; i < sizeof pattern; i++, p1++)
		lpattern[i] = (*p1 != '#') ? *p1 : *p3++;

	printf(lpattern); /* this is of course c and the program pauses here while the data is actually printed out to the console */
}

/* WHY ARE THEY DANGEROUS  - well the passed in string p3 must fill the number of # in the pattern - if it is smaller then access is made to other peoples data */
/* again the printf is in the body - this time we do not use the global areas which makes the system thread safe */
/* also here we make use of parameter checking in case the program is crashed by sending in partial data
	and the partial use of the input space is accounted for */
void replace2(const char* p1, int s1, const char* p3, int s2)
{

	int i = 0;
	s2--;
	if (p1 != NULL && p3 != NULL && (s1 > 1)) /* check that the input data is valid */
	{
		char* pForString = (char*)malloc(s1);
		char* pForStringWalker= pForString;

		if (pForString != NULL)
		{
			*pForStringWalker = *p1;
			for (; i < s1; i++, pForStringWalker++, p1++)
			{
				if ((s2 == 0) || (*p1 != REPLACE_CHAR)) /* check if we have run out of source characters*/
				{
					*pForStringWalker = *p1;
				}
				else
				{
					s2--;
					*pForStringWalker = *p3;
					p3++;/* only point to the next entry when one has been used */
				}
			}
			printf(pForString); /* this is of course c and the program pauses here while the data is actually printed out to the console */
			free(pForString);
		}
	}
}


int main()
{
	std::cout << "Hello World!\n";	//comfort message that the program is working 
	std::cout << dangerous_replace(pattern, "1003"); // using dangerous method 1 
	std::cout << "\n"; // just a new line 
	(void)dangerous_replace1(pattern, "1004"); // using dangerous method 2 
	std::cout << "\n"; // just a new line 
	replace2(pattern, sizeof pattern, "1104", sizeof "1104");
	std::cout << "\n"; // just a new line 
	replace2(pattern, sizeof pattern, "1124", sizeof "1124");
	std::cout << "\n"; // just a new line 
	PRINT_REPLACE("1194");
	std::cout << "\n"; // just a new line 
	PRINT_REPLACE("11");
	std::cout << "\n"; // just a new line 
	PRINT_REPLACE("112345678"); /* <<<<< Krazy use of the function still does not break it*/
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

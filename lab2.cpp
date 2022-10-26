#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <stack>
#include <ctype.h>

using namespace std;

const string Keyword[32] =
{
	"auto", "break", "case","char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};

struct CountGroup
{
	int count = 0;
	int count_switch_case = 0;
	int count_if_else = 0;
	int count_if_elseif_else = 0;
	int count_case[100] = {0};
	int case_number = 0;
};

int main()
{
	void TotalOutput( CountGroup &f,int level );
	void Match( CountGroup &f, string file_word, queue<string> &s ,fstream &fin );
	void KeywordMatch( CountGroup &f, queue<string> &s );
	void ReadFile( string & file_name );
	int level;
	string file_name ;
	cout<< "Please enter the location of the file: ";
	cin >> file_name;
	cout << "Please enter the level you want to achieve: ";     //Users enter the location of the file and the result level they want to achieve;
	cin >> level;
	ReadFile( file_name );
	string file_word = " ";
	fstream fin;
	fin.open(file_name.c_str(), ios::in );
	CountGroup f;
	queue<string> keyword_queue;
	Match(f, file_word, keyword_queue ,fin);
	KeywordMatch(f,keyword_queue);
	TotalOutput(f,level);
	fin.close();
}

//Open the file and Deal with the text in the file;
//File address specification function and file reading function.
void ReadFile( string & file_name)
{
	int s = file_name.length();
	stack <char> temp;
	while(s--)
	{
		temp.push(file_name[s]);
	}
	file_name = "";
	while(!temp.empty())
	{
		file_name+=temp.top();
		if(temp.top() == '\\')
			file_name += '\\';
		temp.pop();
	}
}

// judge whether the keywords are in the file;
void KeywordMatch(CountGroup &f,queue<string> & keyword_queue)
{
	int i = 0,j = 0;
	stack <string> temp;
	while(!keyword_queue.empty())
	{
		if(keyword_queue.front() == "if")
		{
			temp.push(keyword_queue.front());
			keyword_queue.pop();
		}
		else if(keyword_queue.front() == "elseif")
		{
			temp.push(keyword_queue.front());
			keyword_queue.pop();
		}
		else if(keyword_queue.front() == "else")
		{
			if(temp.top() == "if")
			{
				i++;
				f.count_if_else++;
				temp.pop();
			}
			else
			{
				
				while(temp.top() == "elseif" && !temp.empty())
				{
					temp.pop();
				}
				if(!temp.empty())
				{
					j++;
					f.count_if_elseif_else++;
					temp.pop();
				}
			}
			keyword_queue.pop();
			
		}
		
		else if(keyword_queue.front() == "switch")
		{
			keyword_queue.pop();
			
			while((keyword_queue.front() == "case" || keyword_queue.front() == "break" || keyword_queue.front() == "default") && !keyword_queue.empty())
			{
				if(keyword_queue.front() == "case")
				{
					f.count_case[f.case_number]++;
				}
				keyword_queue.pop();
			}
			f.case_number++;
		}
		else
		{
			keyword_queue.pop();
		}
	}
}

//setup the output of this project.
void TotalOutput(CountGroup &f,int level)
{
	void cout2(CountGroup &f);
	void cout3(CountGroup &f);
	void cout4(CountGroup &f);
	
	cout<<"total num: "<<f.count<<endl;
	if(level == 2)
	{
		cout2(f);
	}
	else if(level == 3)
	{
		cout2(f);
		cout3(f);
	}
	else
	{
		cout2(f);
		cout3(f);
		cout4(f);
	}
}

void cout2(CountGroup &f)
{
	int i = f.case_number;
	int j = 0;
	cout << "switch num: " << f.count_switch_case << endl;
	cout << "case num: ";
	while(i--)
	{
		cout << f.count_case[j] << ' ';
		j++;
	}
	cout << endl;
}

void cout3(CountGroup &f)
{
	cout << "if-else num: " << f.count_if_else << endl;
}

void cout4(CountGroup &f)
{
	cout << "if-elseif-else num: " << f.count_if_elseif_else << endl;
}

void Match(CountGroup &f, string file_word, queue<string> &keyword_queue, fstream &fin )
{
	
	while( fin>>file_word )
	{
		if(file_word[0] == '/' && file_word[1] == '/' )
		{
			cout << "//    " << file_word << endl;
			getline(fin,file_word);
			fin>>file_word;
		}
		else if(file_word[0] == '/' && file_word[1] == '*')
		{
			while(fin>>file_word)
			{
				if(file_word[file_word.size()-2] == '*' && file_word[file_word.size()-1] == '/')
				{
					break;
				}
			}
		}
		int index = 0;
		int size_file_word = file_word.size();
		string temp_string;
		for( int i = 0; i<size_file_word-1; i++ )
		{
			if( !isalpha(file_word[i]) && isalpha(file_word[i+1]) )
				index = i+1;
			
			if( ( isalpha(file_word[i]) && !isalpha(file_word[i+1]) ) || ( i+1 == size_file_word-1 && isalpha(file_word[i+1]) ))
			{
				//Keyword extraction.
				//Cover the string into letters only.
				if( ( isalpha(file_word[i]) && !isalpha(file_word[i+1]) ) )
					temp_string = file_word.substr(index,i+1-index);
				else
					temp_string = file_word.substr(index,i+2-index);
				
				for( int j = 0; j < 32; j++ )
				{
					if( temp_string == Keyword[j] )
					{
						if( temp_string != "else")
						{
							keyword_queue.push( temp_string );
							f.count++;
							if( temp_string == "switch")
								f.count_switch_case++;
						}
						else
						{//to provent from the condition "else{if}";
							f.count++;
							if(file_word[4] == '{')
							{
								keyword_queue.push("else");
							}
							else{
								fin>>file_word;
								i++;
								if( ( file_word[0] == 'i' && file_word[1] == 'f' && file_word[2] == '(') || file_word == "if" )
								{
									keyword_queue.push("elseif");
									f.count++;
								}
							}
						}
						break;
					}
				}
				index = 0;
				break;
			}
		}
	}
}

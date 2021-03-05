#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <iterator>
#include <array>
#include <fstream>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cctype>


void freeArrayOfCharArrays(char **array, size_t array_length);
void splitString(std::string text, char d, std::vector<std::string>& result);
void vectorOfStringsToArrayOfCharArrays(std::vector<std::string>& list, char ***result);

int main (int argc, char **argv)
{
    //char *input;
    std::string input;
    char* charInput;
    char* secondInput;
    std::string secondInputString;
    std::string history = "history";
    std::string clear = " clear";
    char exiting[] = "exit";
    char dot[] = ".";
    char slash[] = "/";
    int historySize = 0;
    char fileName[] = "history.txt";
    std::string char1;
    bool foundPath;

    //Arg list decloration
    std::vector<std::string> vector_arg_list;
    char **arg_list_exec;

    int i = 0;
    struct stat sb;
    char execution[128];

    std::string currentLine;
    std::string copy;

    std::fstream file;
    file.open(fileName, std::fstream::in | std::fstream::out | std::fstream::app);
    //If the file does not exist
    if(!file){
        file.open(fileName, std::fstream::in | std::fstream::out | std::fstream::app);
    }
    std::string * historyList = new std::string[128];

    
    while(!file.eof()){
        std::string line;
        getline(file, line);

        //if first = \n dont add it
        if(!line.empty()){
            historyList[historySize] = line;
            historySize++;
        }
    }
    file.close();
    file.open(fileName, std::fstream::app);
    
    // Get list of paths to binary executables
    // `os_path_list` supports up to 16 directories in PATH, 
    //     each with a directory name length of up to 64 characters
    std::vector<std::string> os_path_list;
    
    char* os_path = getenv("PATH");
    char *arg_list = getenv("PATH");

    // Welcome message
    printf("Welcome to OSShell! Please enter your commands ('exit' to quit).\n");
    // Allocate space for input command lists
    // `command_list` supports up to 32 command line parameters, 
    //     each with a parameter string length of up to 128 characters
    
    
    int whileLoop = 1;
    int j = 0;
    int size = 0;
    int temp = 0;


    //third variable needs to be a vector
    splitString(os_path, ':', os_path_list);

    std::string string_os_path_list;
    string_os_path_list = os_path_list[0];
    char * char_os_path_list = new char[string_os_path_list.size() + 1];
    std::copy(string_os_path_list.begin(), string_os_path_list.end(), char_os_path_list);
    char_os_path_list[string_os_path_list.size()] = '\0'; // don't forget the terminating 0

    // Repeat:
    while(whileLoop == 1){
        //  Print prompt for user input: "osshell> " (no newline)
        std::cout << "osshell>";
        charInput = new char[input.length()+1];
        

        //  Get user input for next command
        std::getline(std::cin, input);
        char1 = input.substr(0,1);
        
        if(input.empty()){
            printf("input was a new line character\n");
            continue;
        }else{
            
            std::strcpy(charInput, input.c_str());
            
            //getting and initalizing the different arguments in input
            splitString(input, ' ', vector_arg_list);
            vectorOfStringsToArrayOfCharArrays(vector_arg_list, &arg_list_exec);
            std::string string_arg_list;
            string_arg_list = vector_arg_list[0];
            char * char_arg_list = new char[string_arg_list.size()+1];
            std::copy(string_arg_list.begin(), string_arg_list.end(), char_arg_list);
            char_arg_list[string_arg_list.size()] = '\0'; // don't forget the terminating 0

            file << input << std::endl;

            std::vector<std::string> input_list;

            char **input_list_exec;
            
            splitString(input, ' ', input_list);
            
            //Added input to the history list
            historyList[historySize] = input;
            
            //Reassigning charInput to be the first element in the string that was just split
            std::strcpy(charInput, input_list[0].c_str());

            //  If command is `exit` exit loop / quit program
            if(strcmp(charInput, exiting) == 0){
                size++;
                exit(-1);
            
            //  If command is `history` print previous N commands
            } else if(strcmp(input_list[0].c_str(), history.c_str()) == 0){
               // charInput only checks the first part of input
                char inputlistAt1[input.length()+1];
                int inputIndex;

                if(input_list[1].c_str() != NULL){
                    std::strcpy(inputlistAt1, input_list[1].c_str());
                    
                    if(strcmp(input_list[1].c_str(), clear.c_str())){
                        //delete file -- reset size to 0;
                        historySize = -1; 
                        file.close();
                        file.open("history.txt", std::fstream::out | std::fstream::trunc);
                    } else{
                        for(int j =0; j < input_list[1].size(); j++){
                            if(!isdigit(inputlistAt1[j]) || std::stoi(input_list[1]) <= 0){
                                std::cout << "Error: history expects an integer > 0 (or 'clear')\n";
                                break;
                            }
                        }
                    }
                } else{
                    for(int i = 0; i < historySize; i++){
                        std::cout << "  " << i << ": " << historyList[i] << "\n";
                    }
                }
            
            }else if(char1.compare("/") == 0 || (char1.compare(".") == 0)){ 
                //do this
                if(stat(string_arg_list.c_str(), &sb) != -1){
                    int pid = fork();
                    //child
                    if(pid == 0){
                        execv(string_arg_list.c_str(), &charInput);
                    } else{
                        //parent
                        int status;
                        waitpid(pid, &status, 0);

                    }
                } else{
                    std::cout << input << ": Error command not found\n";
                }

            }else{
                i = 0;
                while(os_path_list[i].c_str() != NULL){
                    copy = string_arg_list;
                    string_arg_list = os_path_list[i] + "/" + string_arg_list;
                    
                    if(stat(string_arg_list.c_str(), &sb) != -1){
                        foundPath = true;
                        //if command path found, break out of loop
                        break;
                    } else{
                        string_arg_list = copy;
                    }
                    i++;
                }
                if(foundPath){
                    int pid = fork();
                    //child
                    if(pid == 0){
                        execv(string_arg_list.c_str(), arg_list_exec);
                    } else{
                        //parent
                        int status;
                        waitpid(pid, &status, 0);

                    }
                } else{
                    std::cout << input << ": Error command not found\n";
                }
            }
            
        }
        historySize++;
    }

    // Free allocated memory
    freeArrayOfCharArrays(arg_list_exec, vector_arg_list.size()+1);

    // don't forget to free the string after finished using it
    //delete[] char_os_path_list;
    file.close();

    return 0;
}

/*
   array: list of strings to be freed
   array_length: number of strings in the list to free
*/
void freeArrayOfCharArrays(char **array, size_t array_length)
{
    int i;
    for (i = 0; i < array_length; i++)
    {
        if (array[i] != NULL)
        {
            delete[] array[i];
        }
    }
    delete[] array;
}

/*
   text: string to split
   d: character delimiter to split `text` on
   result: NULL terminated list of strings (char **) - result will be stored here
*/
void splitString(std::string text, char d, std::vector<std::string>& result)
{
    enum states { NONE, IN_WORD, IN_STRING } state = NONE;

    int i;
    std::string token;
    result.clear();
    for (i = 0; i < text.length(); i++)
    {
        char c = text[i];
        switch (state) {
            case NONE:
                if (c != d)
                {
                    if (c == '\"')
                    {
                        state = IN_STRING;
                        token = "";
                    }
                    else
                    {
                        state = IN_WORD;
                        token = c;
                    }
                }
                break;
            case IN_WORD:
                if (c == d)
                {
                    result.push_back(token);
                    state = NONE;
                }
                else
                {
                    token += c;
                }
                break;
            case IN_STRING:
                if (c == '\"')
                {
                    result.push_back(token);
                    state = NONE;
                }
                else
                {
                    token += c;
                }
                break;
        }
    }
    if (state != NONE)
    {
        result.push_back(token);
    }
}

void vectorOfStringsToArrayOfCharArrays(std::vector<std::string>& list, char ***result)
{
    int i;
    int result_length = list.size() + 1;
    *result = new char*[result_length];
    for (i = 0; i < list.size(); i++)
    {
        (*result)[i] = new char[list[i].length() + 1];
        strcpy((*result)[i], list[i].c_str());
    }
    (*result)[list.size()] = NULL;
}
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <unistd.h>

void freeArrayOfCharArrays(char **array, size_t array_length);
void splitString(std::string text, char d, std::vector<std::string>& result);
void vectorOfStringsToArrayOfCharArrays(std::vector<std::string>& list, char ***result);

int main (int argc, char **argv)
{

    char *input;
    char history[] = "history";
    char exiting[] = "exit";
    char *text;
    // Get list of paths to binary executables
    // `os_path_list` supports up to 16 directories in PATH, 
    //     each with a directory name length of up to 64 characters
    std::vector<std::string> os_path_list;
    
    

    char* os_path = getenv("PATH");

    // Example code for how to loop over NULL terminated list of strings
    /*
    int i = 0;
    while (os_path_list[i] != NULL)
    {
        printf("PATH[%2d]: %s\n", i, os_path_list[i]);
        i++;
    }
    */


   char today[] = "today";
   char yest[] = "yest";
   strcmp(today, yest);

    // Welcome message
    printf("Welcome to OSShell! Please enter your commands ('exit' to quit).\n");
    // Allocate space for input command lists
    // `command_list` supports up to 32 command line parameters, 
    //     each with a parameter string length of up to 128 characters
    
    std::vector<std::string> command_list;
    int whileLoop = 1;
    int j = 0;
    int size = 0;
    int temp = 0;

    std::string example_command = "ls -lh";
    char **command_list_exec; 

    vectorOfStringsToArrayOfCharArrays(command_list, &command_list_exec);
    splitString(example_command, ' ', command_list);
    
    //third variable needs to be a vector
    splitString(os_path, ':', os_path_list);

    // Repeat:
    while(whileLoop == 1){
        //  Print prompt for user input: "osshell> " (no newline)
        std::cout << "osshell>";

        //  Get user input for next command
        std::cin >> input;
        //Add command good or bad into comand_list
        printf("%i\n", size);
        //command_list[size] = input;
        std:: cout << command_list[0] << "\n";
        std:: cout << command_list[1] << "\n";
        //std:: cout << command_list[2] << "\n";

        //  If command is `exit` exit loop / quit program
        printf("before exit strcmp\n");
        if(strcmp(input, exiting) == 0){
            //printf("exit = break\n");
            size++;
            exit(-1);
        }
        //printf("PATH : %s\n", getenv("PATH"));
        //  If command is `history` print previous N commands
        if(strcmp(input, history) == 0){
            printf("inside history if\n");
            int k;
            for(k = 0; k <= size; k++){
                std::cout << k << ": " << command_list[k] << std::endl;
            }
        }

        printf("after the if checks\n");
        //  For all other commands, check if an executable by that name is in one of the PATH directories
        //   If yes, execute it
        
        //   If no, print error statement: "<command_name>: Error command not found" (do include newline)
        //TODO
        if(execv(os_path_list, command_list) == -1){
            //-1 means error was found
            std::cout << command_list[0] << ": Error command not found";            
            break;
        }
        //  If input is a legal command
        

        size++;
    }

    // Free allocated memory
    //freeArrayOfCharArrays(os_path_list, 16);
    //freeArrayOfCharArrays(command_list, 32);

    freeArrayOfCharArrays(command_list_exec, command_list.size() +1);

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




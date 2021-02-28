#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <iterator>
#include <array>

void freeArrayOfCharArrays(char **array, size_t array_length);
void splitString(std::string text, char d, std::vector<std::string>& result);
void vectorOfStringsToArrayOfCharArrays(std::vector<std::string>& list, char ***result);

int main (int argc, char **argv)
{
    char *input;
    char history[] = "history";
    char exiting[] = "exit";
    char *text;
    int history_list_size = 0;

    //char cmdHistory[127][127];    //Probs have to use malloc to allocate memory 

    //std::array<std::string, 128> history_list;
    
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

    std::string string_os_path_list;
    string_os_path_list = os_path_list[0];
    char * char_os_path_list = new char[string_os_path_list.size() + 1];
    std::copy(string_os_path_list.begin(), string_os_path_list.end(), char_os_path_list);
    char_os_path_list[string_os_path_list.size()] = '\0'; // don't forget the terminating 0

    // Repeat:
    while(whileLoop == 1){
        //  Print prompt for user input: "osshell> " (no newline)
        std::cout << "osshell>";

        //  Get user input for next command
        std::cin >> input;
        printf("got input\n");
        /*
        strcpy(cmdHistory[0], input);
        history_list_size++;
        std::cout << cmdHistory[0] << "\n";
        */

        //history_list_size = history_list.size();
        //history_list[history_list_size-1] = input;

        /*
        //Add command good or bad into history vector
        printf("before line 76\n");
        history_list_size = history_list.size();
        printf("%i\n", history_list_size);
        std::vector<int>::iterator it;
        if(history_list_size == 0){
            it = history_list.begin();
            history_list.insert(it, input);
        }
        else{
            history_list.begin() + history_list_size - 1;
            it = history_list.insert(it, input);
        }
        */
        
        //  If command is `exit` exit loop / quit program
        printf("before exit strcmp\n");
        if(strcmp(input, exiting) == 0){
            //printf("exit = break\n");
            size++;
            exit(-1);
            //printf("PATH : %s\n", getenv("PATH"));

            //  If command is `history` print previous N commands
        } else if(strcmp(input, history) == 0){
            for(int i = 0; i < history_list_size; i++){
                //std::cout << "  " << i << ": " << history_list[i];
            }

        //printf("after the if checks\n");
    
    
        //  For all other commands, check if an executable by that name is in one of the PATH directories
    
        //   If no, print error statement: "<command_name>: Error command not found" (do include newline)

        } else if(execv(char_os_path_list, command_list_exec) == -1){
            //-1 means error was found
            // TODO change output to most recent element in history
            std::cout << input << ": Error command not found\n"; 
            break;
        }
        //  If input is a legal command -- execute it

        size++;
    }

    // Free allocated memory

    freeArrayOfCharArrays(command_list_exec, command_list.size() +1);

    // don't forget to free the string after finished using it
    delete[] char_os_path_list;

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

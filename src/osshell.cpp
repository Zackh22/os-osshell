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
#include <string>

#include <sys/wait.h>
#include <sys/mman.h>

void freeArrayOfCharArrays(char **array, size_t array_length);
void splitString(std::string text, char d, std::vector<std::string>& result);
void vectorOfStringsToArrayOfCharArrays(std::vector<std::string>& list, char ***result);
int getNumLinesInFile();
void* createSharedMemory(size_t size);

int main (int argc, char **argv)
{
    //char *input;
    std::string input;
    char* charInput;
    char* secondInput;
    std::string secondInputString;
    char history[] = "history";
    char exiting[] = "exit";
    char clear[] = "clear";
    int historySize = 0;
    char fileName[] = "history.txt";
    std::string currentLine;

    std::fstream file;
    file.open(fileName, std::fstream::in | std::fstream::out | std::fstream::app);
    //If the file does not exist
    if(!file){
        file.open(fileName, std::fstream::in | std::fstream::out | std::fstream::app);
    }
    std::string * historyList = new std::string[128];

    //put everything currently in the file into historyList[]
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

    for(int j=0; j < historySize; j++){
        printf("Printing what is currently in the history.txt file: ");
        std::cout<< historyList[j]<< std::endl;
    }
    
    // Get list of paths to binary executables
    // `os_path_list` supports up to 16 directories in PATH, 
    //     each with a directory name length of up to 64 characters
    std::vector<std::string> os_path_list;
    
    char* os_path = getenv("PATH");

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

    for(int i = 0; i < 10; i++){
        std::cout << os_path_list[i] << "\n";
    }

    // Repeat:
    while(whileLoop == 1){
        //  Print prompt for user input: "osshell> " (no newline)
        std::cout << "osshell>";

        //  Get user input for next command
        std::getline(std::cin, input);
        if(input.empty()){
            printf("input was a new line character\n");
            continue;
        }else{
            
            charInput = new char[input.length()+1];
            std::strcpy(charInput, input.c_str());

            file << input << std::endl;

            std::vector<std::string> input_list;

            //std::string example_command = "ls -lh";
            char **input_list_exec;
            //vectorOfStringsToArrayOfCharArrays(input_list, &input_list_exec);
            splitString(input, ' ', input_list);            

            historySize++;
            //Added input to the history list
            historyList[historySize] = input_list[0];
            //printf("input_list[0] = ");
            //std::cout << input_list[0] << std::endl;
            
            //Reassigning charInput to be the first element in the string that was just split
            std::strcpy(charInput, input_list[0].c_str());

            //  If command is `exit` exit loop / quit program
            if(strcmp(charInput, exiting) == 0){
                size++;
                exit(-1);
            
            //  If command is `history` print previous N commands
            } else if(strcmp(charInput, history) == 0){
               // charInput only checks the first part of input
                //std::string line = "";
                //std::string inputlistAt1 = input_list[1];
                //int index = 0;
                //int numPrint = 0; 
                //secondInput = new char[input.length()+1];

                for(int i = 1; i < historySize; i++){
                        std::cout << "  " << i << ": " << historyList[i] << "\n";
                }

                /*
                printf("above line 144\n");
                if(inputlistAt1.empty()){
                    //print all of the historyArray
                    std::cout<< "input_list[1] is true\n";
                    for(int i = 1; i < historySize; i++){
                        std::cout << "  " << i << ": " << historyList[i] << "\n";
                    }
                }
                secondInputString = input_list[1];
                
                std::strcpy(secondInput, secondInputString.c_str());

                std::cout << "Second Input is: " << secondInput << std::endl;
                
               
                //check if input is histroy and has no other argumens
                if(secondInputString.empty()){
                    
                    //Delete everything in history
                } else if(strcmp(secondInput, clear)){
                    //
                } else if(isdigit(secondInput[0])){
                    printf("inside isdigit conditional");
                    numPrint = std::stoi(input_list[1]);
                    //If the number is not negative or 0.
                    if(numPrint > 0){
                        //print only that many lines from the historyArray
                        //TODO set numPrint to the second argument that comes with history
                        
                        for(int i = historySize - numPrint; i < historySize; i++){
                            std::cout << "  " << i << ": " << historyList[i] << "\n";
                        }
                    }
                }
                */    

            //   If no, print error statement: "<command_name>: Error command not found" (do include newline)
            } else if(execv(charInput, command_list_exec) == -1){
                std::cout << input << ": Error command not found\n"; 
                continue;

             //  If input is a legal command -- execute it
            //  For all other commands, check if an executable by that name is in one of the PATH directories
            } else {
                printf("inside the execute if statement");
                void *shmem = createSharedMemory(sizeof(int));

                int pid = fork();

                //child
                if(pid == 0)
                {
                    //execute the first element
                    bool fileExists;
                    fileExists = false;
                    
                    while(fileExists){
                        for(int i = 0; i < os_path_list.size(); i++){
                            string_os_path_list = os_path_list[i];
                            char * char_os_path_list = new char[string_os_path_list.size() + 1];
                            std::copy(string_os_path_list.begin(), string_os_path_list.end(), char_os_path_list);
                            char_os_path_list[string_os_path_list.size()] = '\0'; // don't forget the terminating 0

                            if(execv(charInput, command_list_exec) == -1){
                                fileExists = true;
                            }
                        }
                    }
        
                    if(fileExists == true){
                        //executeFile
                        execv(char_os_path_list, command_list_exec);
                    } else{
                        std::cout << "ERROR\n";
                    }
                    
                   
                    //execv(char_os_path_list, command_list_exec);
                }
                //parent
                else
                {
                    int status;
                    waitpid(pid, &status, 0);
                }
            }

        }
    }

    // Free allocated memory
    freeArrayOfCharArrays(command_list_exec, command_list.size() +1);

    // don't forget to free the string after finished using it
    delete[] char_os_path_list;
    //historyFile.close();
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

void* createSharedMemory(size_t size)
{
    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_SHARED | MAP_ANONYMOUS;

    return mmap(NULL, size, protection, visibility, -1, 0);
}

/*
int getNumLinesInFile(){
    int numLines = 0;

    //open file
    std::fstream file;
    file.open("history.txt");

    //count lines in file
    while(!file.eof()){
        std::string line = "";
        getline(file, line);
        numLines++;
    }
    return numLines;
}
*/
    //hist = fopen("history.txt", "r");
    
    

    //std::ofstream historyFile;
    //historyFile.open("history.txt");
    //historySize = 0;
    //char charPerLine[360];

    /*
    while(std::cin.getline(charPerLine, 5)){
        std::cout << "hi\n";
        break
    }
    */    
    
    //char cmdHistory[127][127];    //Probs have to use malloc to allocate memory 

    //std::array<std::string, 128> history_list;


    // Example code for how to loop over NULL terminated list of strings
    /*
    int i = 0;
    while (os_path_list[i] != NULL)
    {
        printf("PATH[%2d]: %s\n", i, os_path_list[i]);
        i++;
    }
    */

    /*
    text = new char[example_command.length()+1];
    std::strcpy(text, example_command.c_str());
    std::cout << text << std::endl;
    */
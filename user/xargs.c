// /**
//  * @file xargs.c
//  * @author Baymine
//  * @brief Implementation of xargs. 
//  * @version 0.1
//  * @date 2022-12-25
//  * 
//  * @copyright Copyright (c) 2022
//  * 
//  */

// /* Preknowledge
// * 管道命令(|): 将左侧命令的标准输出转换成标准输入，提供给右侧命令
// 但是，大多数命令都不接受标准输入作为参数，只能直接在命令行输入参数，
// 这导致无法用管道命令传递参数。

// * xargs 命令： 将标准输入转为标准命令行参数

// * 字符0、数字0和‘\0’的区别
// ASCII码值 0 表示空字符，空字符就是平时所说的 ‘\0’。
// 字符 ‘0’，ASCII码值为 48，如：“012” 字符串中的 0 表示字符 ‘0’。
// 数字 0， 所说的数字 0，就是平时说的进制数字 0，其ASCII码为 0，在字符串中表示 ‘\0’，即空字符。
// */

// /* Main idea
// - 将标准输入和命令行输入存储在一个字符类型指针的数组中，然后调用exec运行
// */

// #include "kernel/types.h"
// #include "user/user.h"
// #include "kernel/param.h"

// #define MAX_STDIN 512

// void fmt_args(char *read_in, int len, char **args, int *args_cnt);

// int main(int argc, char **argv){
//     char stdin_buf[MAX_STDIN];  // store the parameters from standard input
//     int result;

//     char* args[MAXARG + 1];   // command line paramter
//     int args_cnt;

//     // Command line parameters after the xargs
//     for(int i = 1; i < argc; i++){
//         args[args_cnt] = malloc(sizeof(argv[i]));  
//         memcpy(args[args_cnt++], argv[i], sizeof(argv[i])); 
//     }

//     // Standard input from the pipe
//     if ((result = read(0, stdin_buf, sizeof(stdin_buf))) > 0){
//         fmt_args(stdin_buf, strlen(stdin_buf), args, &args_cnt);
//     }
//     exec(args[0], args);  // int exec(char*, char**);
    
//     exit(0);
// }

// /// @brief Standard input arguments to command line input argument.
// /// @param read_in Standard input args
// /// @param len length of read_in
// /// @param args command line args
// /// @param args_cnt length of args(N.O. of the args)
// void fmt_args(char *read_in, int len, char **args, int *args_cnt){
//     char cur_buf[MAX_STDIN];
//     int cur_buf_len = 0;

//     int i;
//     for (i = 0; i <= len; i++){
//         if((read_in[i] == ' ' || read_in[i] == '\n') && cur_buf_len){
//             args[*args_cnt] = malloc(cur_buf_len + 1);
//             memcpy(args[*args_cnt], cur_buf, cur_buf_len);  // copy the characters from cur_buf
//             args[*args_cnt][cur_buf_len] = 0; // end of string
//             cur_buf_len = 0;  // flush, ready for a new arg
//             (*args_cnt) ++; // next position
//         } else { // read the arguments char by char
//             cur_buf[cur_buf_len] = read_in[i];
//             cur_buf_len++;
//         }
//     }
    
// }
#include "../kernel/types.h"
#include "../user/user.h"
#include "../kernel/param.h"

#define MAX_STDIN 512

void fmt_args(char *read_in, int len, char **args, int *args_cnt)
{

    // 将一行标准输入转为多个参数
    char cur_buf[MAX_STDIN];
    int cur_buf_len = 0;

    int i;
    for (i = 0; i <= len; i++)
    {
        if ((read_in[i] == ' ' || read_in[i] == '\n') && cur_buf_len)
        {
            // 读到了一个新参数
            args[*args_cnt] = malloc(cur_buf_len + 1);
            memcpy(args[*args_cnt], cur_buf, cur_buf_len);
            args[*args_cnt][cur_buf_len] = 0;
            cur_buf_len = 0;
            (*args_cnt)++;
        }
        else
        {
            cur_buf[cur_buf_len] = read_in[i];
            cur_buf_len++;
        }
    }
}

int main(int argc, char **argv)
{
    char stdin_buf[MAX_STDIN];
    int result;

    char *args[MAXARG + 1];
    int args_cnt;

    // 读 xargs 后面的命令行参数
    for (int i = 1; i < argc; i++)
    {
        args[args_cnt] = malloc(sizeof(argv[i]));
        memcpy(args[args_cnt++], argv[i], sizeof(argv[i]));
    }
    // 读从管道传来的标准输入
    if ((result = read(0, stdin_buf, sizeof(stdin_buf))) > 0)
    {

        fmt_args(stdin_buf, strlen(stdin_buf), args, &args_cnt);
    }

    exec(args[0], args);

    exit(0);
}
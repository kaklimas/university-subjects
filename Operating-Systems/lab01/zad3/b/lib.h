#pragma once

#ifndef LIB_H
#define LIB_H

void wc_files(char* file_path);

int is_txt_file(char* string);

void** create_table(int size);

int add_block(void** pointers, char* file_path, int array_len);

void remove_block(void** pointers, int index, int array_len);

#endif //LIB_H

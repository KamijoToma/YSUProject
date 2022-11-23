/*
 * The EndOfCourse project for C Program Language in the YSU
 *
 * Date: 2022/11/22
 * Author: SkyRain PYH NXC
 * Instruction Teacher: NJC
 *
 * Name: Book Management System
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

// Book Item
typedef struct {
	char name[20]; // Book name
	char isbn[14]; // Book ISBN number
	char author[20]; // Book Author
	long long addAt; // When the book was added to the library
	long long updatedAt; // When the book was last updated
} Book;

// BookNode in BookList
typedef struct _BookNode {
	Book* item; // The book in this node
	struct _BookNode* next;
} BookNode;

// BookList (a List)
typedef struct {
	BookNode* head; // Head pointer of this List
} BookList;

typedef struct {
	char name[20];
	BookList* bookList;
} Library;


/*
 * Library* createLibrary(char*)
 * - create a Library object in the given pointer and name
 *   This function will check the length of the name
 * - Return allocated Library*
 */
Library* createLibrary(char* name){
	Library* library; // Define the pointer that is to be returnd
	library = (Library*) malloc(sizeof(Library)); // Dynamicly allocate memory for Library
	memcpy(library->name, name, sizeof(char) * (strlen(name) > 20 ? 20 : strlen(name) )); // String length limit for the name property
	library->bookList = (BookList*) malloc(sizeof(BookList)); // Allocate memory for bookList property
	library->bookList->head = NULL; // Initalize the head property with NULL. That ensures no error will occurred when looping the list.
	return library;
}

/*
 * void freeLibrary(Library*)
 * - free library
 */
void freeLibrary(Library* library){
	BookNode *node = library->bookList->head, *n;
	while(node!=NULL){
		n = node->next;
		free(node->item);
		free(node);
		node = n;
	}
	free(library->bookList);
	free(library);
}

/*
 * void addBook(Library*, Book*)
 * - Add book into Library
 */
void addBook(Library* library, Book* book){
	BookList* list = library->bookList; // Define variables
	BookNode* node = list->head;
	if(list->head == NULL){ // When the head is NULL, add book as head.
		list->head = (BookNode*) malloc(sizeof(BookNode)); // Allocate memory for BookNode
		list->head->item = (Book*) malloc(sizeof(Book)); // Allocate memory for Book
		memcpy(list->head->item, book, sizeof(Book)); // We copy the memory here, because book is a local variable referring to main()
		list->head->next = NULL; // Initalize the next property with NULL. That ensures no error will occurred when looping the list
	}else{
		BookNode* p;
		for(; node!=NULL; p=node, node=node->next); // Go to the last node of the list
		node = (BookNode*) malloc(sizeof(BookNode)); // Allocate memory for BookNode
		node->next = NULL; // Initalize the next property with NULL. That ensures no error will occurred when looping the list
		p->next = node; // Append the BookNode in the end of the list
		node->item = (Book*) malloc(sizeof(Book));
		memcpy(node->item, book, sizeof(Book)); // We copy the memory here, because book is a local variable referring to main()
	}
}

/*
 * void saveToFile(Library*, char*)
 * - save Library* data to given destination
 *
 *   FILE STRUCTURE
 *   +	Library Name (20Byte)
 *   +	Book numbers (sizeof(int)Byte)
 *   +	n*Book (n*sizeof(Book)Byte)
 */
int saveToFile(Library* library, char* dest){
	FILE* f = fopen(dest, "wb+");
	if(f == NULL) return 0;
	BookNode* node = library->bookList->head;
	int sum = 0;
	while(node != NULL){
		sum++;
		node = node->next;
	}
	if(sum == 0){
		fwrite(library->name, sizeof(char), 20, f);
		fwrite(&sum, sizeof(int), 1, f);
		fclose(f);
		return 1;
	}
	node = library->bookList->head;
	Book* bookList = (Book*) calloc(sum, sizeof(Book));
	for(int i=0; i<sum; i++){
		memcpy(bookList+i, node->item, sizeof(Book));
		node = node->next;
	}

	fwrite(library->name, sizeof(library->name), 1, f);
	fwrite(&sum, sizeof(int), 1, f);
	fwrite(bookList, sizeof(Book), sum, f);
	fclose(f);
	free(bookList);
	return 1;
}

/*
 * Library* loadFromFile(char*)
 * - load Library from given path.
 *   return NULL if errors occurred
 */
Library* loadFromFile(char* dest){
	FILE* f = fopen(dest, "r");
	if(f == NULL) return NULL;
	Library* library = malloc(sizeof(Library));
	library->bookList = (BookList*) malloc(sizeof(BookList));
	library->bookList->head = NULL;
	fread(library->name, sizeof(char), 20, f);
	int sum;
	fread(&sum, sizeof(int), 1, f);
	if(sum == 0) return library;
	Book* bookList = (Book*) malloc(sizeof(Book)*sum);
	fread(bookList, sizeof(Book), sum, f);
	fclose(f);
	library->bookList->head = (BookNode*) malloc(sizeof(BookNode));
	library->bookList->head->next = NULL;
	library->bookList->head->item = (Book*) malloc(sizeof(Book));
	memcpy(library->bookList->head->item, bookList, sizeof(Book));
	BookNode *p=library->bookList->head;
	for(int i=1; i<sum; i++, p=p->next){
		p->next = (BookNode*) malloc(sizeof(BookNode));
		p->next->item = (Book*) malloc(sizeof(Book));
		memcpy(p->next->item, bookList+i, sizeof(Book));
		p->next->next = NULL;
	}
	free(bookList);
	return library;
}

/*
 * long long get_timestamp()
 * - Get the timestamp now
 */
long long get_timestamp()
{
	time_t seconds = time(NULL); //The function time(NULL) returns the time since the Epoch (00:00:00 UTC, January 1, 1970), measured in seconds.
	return (long long)seconds;
}

/*
 * int stdinRead(char*, int);
 * - Read the stdin with given length
 */
void stdinRead(char* data, int size){
	fgets(data, size, stdin);
	char* p;/*
	p = strchr(data, '\r');
	if(p!=NULL) p = '\0';*/
	while(p=strchr(data, '\n'), p!=NULL) *p = '\0';
	fflush(stdin);
}

/*
 * The enterance of this program
 */
int main(){
	Library* library = createLibrary("YSULib");
	char command;

	int shouldExit = 114514;
	while(shouldExit == 114514){
		system("cls");
		fflush(stdin);
		printf("========== Book Management System ==========\n");
		printf(" - [l] Show books in current library\n");
		printf(" - [a] Add a new book\n");
		printf(" - [s] Save book data to disk\n");
		printf(" - [L] Load book data from disk\n");
		printf("============================================\n");
		printf(" - [v] Show version information\n");
		printf(" - [x] Exit this system\n");
		printf("============================================\n");
		printf(" + Current Library: %s\nPress C to change the name.\n\n", library->name);

		while(scanf("%c", &command), command=='\n' || command==' '); // Get rid of `pause`
		fflush(stdin);
		switch(command){
			case 'l':{
						 BookList* list = library->bookList;
						 BookNode* head = list->head;
						 if(head == NULL){
							 printf(" x There is no book in the library, shame!\n");
						 }else{
							 int sum = 0;
							 Book* b;
							 while(head != NULL){
								 sum++;
								 b = head->item;
								 printf("Book[%d] %s\n", sum, b->name);
								 head = head->next;
							 }
						 }
						 system("pause");
						 break;
					 }
			case 'a':{
						 Book b;
						 printf("\n");
						 printf(" - Please input book name (in 19 characters): ");
						 stdinRead(b.name, 20);
						 printf(" - Please input the author of this book: ");
						 stdinRead(b.author, 20);
						 printf(" - Please input the ISBN code of this book: ");
						 // TODO: Check if the ISBN is vaild
						 stdinRead(b.isbn, 14);
						 b.addAt = get_timestamp();
						 b.updatedAt = get_timestamp();
						 addBook(library, &b);
						 system("pause");
						 break;
					 }

			case 'v':{
						 printf("Version 1.0.0\n");
						 printf("关注永雏塔菲喵，关注永雏塔菲谢谢喵\n");
						 system("pause");
						 break;
					 }
			case 'x':{
						 printf("Bye.\n");
						 shouldExit = 1919810;
						 break;
					 }
			case 'L':{
						 printf("Please input the path of the data file: ");
						 char path[50];
						 stdinRead(path, 50);
						 Library* l = loadFromFile(path);
						 if(l == NULL){
							 printf("Path or file is invalid.\n");
						 }else{
							 freeLibrary(library);
							 library = l;
							 printf("Successfully loaded.\n");
						 }
						 system("pause");
						 break;
					 }
			case 's':{
						 printf("Please input the path of the data file: ");
						 char path[50];
						 stdinRead(path, 50);
						 if(saveToFile(library, path) == 0){
							 printf("Failed to open file: Errno[%d] %s\n", errno, strerror(errno));
						 }else{
							 printf("Save success!\n");
						 }
						 system("pause");
						 break;
					 }
			case 'C':{
						 printf("Please input the name you like: ");
						 stdinRead(library->name, 20);
						 system("pause");
						 break;
					 }

			default:{
						printf("Please input correct command!\n");
						system("pause");
					}
		}
	}

	return 0;
}

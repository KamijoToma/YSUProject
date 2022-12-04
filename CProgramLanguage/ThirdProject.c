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


#define BOOK_NAME_LENGTH 21
#define BOOK_ISBN_LENGTH 18
#define BOOK_AUTHOR_LENGTH 21
#define LIBRARY_NAME_LENGTH 20

// Book Item
typedef struct {
	char name[BOOK_NAME_LENGTH]; // Book name
	char isbn[BOOK_ISBN_LENGTH]; // Book ISBN number
	char author[BOOK_AUTHOR_LENGTH]; // Book Author
	long long addAt; // When the book was added to the library
	long long updatedAt; // When the book was last updated
} Book;

// BookNode in BookList
typedef struct _BookNode {
	Book item; // The book in this node
	struct _BookNode* next;
} BookNode;

// BookList (a List)
typedef struct {
	BookNode* head; // Head pointer of this List
} BookList;

typedef struct {
	char name[LIBRARY_NAME_LENGTH];
	BookList bookList;
} Library;


/*
 * Library* createLibrary(char*)
 * - create a Library object in the given name
 *   This function will check the length of the name
 * - Return allocated Library*
 */
Library* createLibrary(char* name){
	Library* library; // Define the pointer that is to be returnd
	library = (Library*) malloc(sizeof(Library)); // Dynamicly allocate memory for Library
	memcpy(library->name, name, sizeof(char) * (strlen(name) > LIBRARY_NAME_LENGTH ? LIBRARY_NAME_LENGTH : strlen(name) )); // String length limit for the name property
	library->bookList.head = NULL; // Initalize the head property with NULL. That ensures no error will occurred when looping the list.
	return library;
}

/*
 * void freeLibrary(Library*)
 * - free library
 */
void freeLibrary(Library* library){
	BookNode *node = library->bookList.head, *n;
	while(node!=NULL){
		n = node->next;
		free(node);
		node = n;
	}
	free(library);
}

/*
 * void addBook(Library*, Book*)
 * - Add book into Library
 */
void addBook(Library* library, Book* book){
	BookNode* node = library->bookList.head;
	if(library->bookList.head == NULL){ // When the head is NULL, add book as head.
		library->bookList.head = (BookNode*) malloc(sizeof(BookNode)); // Allocate memory for BookNode
		library->bookList.head->item = *book;
		library->bookList.head->next = NULL; // Initalize the next property with NULL. That ensures no error will occurred when looping the list
	}else{
		BookNode* p;
		for(; node!=NULL; p=node, node=node->next); // Go to the last node of the list
		node = (BookNode*) malloc(sizeof(BookNode)); // Allocate memory for BookNode
		node->next = NULL; // Initalize the next property with NULL. That ensures no error will occurred when looping the list
		p->next = node; // Append the BookNode in the end of the list
		node->item = *book;
	}
}

/*
 * void saveToFile(Library*, char*)
 * - save Library* data to given destination
 *
 *   FILE STRUCTURE
 *   +	BOOK_NAME_LENGTH BOOK_AUTHOR_LENGTH BOOK_ISBN_LENGTH LIBRARY_NAME_LENGTH (4*sizeof(int))Byte
 *   +	Library Name (20Byte)
 *   +	Book numbers (sizeof(int)Byte)
 *   +	n*Book (n*sizeof(Book)Byte)
 */
int saveToFile(Library* library, char* dest){
	FILE* f = fopen(dest, "wb+"); // Open file handler
	if(f == NULL) return 0; // Error process
	BookNode* node = library->bookList.head;
	int sum = 0;
	// Get book number
	while(node != NULL){
		sum++;
		node = node->next;
	}
	if(sum == 0){ // Book number == 0
		int head[] = {BOOK_NAME_LENGTH, BOOK_AUTHOR_LENGTH, BOOK_ISBN_LENGTH, LIBRARY_NAME_LENGTH};
		fwrite(head, sizeof(int), 4, f); // Write Meta
		fwrite(library->name, sizeof(char), LIBRARY_NAME_LENGTH, f); // Write Library name 
		fwrite(&sum, sizeof(int), 1, f); // Write Book number
		fclose(f);// Close handler
		return 1; // Return 
	}
	node = library->bookList.head;
	Book* bookList = (Book*) calloc(sum, sizeof(Book)); // Alloc memory
	for(int i=0; i<sum; i++){
		bookList[i] = node->item;
		node = node->next;
	}

	int head[] = {BOOK_NAME_LENGTH, BOOK_AUTHOR_LENGTH, BOOK_ISBN_LENGTH, LIBRARY_NAME_LENGTH};
	fwrite(head, sizeof(int), 4, f);
	fwrite(library->name, sizeof(library->name), 1, f);
	fwrite(&sum, sizeof(int), 1, f);
	//fwrite(bookList, sizeof(Book), sum, f);
	for(int i=0; i<sum; i++){ // Write book
		fwrite(bookList[i].name, sizeof(char), BOOK_NAME_LENGTH, f);
		fwrite(bookList[i].author, sizeof(char), BOOK_AUTHOR_LENGTH, f);
		fwrite(bookList[i].isbn, sizeof(char), BOOK_ISBN_LENGTH, f);
		fwrite(&bookList[i].addAt, sizeof(long long), 1, f);
		fwrite(&bookList[i].updatedAt, sizeof(long long), 1, f);
	}
	fclose(f);
	free(bookList);
	return 1;
}

/*
 * int checkISBN(char*)
 * - This function check if the ISBN is valid.
 *   Returns 1 if valid
 *   else returns 0 if  isbn string is too short or it is invalid
 */
int checkISBN(char* isbnCode){
	if(strlen(isbnCode)<13) return 0;
	int sum=0, len=strlen(isbnCode);
	char c;
	for(int i=0,j=1; i<len-1; i++){
		if(isbnCode[i] >= '0' && isbnCode[i] <= '9'){
			if(j%2==1){
				sum+=isbnCode[i]-'0';
			}else{
				sum+=3*(isbnCode[i]-'0');
			}
			j++;
		}
	}
	c = isbnCode[len-1];
	sum=10-(sum%10);
	if(sum == 10) return c == 'X' || c == 'x';
	else return sum == c-'0';
}

// Deprecated
Library* loadFromFilev1(char* dest){
	FILE* f = fopen(dest, "r");
	if(f == NULL) return NULL;
	Library* library = malloc(sizeof(Library));
	library->bookList.head = NULL;
	int head[4];
	fread(head, sizeof(head), 1, f);
	fread(library->name, sizeof(char), LIBRARY_NAME_LENGTH, f);
	int sum;
	fread(&sum, sizeof(int), 1, f);
	if(sum == 0) return library;
	Book* bookList = (Book*) malloc(sizeof(Book)*sum);
	fread(bookList, sizeof(Book), sum, f);
	fclose(f);
	library->bookList.head = (BookNode*) malloc(sizeof(BookNode));
	library->bookList.head->next = NULL;
	library->bookList.head->item = bookList[0];
	BookNode *p=library->bookList.head;
	for(int i=1; i<sum; i++, p=p->next){
		p->next = (BookNode*) malloc(sizeof(BookNode));
		p->next->item = bookList[i];
		p->next->next = NULL;
	}
	free(bookList);
	return library;
}

void strfit(char* d, char* s, int size){
	// Fit string to given size
	if(strlen(s)+1 <= size){
		memcpy(d, s, sizeof(char)*strlen(s)+1);
	}else{
		memcpy(d, s, size);
		d[size-1] = '\0';
	}
}

/*
 * Library* loadFromFile(char*)
 * - load Library from given path.
 *   return NULL if errors occurred
 */
Library* loadFromFile(char* dest){
	FILE* f = fopen(dest, "r");
	if(f == NULL) return NULL;
	Library* library = (Library*) malloc(sizeof(Library));
	library->bookList.head = NULL;
	int head[4];
	fread(head, sizeof(head), 1, f);
	char* libraryString = (char*) malloc(sizeof(char)*head[3]);
	fread(libraryString, sizeof(char), head[3], f);
	strfit(library->name, libraryString, LIBRARY_NAME_LENGTH);
	free(libraryString);
	int sum;
	fread(&sum, sizeof(int), 1, f);
	if(sum == 0) return library;
	Book* bookList = (Book*) malloc(sizeof(Book)*sum);
	for(int i=0; i<sum; i++){
		char* t;
		t = (char*) malloc(sizeof(char*)*head[0]);
		fread(t, sizeof(char), head[0], f);
		strfit(bookList[i].name, t, BOOK_NAME_LENGTH);
		free(t);
		t = (char*) malloc(sizeof(char*)*head[1]);
		fread(t, sizeof(char), head[1], f);
		strfit(bookList[i].author, t, BOOK_AUTHOR_LENGTH);
		free(t);
		t = (char*) malloc(sizeof(char*)*head[2]);
		fread(t, sizeof(char), head[2], f);
		strfit(bookList[i].isbn, t, BOOK_ISBN_LENGTH);
		free(t);
		fread(&bookList[i].addAt, sizeof(long long), 1, f);
		fread(&bookList[i].updatedAt, sizeof(long long), 1, f);
	}
	fclose(f);
	library->bookList.head = (BookNode*) malloc(sizeof(BookNode));
	library->bookList.head->next = NULL;
	library->bookList.head->item = bookList[0];
	BookNode *p=library->bookList.head;
	for(int i=1; i<sum; i++, p=p->next){
		p->next = (BookNode*) malloc(sizeof(BookNode));
		p->next->item = bookList[i];
		p->next->next = NULL;
	}
	free(bookList);
	return library;
}	


/* int deleteBook(Library*, int)
 * - Delete a book from the library
 *   Return the id of the deleted book
 *   Return -1 if the book isnot found
 */
int deleteBook(Library* library, unsigned int index){
	BookNode* node = library->bookList.head, *p=node;
	if(node == NULL) return -1;
	if(index == 1){
		p = library->bookList.head;
		library->bookList.head = library->bookList.head->next;
		free(p);
		return index;
	}
	int now = 1;
	while(now != index){
		p = node;
		node = node->next;
		if(node == NULL) return -1;
		now++;
	}
	p->next = node->next;
	free(node);
	return index;
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
 * void timestampToTime(long long, char*, char*, int)
 * - Convert timestamp to date string in the given format
 */
void timeStampToTime(long long timestamp, char* dest, char* format, int sizeofDest){
	struct tm tm;
	time_t tick = (time_t) timestamp;
	tm = *localtime(&tick);
	strftime(dest, sizeofDest, format, &tm);
}

/*
 * void printBook(Book*)
 * - print book on stdout
 */
void printBook(Book* book){
	char addTime[20], updatedTime[20];
	timeStampToTime(book->addAt, addTime, "%Y-%m-%d %H:%M:%S", sizeof(addTime));
	timeStampToTime(book->updatedAt, updatedTime, "%Y-%m-%d %H:%M:%S", sizeof(updatedTime));
	printf("%-*s|%-*s|%-*s|%-20s|%-20s\n", BOOK_NAME_LENGTH-1, book->name, BOOK_AUTHOR_LENGTH-1, book->author, BOOK_ISBN_LENGTH-1, book->isbn, addTime, updatedTime);
}

#define PRINT_HEAD printf("========================================================================================================\n");\
	printf("%-3s %-*s %-*s %-*s %-20s %-20s\n","ID", BOOK_NAME_LENGTH - 1, "BOOK NAME", BOOK_AUTHOR_LENGTH - 1, "AUTHOR", BOOK_ISBN_LENGTH - 1, "ISBN", "ADD TIME", "UPDATE TIME");\
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
#define PRINT_TAIL printf("========================================================================================================\n");
/*
 * void printBookList(Library*)
 * - Print a table of books
 */
void printBookList(Library* library){
	int sum=1;
	BookNode* node = library->bookList.head;
	PRINT_HEAD
	while(node != NULL){
		printf("%-3d ", sum);
		printBook(&node->item);
		node = node->next;
		sum++;
	}
	PRINT_TAIL
}

/*
 * int getIDByBook(Library*, Book*)
 * - return the id
 */
int getIDByBook(Library* library, Book* book){
	int id = 1;
	BookNode* node = library->bookList.head;
	while(&node->item != book){
		node = node->next;
		if(node == NULL) return -1;
		id++;
	}
	return id;
}

/*
 * int existsBook(Library*, int)
 * - Return the id of the book. If not found, return -1
 */
int existsBook(Library* library, int id){
	if(id < 1) return -1;
	BookNode* node = library->bookList.head;
	int sum = 1;
	while(sum != id){
		if(node == NULL) return -1;
		node = node->next;
		sum++;
	}
	return sum;
}

/*
 * int changeBook(Library*, Book*, int)
 * - change the book by id
 *   return the id of the changed book
 *   return -1 if the id is invaild
 */
int changeBook(Library* library, Book* book, int id){
	if(existsBook(library, id) == -1) return -1;
	int sum = 1;
	BookNode* node = library->bookList.head;
	while(sum!=id){
		node = node->next;
		sum++;
	}
	long long addTimestamp = node->item.addAt;
	node->item = *book;
	node->item.updatedAt = get_timestamp();
	node->item.addAt = addTimestamp;
	return id;
}

/*
 * int stdinRead(char*, int);
 * - Read the stdin with given length
 */
void stdinRead(char* data, int size){
	fgets(data, size, stdin);
	char* p;
	while(p=strchr(data, '\n'), p!=NULL) *p = '\0';
	fflush(stdin);
}

/*
 * int inputBook(Book*)
 * input the book data
 */
int inputBook(Book* b){
	printf(" - Please input book name (in %d characters): ", BOOK_NAME_LENGTH-1);
	stdinRead(b->name, BOOK_NAME_LENGTH);
	printf(" - Please input the author of this book: ");
	stdinRead(b->author, BOOK_AUTHOR_LENGTH);
	printf(" - Please input the ISBN code of this book: ");
	stdinRead(b->isbn, BOOK_ISBN_LENGTH);
#ifndef TP_FORCE_RIGHT_ISBN
	if(!checkISBN(b->isbn)) printf("Warning: ISBN invalid!\n");
#else
	if(!checkISBN(b->isbn)){
		printf("Error: ISBN invalid!\n");
		return 0;
	}
#endif
	b->addAt = get_timestamp();
	b->updatedAt = get_timestamp();
	return 1;
}

/*
 * Book* getBookByISBN(Library*, char*)
 * - Get book by given ISBN code
 */
Book* getBookByISBN(Library* library, char* isbn){
	BookNode* node = library->bookList.head;
	if(node == NULL) return NULL;
	Book* book = NULL;
	while(node != NULL){
		if(strcmp(node->item.isbn, isbn) == 0){
			book = &(node->item);
			break;
		}
		node = node->next;
	}
	return book;
}

/*
 * Book* getBookByName(Library* char*)
 * - Get book by given book name
 */
Book* getBookByName(Library* library, char* name){
	BookNode* node = library->bookList.head;
	if(node == NULL) return NULL;
	Book* book = NULL;
	PRINT_HEAD
	while(node != NULL){
		if(strstr(node->item.name, name) != NULL){
			book = &node->item;
			printf("%-3d ", getIDByBook(library, book));
			printBook(book);
		}
		node = node->next;
	}
	PRINT_TAIL
	return book;
}

/*
 * void pressEnterToContinue()
 * - Generate a message
 */
void pressEnterToContinue(){
	printf("Press ENTER to continue.");
	char a[10];
	stdinRead(a, 10);
}

/*
 * The enterance of this program
 */
int main(){
	Library* library = createLibrary("YSULib");
	char command;

	int shouldExit = 0;
	while(shouldExit == 0){
#ifdef __WIN32__
		system("cls");
#endif
		fflush(stdin);
		printf("========== Book Management System ==========\n");
		printf(" - [l] Show books in current library\n");
		printf(" - [a] Add a new book\n");
		printf(" - [d] Delete a book from the library.\n");
		printf(" - [c] Change book data by id\n");
		printf(" - [q] Query book by its name\n");
		printf(" - [i] Query book by its ISBN\n");
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
						 printBookList(library);
						 break;
					 }
			case 'a':{
						 Book b;
						 printf("\n");
						 if(inputBook(&b)){
							addBook(library, &b);
						 }else{
							 printf("Input invalid!\n");
						 }
						 break;
					 }

			case 'v':{
						 printf("TP. The Final Version\n");
#ifdef __DEBUG
						 printf("关注永雏塔菲喵，关注永雏塔菲谢谢喵\n");
#endif
						 break;
					 }
			case 'x':{
						 printf("Bye.\n");
						 shouldExit = 1;
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
							 printBookList(library);
						 }
						 break;
					 }
			case 'i':{
						 printf("Please input book ISBN:");
						 char isbn[BOOK_ISBN_LENGTH];
						 stdinRead(isbn, BOOK_ISBN_LENGTH);
						 Book* b = getBookByISBN(library, isbn);
						 if(b == NULL) printf("Book does not exist!\n");
						 else{
							 PRINT_HEAD
							 printf("%-3d ", getIDByBook(library, b));
							 printBook(b);
							 PRINT_TAIL
						 }
						 break;
					 }
			case 'q':{
						 printf("Please input book name:");
						 char name[BOOK_NAME_LENGTH];
						 stdinRead(name, BOOK_NAME_LENGTH);
						 if(getBookByName(library, name) == NULL) printf("Book does not exist!\n");
						 break;
					 }
			case 'd':{
						 printBookList(library);
						 printf("\n\nPlease input the book id you want to delete: ");
						 int index;
						 scanf("%d", &index);
						 fflush(stdin);
						 if(deleteBook(library, index) == -1){
							 printf("Invalid index is given!\n");
						 }else{
							 printf("Successfully deleted.\n");
						 }
						 break;
					 }
			case 'c':{
						 printBookList(library);
						 printf("Please input the book id that you want to change: ");
						 int id;
						 scanf("%d", &id);
						 fflush(stdin);
						 if(existsBook(library, id) == -1){
							 printf("Book id is invalid! \n");
							 //pressEnterToContinue();
							 break;
						 }
						 Book book;
						 printf("\n");
						 if(inputBook(&book)){
							changeBook(library, &book, id);
						 }else{
							 printf("Input invalid!\n");
						 }
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
						 break;
					 }
			case 'C':{
						 printf("Please input the name you like: ");
						 stdinRead(library->name, LIBRARY_NAME_LENGTH);
						 break;
					 }

			default:{
						printf("Please input correct command!\n");
					}
		}
		if(shouldExit == 0) pressEnterToContinue();
	}
	return 0;
}

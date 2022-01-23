/*
Author: Ali Salloum
Time and date: 6:26 pm 9/30/2020
A program to run a library system by taking commands from the library administrator from CLI and performing it.
The commands can be adding a book or a customer or editing or viewing the list, etc...
Implementation: used a linked list to add and remove customers and books.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int command;

typedef struct book
{
    char title[60];
    char author[50];
    int year;
    float book_rate;
    struct book *next_book;
} book;

struct customer
{
    char name[50];
    int age;
    float customer_rate;
    struct customer *next_customer;
};

book *first_book = NULL;
struct customer *first_customer = NULL;

void main_menu();
void remove_last_newline(char string[]);

void fill_book(book *book_to_fill, char title[], char author[], int year, float book_rate);
void show_books();
void get_book_details(book *current_book);
void add_book();
book *find_book(char title[60]);
book *find_previous_book(char title[60]);
void update_book();
void remove_book();

void fill_customer(struct customer *customer_to_fill, char name[], int age, float customer_rate);
void show_customers();
void get_customer_details(struct customer *current_customer);
void add_customer();
struct customer *find_customer(char name[50]);
struct customer *find_previous_customer(char name[50]);
void update_customer();
void remove_customer();

int main()
{
    main_menu();
    scanf("%d", &command); // the number of the command
    getchar();
    // every scanf() leaves a newline character in buffer that is read by next scanf,
    // which causes a problem when using fgets so we need to use getchar.
    for (; command != 9;)
    {
        switch (command)
        {
        case 1:
            show_books();
            break;
        case 2:
            add_book();
            break;
        case 3:
            update_book();
            break;
        case 4:
            remove_book();
            break;
        case 5:
            show_customers();
            break;
        case 6:
            add_customer();
            break;
        case 7:
            update_customer();
            break;
        case 8:
            remove_customer();
            break;
        //actually we don't need the case where the command is nine because the
        //condition in the for loop is that command is not equal to nine (nine means quit)
        default: //if the number is not between one and nine (neither of the above cases)
            printf("\nError! Please enter a number between one and nine ! \n\n");
            break;
        }
        printf("Press Enter to continue");
        getchar(); // wait for the user to see the result of his command and press enter to proceed
        main_menu();
        scanf("%d", &command);
        getchar();
    }
    //the command is nine (quit)
    printf("\n\tThank you for using our system!\n");
    return 0;
}

void main_menu()
{
    printf("The main menu of the library system! Here is the list of commands :\n"
           "\t 1 : Show books\n"
           "\t 2 : Add a book\n"
           "\t 3 : Update a book\n"
           "\t 4 : Remove a book\n"
           "\t 5 : Show customers\n"
           "\t 6 : Add a customer\n"
           "\t 7 : Update a customer\n"
           "\t 8 : Remove a customer\n"
           "\t 9 : Quit\n"
           "Please enter the number of your command :\t");
}

//this function removes the newline at the end of a string(if any)
void remove_last_newline(char string[])
{
    int size = strlen(string);
    if (string[size - 1] == '\n')
        string[size - 1] = '\0';
}

//to assign the details of a book to the node in linked list
void fill_book(book *book_to_fill, char title[], char author[], int year, float book_rate)
{
    remove_last_newline(title);         // here we need to remove last newline because when we use fgets() it reads the newline too
    strcpy(book_to_fill->title, title); // strcpy assigns the value of the second string to the first string
    remove_last_newline(author);        // the same thing as in "title"
    strcpy(book_to_fill->author, author);
    book_to_fill->year = year;
    book_to_fill->book_rate = book_rate;
}

void show_books()
{
    // if the pointer to the node is null then the linked list is empty
    if (first_book == NULL)
    {
        printf("\n\tThere are no books in the library. You may add books by pressing 2 \n\n");
    }
    else
    {
        printf("\nHere is the list of all available books in the library :\n\n");
        book *current_book = first_book;
        auto int counter = 1;        //for numbering the books
        while (current_book != NULL) //this condition means that there are still books in the library
        {
            printf("Book no. %d\n", counter);
            printf("\t title     : %s  \n", current_book->title);
            printf("\t author    : %s  \n", current_book->author);
            printf("\t year      : %d  \n", current_book->year);
            printf("\t book rate : %f\n\n", current_book->book_rate);
            current_book = current_book->next_book; //move to the next book in the linked list
            counter++;
        }
        printf("Those were all the books in the library.\n\n");
    }
}

void get_book_details(book *current_book)
{
    char title[61];  //here we need an extra char for storing the newline temporarily (because the fgets() reads the newline)
    char author[51]; //the same thing as in title
    int year;
    float book_rate;
    printf("\t title : \t");
    fgets(title, 61, stdin); //fgets() reads a string with space until it finds a newline (it reads it as well)
    printf("\t author : \t");
    fgets(author, 51, stdin);
    printf("\t year : \t");
    scanf("%d", &year); //we need a getchar() after each scanf to prevent leaving a newline in buffer and to prevent making the fgets() read it
    getchar();
    printf("\t book rate : \t");
    scanf("%f", &book_rate);
    getchar();
    fill_book(current_book, title, author, year, book_rate); //to assign the values in the node
}

void add_book()
{
    book *current_book = (book *)malloc(sizeof(book)); // allocating memory for a pointer to struct book
    //here we don't need the word struct because we used typedef
    printf("\nPlease enter the following information :\n");
    get_book_details(current_book);
    current_book->next_book = NULL;
    //if there're no books in the library
    if (first_book == NULL)
    {
        first_book = current_book;
    }
    else
    {
        book *temporary = first_book;
        while (temporary->next_book != NULL) //to reach the last node in the linked list
        {
            temporary = temporary->next_book;
        }
        temporary->next_book = current_book;
    }
    printf("\nThe book has been added successfully\n\n");
}

book *find_book(char title[60])
{
    book *temporary = first_book; //to start from the first book and traverse through all the books
    while (temporary != NULL)
    {
        if (!strcmp(temporary->title, title))
            return temporary; //the strcmp() returns zero if the strings are equal
        temporary = temporary->next_book;
    }
    return NULL; //meaning that there is no book with this title
}

//we need to find the previous node to link it with the next node of a deleted node
book *find_previous_book(char title[60])
{
    if (first_book == NULL || first_book->next_book == NULL)
        return NULL; // if the list is empty or it has one book there is no previous node
    book *previous = NULL, *current = first_book;
    while (current != NULL)
    {
        if (!strcmp(current->title, title))
            return previous;
        previous = current; // update the previous and the current (move one step forward)
        current = current->next_book;
    }
    return NULL; //there is no book with such title
}

void update_book()
{
    printf("\nPlease enter the title of the book you want to update :\t");
    char to_update[61];
    fgets(to_update, 61, stdin);    //fgets() reads a string with space until it finds a newline (it reads it as well)
    remove_last_newline(to_update); //we need to remove that newline
    book *current_book = find_book(to_update);
    if (current_book == NULL) // the book was not found
    {
        printf("\n\tThe title of the book you entered does not exist in the library system. Please check the book title or add this book.\n");
    }
    else //the book exists and it is "current book"
    {
        printf("\nEnter the new details of the book:\n");
        get_book_details(current_book);
        printf("\nThe book has been updated successfully.\n\n");
    }
}

void remove_book()
{
    printf("\nPlease enter the title of the book you want to remove :\t");
    char to_remove[61];
    fgets(to_remove, 61, stdin);    //fgets() reads a string with space until it finds a newline (it reads it as well)
    remove_last_newline(to_remove); //we need to remove that newline
    book *book_to_remove = find_book(to_remove);
    book *previous = find_previous_book(to_remove);
    if (book_to_remove == NULL)
    {
        printf("\n\tThe title of the book you entered does not exist in the library system.\n\n");
    }
    else
    {
        if (first_book == book_to_remove)
        {
            first_book = first_book->next_book; //the second book became the first book
            free(book_to_remove);               //the opposite of allocating. we don't need this memory any more so we set it free
        }
        else
        {
            previous->next_book = book_to_remove->next_book; //here we link the previous node of the removed node to its next one
            free(book_to_remove);
        }
        printf("\n\tThe book has been removed successfully.\n\n");
    }
}

//to assign the details of a customer to the node in linked list
void fill_customer(struct customer *customer_to_fill, char name[], int age, float customer_rate)
{
    remove_last_newline(name);
    strcpy(customer_to_fill->name, name);
    customer_to_fill->age = age;
    customer_to_fill->customer_rate = customer_rate;
}

void show_customers()
{
    if (first_customer == NULL) //the list is empty
    {
        printf("\n\tThere are no customers in the library system. You may add customers by pressing 6 \n\n");
    }
    else
    {
        printf("\nHere is the list of all customers in the library system:\n\n");
        struct customer *current_customer = first_customer; //for traversing the linked list
        auto int counter = 1;                               //for numbering the customers
        while (current_customer != NULL)
        {
            printf("Customer no. %d\n", counter);
            printf("\t name     : %s  \n", current_customer->name);
            printf("\t age      : %d  \n", current_customer->age);
            printf("\t customer rate : %f\n\n", current_customer->customer_rate);
            current_customer = current_customer->next_customer; //move to the next node
            counter++;
        }
        printf("Those were all the customers in the library system.\n\n");
    }
}

void get_customer_details(struct customer *current_customer)
{
    char name[51];
    int age;
    float customer_rate;
    printf("\t name : \t\t");
    fgets(name, 51, stdin);
    printf("\t age  : \t\t");
    scanf("%d", &age);
    getchar();
    printf("\t customer rate : \t");
    scanf("%f", &customer_rate);
    getchar();
    fill_customer(current_customer, name, age, customer_rate);
}

void add_customer()
{
    struct customer *current_customer = (struct customer *)malloc(sizeof(struct customer));
    printf("\nPlease enter the following information :\n");
    get_customer_details(current_customer);
    current_customer->next_customer = NULL;
    if (first_customer == NULL)
    {
        first_customer = current_customer;
    }
    else
    {
        struct customer *temporary = first_customer;
        while (temporary->next_customer != NULL)
        {
            temporary = temporary->next_customer;
        }
        temporary->next_customer = current_customer;
    }
    printf("\nThe customer has been added successfully.\n\n");
}

struct customer *find_customer(char name[50])
{
    struct customer *temporary = first_customer;
    while (temporary != NULL)
    {
        if (!strcmp(temporary->name, name))
            return temporary;
        temporary = temporary->next_customer;
    }
    return NULL;
}

//we need to find the previous node to link it with the next node of a deleted node
struct customer *find_previous_customer(char name[50])
{
    if (first_customer == NULL || !strcmp(first_customer->name, name))
        return NULL;
    struct customer *previous = NULL, *current = first_customer;
    while (current != NULL)
    {
        if (!strcmp(current->name, name))
            return previous;
        previous = current;
        current = current->next_customer;
    }
    return NULL;
}

void update_customer()
{
    printf("\nPlease enter the name of the customer you want to update :\t");
    char to_update[51];
    fgets(to_update, 51, stdin);
    remove_last_newline(to_update);
    struct customer *current_customer = find_customer(to_update);
    if (current_customer == NULL)
    {
        printf("\n\tThe name of the customer you entered does not exist in the library system. Please check the customer name or add this customer.\n\n");
    }
    else
    {
        printf("\nEnter the new details of the customer:\n");
        get_customer_details(current_customer);
        printf("\nThe customer has been updated successfully.\n\n");
    }
}

void remove_customer()
{
    printf("\nPlease enter the name of the customer you want to remove :\t");
    char to_remove[51];
    fgets(to_remove, 51, stdin);
    remove_last_newline(to_remove);
    struct customer *customer_to_remove = find_customer(to_remove);
    struct customer *previous = find_previous_customer(to_remove);
    if (customer_to_remove == NULL)
    {
        printf("\n\tThe name of the customer you entered does not exist in the library.\n\n");
    }
    else
    {
        if (first_customer == customer_to_remove)
        {
            first_customer = first_customer->next_customer;
            free(customer_to_remove);
        }
        else
        {
            previous->next_customer = customer_to_remove->next_customer;
            free(customer_to_remove);
        }
        printf("\n\tThe customer has been removed successfully.\n\n");
    }
}

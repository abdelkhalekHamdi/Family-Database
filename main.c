/****************************************
//LinkedIn Learning course challenge.
//Course name: Master C Language Pointers by Dan Gookin
//Code Description :
A light version of a database system for family members personal information
based on linked lists and file management, it allows the user to:
- Create a linked list of structures that contains the following information:
    Index, Name, Relationship, Age.
- Add a structure to the list.
- Remove a structure from the list.
- Save the linked list to a (.dat) file.
- Read the list from a (.dat) file.
---->Some features:
- Index updating after reading a list from a file.
- Delete all content of a file.
- Read from an empty file.
//Author : ABDELKHALEK HAMDI
//Created On : 20/08/2020
*****************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 16

struct family {
	char name[SIZE];
	char relationship[SIZE];
	int age;
	int index;
	struct family *next;
};

/*****Functions Prototype******/
/* read standard input, discard newline */
char *input(void);
/* allocate memory for a new structure */
struct family *allocate(void);

/* fill the structure */
void fill(struct family *s, int i);

/* output the list */
void output(struct family *s);

/* delete the first structure from the list*/
void delFirstStruct(struct family **s, int *y);

/* delete structure other than first*/
void delOtherStruct(struct family **s, int i, int *y);

/*order structure indices*/
void orderStruct( struct family *s);

/*save the list to the file*/
void saveToFile(struct family *s);

/*read the list from the file*/
void readFromFile(struct family **s, struct family **c, int *i);

/****** Main ******/
int main()
{
	FILE *fh;
	struct family *first, *current;
	int done = 0; /*to check the quit choice*/
	char choice[SIZE]; /*to choose from the menu*/
	int delIndex, addIndex = 0; /*to delete or add structure*/
	int opened = 0; /*to check whether the file was opened or not*/

   	 /* initialize first record to NULL */
	first = NULL;

	/* main menu and input loop */
	puts("Family Database");
	while(!done)
	{
		/* output menu */
		printf("\n");
		puts("Menu:");
		puts("A - Add record");
		puts("D - Delete record");
		puts("L - List all records");
		puts("O - Open");
		puts("S - Save");
		puts("Q - Quit");
		printf("Choice: ");
		/* read input */
		strcpy(choice,input() );
		/* select instruction */
		switch(choice[0])
		{
			/* add a record */
			case 'A':
			case 'a':
            		/*if the file is open, add record*/
            		if(opened == 1)
            		{
                		if(first == NULL)
                		{
				    first = allocate();
				    current = first;
				}
				else
				{
				    /*look for the last structure in the list*/
				    while(current->next != NULL)
					current = current->next;
				    /*add structure*/
				    current->next = allocate();
				    current = current->next;
				}
				/* fill structure */
				fill(current,++addIndex);
				/*mark the end of the list*/
				current->next = NULL;
						break;
            		}
			else
				puts("Cannot add a record now, please open the file before adding any record!");
				break;

			/* delete a record */
			case 'D':
			case 'd':
				printf("Enter the record index :");
				delIndex = strtod(input() ,NULL);
				/* check for chars or negative input*/
				if (delIndex <= 0)
				{
				    puts("Invalid Choice !");
				    break;
				}
				delIndex-=1;
				/*check if the record exits within the list*/
				if(delIndex > addIndex-1)
				    printf("Record %d does not exit!\n",delIndex+1);
				/* if structure to be deleted is the first*/
				else if(delIndex == 0)
				    {
					delFirstStruct(&first, &addIndex);
					printf("Record %d deleted!\n",delIndex+1);
				    }
				/*if other structure to be deleted*/
				else
				{
				    current = first;
				    delOtherStruct(&current, delIndex, &addIndex);
				    printf("Record %d deleted!\n",delIndex+1);
				}
				/*order the list after deleting*/
				orderStruct(first);
                		break;
			/* list all records */
			case 'L':
			case 'l':
				output(first);
				break;
            		/* open/retrieve existing records */
			case 'O':
			case 'o':
				readFromFile(&first, &current, &addIndex);
				output(first);
				/*file opened*/
				opened = 1;
				break;
			/* save all records */
			case 'S':
			case 's':
				if (first == NULL && opened == 0)
				    puts("List Empty, Nothing to save !");
				else
				    saveToFile(first);
						break;
			/* quit the program */
			case 'Q':
			case 'q':
				done = 1;
				puts("Bye!");
				break;
			default:
				puts("Unrecognized command");
		}
	}
	return(0);
}

/*****Functions Definition******/

/* read standard input, discard newline */
char *input(void)
{
	static char buffer[SIZE];
	char *r;
	int x;

	/* initialize the buffer */
	buffer[0]='\0';

	/* gather input */
	r = fgets(buffer,SIZE,stdin);
	if( r==NULL )
	{
		fprintf(stderr,"Input error\n");
		exit(1);
	}

	/* remove the newline */
	for(x=0; x<SIZE; x++)
	{
		/* remove newline */
		if( buffer[x]=='\n' )
		{
			buffer[x]='\0';
			break;
		}
	}

	return(buffer);
}

/* allocate memory for a new structure */
struct family *allocate(void)
{
	struct family *p;
	p = (struct family *)malloc( sizeof(struct family) * 1);
	if( p==NULL )
	{
		puts("Unable to allocate memory\n");
		exit(1);
	}
	return(p);
}

/* fill the structure */
void fill(struct family *s, int i)
{
	char temp[SIZE];
	s->index = i;
	printf("Record %d Name: ",i );
	strcpy(s->name,input() );
	printf("Relationship: ");
	strcpy(s->relationship,input() );
	printf("Age: ");
	strcpy(temp,input() );
	s->age = strtof(temp,NULL);
	/* the 'next' member is filled in the main() function */
}

/* output the list */
void output(struct family *s)
{
    if(s == NULL)
        puts("List is empty !");
	while( s!=NULL )
	{
		printf("%d: %s, %s, %d\n",
			s->index,
			s->name,
			s->relationship,
			s->age
			  );
		s = s->next;
	}
}

/* delete the first structure from the list*/
void delFirstStruct(struct family **s, int *y)
{
    struct family *temp;
    /*if first is the only structure in the list*/
    if ((*s)->next == NULL)
    {
        free(*s);
        *s = NULL;
        *y = 0; /* set addIndex to 0*/
    }
    /*if there are other structures in the list*/
    else
    {
        temp = *s;
        (*s)= (*s)->next;
        free(temp);
        *y -= 1; /*decrement addIndex*/
    }
}

/* delete structure other than first*/
void delOtherStruct(struct family **s, int i, int *y)
{
    struct family *temp;
    /*point for the structure to be deleted*/
    while((*s)->index != i)
        (*s) = (*s)->next;
    /*replace current->next by (current->next)->next*/
    temp = (*s)->next;
    (*s)->next = temp->next;
    free(temp);
    *y -= 1; /*decrement addIndex*/
}
/*order structure indices*/
void orderStruct( struct family *s)
{
    int y = 1;
    while(s != NULL)
    {
        s->index = y;
        y++;
        s = s->next;
    }
}
/*save the list to the file*/
void saveToFile(struct family *s)
{
    FILE *fh;
    fh = fopen("family.dat","w");
    if( fh==NULL )
    {
        puts("Unable to open file\n");
        return;
    }
    /*write all structures once at a time*/
    while( s != NULL )
    {
        fwrite(s,sizeof(struct family),1,fh);
        s = s->next;
    }
	fclose(fh);  /* close the file */
}

/*read the list from the file*/
void readFromFile(struct family **s, struct family **c, int *i)
{
    FILE *fh;
    fh = fopen("family.dat","r");
    if( fh==NULL )
    {
        puts("Unable to open file");
        return;
    }
    /*Check the size of the file*/
    fseek (fh, 0, SEEK_END);
    int size = ftell(fh);
    printf("File contains %d records, choose A to add records!\n",size/48);
    /*if empty*/
    if (size == 0)
    {
        fclose(fh);
        return;
    }
    /*reset the cursor*/
    fseek (fh, 0, SEEK_SET);
    /* allocate new structure to read from the file */
    *s = allocate();
    *c = *s;
    /*read all structures once at a time*/
    while(1)
    {
        fread(*c,sizeof(struct family),1,fh);
        if((*c)->next == NULL)
            break;
        (*c)->next = allocate();
        *c = (*c)->next;
    }
    (*c)->next = NULL;
    /*set the addIndex to the last index on the list*/
    *i = (*c)->index;
    fclose(fh); /* close the file */
}


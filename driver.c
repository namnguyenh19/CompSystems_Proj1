/* Read a number and determine if it is prime.
*/
#include        <stdio.h>
#include        <stdlib.h>
#include        <string.h>
#include 				<sys/stat.h>
#include 				<fcntl.h>
#include 				<assert.h>
#include				"main_header.h"

#define	LINE_LENGTH 50
#define NUM_PROCESS 100

/* Extern declarations: */

extern  int     optind;
extern  char    *optarg;

void printPros(FILE *file, void *pros);
List get_pros_list(char *file_name);
void insert_to_disk(List pros_list, List on_disk, int cur_time);

int main(int argc, char** argv)
{
	char input;
	char *algorithm = "first";	// default algorithm name
	char *file_name = "input.txt"; // default file name
	int quantum = 5; // default quantum value
	int mem_size = 100;			// default main memory size

	/* parse command line options example

	   an appropriate usage message should be included

	   a sample command line is provided below:

					./prog_name -l Java -s 500

	   where l option is used to specify the name of the language used (char array)
	         s option is used to specify the size or the number of lines in the file (int)

	*/


	while ((input = getopt(argc, argv, "f:a:m:q:")) != EOF)
	{
		switch ( input )
		{
			case 'f':
				// set the value of input file name (int value) based on optarg
				file_name = optarg;
				break;

			case 'a':
				if(strcmp(optarg, "best") == 0)
					algorithm  = optarg;

				else if(strcmp(optarg, "worst") == 0)
					algorithm = optarg;

				else if(strcmp(optarg, "first") != 0)
				{
					// exit if optarg unknown
					fprintf(stderr, "Unknown algorithm: %s\n ", optarg);
					exit(1);
				}
 				break;
					// should not get here

			case 'm':
					// set the value of main memory size (int value) based on optarg
					mem_size = atoi(optarg);
					break;

			case 'q':
					// set the value of quantum (int value) based on optarg
					quantum = atoi(optarg);
					break;

			default:
				fprintf(stderr, "No input given\n");
				break;
		}
	}

	// a debugging step is included to display the value of args read
	// printf("input = %s algorithm = %s mem_size = %d quantum = %d\n", file_name, algorithm, mem_size, quantum);

	//data structure initialization
	List pros_list = get_pros_list(file_name);
	pros_list = reverse(pros_list);
	int num_pros = len(pros_list);

	List on_disk = (List)malloc(sizeof(List)*num_pros);
	assert(on_disk);

	Memory *mem = (Memory *)malloc(sizeof(Memory)*1);
	assert(mem);
	Block *on_mem = (Block *)malloc(sizeof(Block)*num_pros);
	mem->on_memory = on_mem;
	mem->used = 0;
	mem->capacity = mem_size;
	mem->free = NULL;
	mem->num_block = 0;

	Schedule *schedule = (Schedule*)malloc(sizeof(Schedule)*1);
	assert(schedule);
	List run_pros = (List)malloc(sizeof(List)*num_pros);
	assert(run_pros);
	schedule->running = run_pros;
	schedule->pros_start = 0;
	schedule->cur_time = 0;

	int time_counter = 0;
	printf("stuck before loop???");
	while (time_counter < 100){
		// insert_to_disk(pros_list, on_disk, time_counter);
		time_counter++;
	}

	printf("stuck here???");
	print_list(printPros, stdout, on_disk);

	return 0;
}

// void insert_to_disk(List pros_list, List on_disk, int cur_time){
// 	Process *cur_pros = (Process *)pros_list->data;
// 	printf("comparing time\n");
// 	if(cur_time == cur_pros->time_created){
// 		insert(cur_pros, &on_disk);
// 		printf("insert successful\n");
// 		pop(&pros_list);
// 		printf("wtf\n");
// 	}
// }

List get_pros_list(char *file_name){
	char *cur_line = (char *)malloc(sizeof(char)*LINE_LENGTH);
	assert(cur_line!=NULL);

	FILE *fp = fopen(file_name, "r");
	size_t length = (size_t)LINE_LENGTH;
	int count = 0;
	int pros_info[4] = {0};
	List pros_list = NULL;

	const char s[2] = " ";

	while (getline(&cur_line, &length, fp) != -1) {
		char *token;
		Process *new_pros = (Process *)malloc(sizeof(Process));
   	/* get the first token */
   	token = strtok(cur_line, s);

   	/* walk through other tokens */
   	while( token != NULL )
   	{
      	pros_info[count] = atoi(token);
				count++;
      	token = strtok(NULL, s);
   	}

		count = 0;
		new_pros->time_created = pros_info[0];
		new_pros->process_id = pros_info[1];
		new_pros->memory = pros_info[2];
		new_pros->job_time = pros_info[3];
		new_pros->time_remaining = pros_info[3];
		pros_list = push(pros_list, new_pros);
	}

	fclose(fp);

	return pros_list;
}

void printPros(FILE *file, void *pros){
	Process *p = (Process *)pros;
	fprintf(file, "Time created: %d\n", p->time_created);
	fprintf(file, "Process ID: %d\n", p->process_id);
	fprintf(file, "Memory Size: %d\n", p->memory);
	fprintf(file, "Job Time: %d\n", p->job_time);
}

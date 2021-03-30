//NAME: Attiano Purpura-Pontoniere
//EMAIL: attiano@cs.ucla.edu
#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <limits.h>    /* PATH_MAX */
#include <getopt.h>    /* get_opt_long */
#include <string.h>    /* str_copy */
#include <ctype.h>

const int DEFAULT_MAX_ITEMS = 100000;

const int DEFAULT_MAX_HASH = 99991;

char** word_set = (char**) malloc(sizeof(char*)*DEFAULT_MAX_ITEMS);
int set_size = 0;

unsigned int hash_fn(unsigned int K){
    return K%DEFAULT_MAX_HASH;
}

bool set_contains(char*& value){
	for (int i = 0; i < set_size; i++) {
        char* this_word = word_set[i];
		if (strcmp(this_word,value) == 0) {
			return true;
		}
	}
	return false;
}

bool set_insert(char*& value, int num_chars){
	if (set_contains(value) == false) {
		if ((set_size + 1) > DEFAULT_MAX_ITEMS) {
			return false;
		}
		set_size++;
		word_set[set_size - 1] = (char *) malloc(sizeof(char)*num_chars);
        strcpy(word_set[set_size - 1],value);
		return true;
	}
	else {
		return false;
	}
}

void merge_int(int*& int_arr, int left, int middle, int right){
    int i, j, k;
    int n1 = middle-left+1;
    int n2 = right-middle;

    int L[n1];
    int R[n2];
    for(i = 0; i< n1;i++){
        L[i] = int_arr[left+i];
    }
    for(j = 0; j< n2;j++){
        R[i] = int_arr[right+j];
    }

    i=0;
    j=0;
    k = left;
    while(i<n1 && j<n2){
        if(L[i]<=R[j]){
            int_arr[k] = L[i];
            i++;
        }
        else{
            int_arr[k] = R[j];
            j++;
        }
        k++;
    }

    while(i<n1){
        int_arr[k] = L[i];
        i++;
        k++;
    }

    while(j<n2){
        int_arr[k] = R[j];
        j++;
        k++;
    }
}

void merge_sort_int(int*& int_arr, int left, int right){
    if(left < right){
        int middle = left + (right-left)/2;
        merge_sort_int(int_arr,left,middle);
        merge_sort_int(int_arr,middle+1,right);
        merge_int(int_arr, left, middle, right);

    }
}

void merge_strarr(char**& str_arr, int left, int middle, int right){
    int i, j, k;
    int n1, n2;
    // if((right-left)%2==0){
    //     n1 = middle-left+1; //1
    //     n2 = right-middle; //1
    // }
    // else{
    //     n1 = middle-left; //1
    //     n2 = right-middle+1; //1
    // }

    n1 = middle-left+1; //1
    n2 = right-middle; //1

    char** L = (char**) malloc(sizeof(char*)*n1);
    char** R = (char**) malloc(sizeof(char*)*n2);
    for(i = 0; i < n1;i++){
        L[i] = (char *) malloc(sizeof(char)*201);
        strcpy(L[i],str_arr[left+i]);
    }
    for(j = 0; j< n2;j++){
        R[j] = (char *) malloc(sizeof(char)*201);
        // if((right-left)%2==0){
        //     strcpy(R[j],str_arr[right+j]);
        // }
        // else{
        //     strcpy(R[j],str_arr[middle+j+1]);
        // }
        strcpy(R[j],str_arr[middle+1+j]);
    }

    i=0;
    j=0;
    k = left;
    while(i<n1 && j<n2){
        if(strcmp(L[i],R[j])<=0){
            strcpy(str_arr[k],L[i]);
            i++;
        }
        else{
            strcpy(str_arr[k],R[j]);
            j++;
        }
        k++;
    }

    while(i<n1){
        strcpy(str_arr[k],L[i]);
        i++;
        k++;
    }

    while(j<n2){
        strcpy(str_arr[k],R[j]);
        j++;
        k++;
    }
    
    for(i = 0; i< n1;i++){
        free(L[i]);
    }
    for(j = 0; j< n2;j++){
        free(R[j]);
    }
    free(R);
    free(L);
}

void merge_sort_strarr(char**& str_arr, int left, int right){
    if(left < right){
        int middle = (left + right)/2;
        merge_sort_strarr(str_arr,left,middle);
        merge_sort_strarr(str_arr,middle+1,right);
        merge_strarr(str_arr, left, middle, right);

    }
}

struct node {
    int data;
    node* next;
    node* prev;
};

node* word_hash[DEFAULT_MAX_ITEMS];
int hash_size = 0;

void create_list(node*& head){
    head = (node*) malloc(sizeof(node));
    head->data = -1;
    head->next = head;
    head->prev = head;
}

bool ll_contains(const node* head, const int & value) {
	for (node* p = head->next; p != head; p = p->next) { //iterating through the list checking to see if it contains value
		if (p->data == value) {
			return true;
		}
	}
	return false;
}

int ll_len(const node* head) {
    int i = 0;
	for (node* p = head->next; p != head; p = p->next) { //iterating through the list checking to see if it contains value
        i++;
	}
	return i;
}

bool add_node(node*& head, const int& value){
    if (!ll_contains(head, value)) { //if the LL set doesnt contain the value create a new node that contains the value
		node* end=head->prev; //end of current list
		node* p = (node*) malloc(sizeof(node));
		p->data = value;
		
		//setting end of list to point to p and p to point back to the old end of the list
		p->prev = end;
		end->next = p;
		
		head->prev = p; //p inserted is always at the end of the list
		p->next = head;
		return true;
	}
	else {
		return false; 
	}
}

void free_linked_list(node* head){
	for (node* p = head->next; p != head;) { //iterating through the list and deleting each node individually
		node* temp = p->next; //need to store pointer to p.next since p gets deleted
		free(p);
		p = temp;
	}
	free(head); //exits loop once p=head so still need to delete head
}

int str_len(char * string){
    int i;
    for(i=0;string[i]!='\0';i++);
    return i;
}

int main(int argc, char **argv)
{	
	int c;
	char* file_location;
    int input_flag = 0;
    FILE* input_file;

	static struct option long_options[] = {
		{"file",     optional_argument, 0,  'f' },
		{0,         0,                 0,  0 }
	};
	
	//get long commandline args
	while (1) {
		int option_index = 0;

		c = getopt_long(argc, argv, "",
				long_options, &option_index);
		
		if (c == -1)
			break;
		
		switch (c) {
			case 'f': 
				input_flag = 1;
				if(optarg){
                    int len = str_len(optarg);
                    file_location = (char *) malloc((len+1)*sizeof(char)); //+1 for extra char
					strcpy(file_location,optarg);
				}
				break;

			case '?':
			default:
				fprintf(stderr,"Unrecognized argument %s\n", argv[optind-1]);
				fprintf(stderr,"Correct line usage: ./attiano [--file=FILE_LOCATION]\n");
				exit(1);
			}
	}
    
    //any other args left which are the actual args for this program
    if (optind < argc) {
		//assuming input format of: ./attiano FILE_LOCATION
        input_flag = 1;
        int len = str_len(argv[optind]);
        file_location = (char *) malloc((len+1)*sizeof(char));
        strcpy(file_location,argv[optind]);
	}

    //open file
    if(file_location){
        if ((input_file = fopen(file_location,"r")) == NULL){
            printf("ERROR: opening file\n");
            // Program exits if the file pointer returns NULL.
            exit(1);
        }

    }

    //read from file assuming length of longest word is 200
    char* curr_word = (char*) malloc(sizeof(char)*201);
    // char line[201];
    int linenum = 1;
    unsigned int hash_input = 0;
    int word_idx = 0;
    bool inserted = true;

    int found_word = 0;
    c = 0;
    while((c = fgetc(input_file)) != EOF){
        if(!isalpha(c)){
            // if(c=='.' || c=='\'' || c==':' || c==';' || c=='?' || c=='"' || c=='!'){
            //     continue;
            // }
            if(found_word){
                curr_word[word_idx] = '\0';
                // printf("%s\n",curr_word);
                
                //add word to set
                char* ptr = curr_word;
                inserted = set_insert(ptr, word_idx);
                if(!inserted){
                    inserted = false;
                }
                //add word to hash w/ linenum
                unsigned int hash_val = hash_fn(hash_input);

                if(!word_hash[hash_val]){
                    node* new_head;
                    create_list(new_head);
                    word_hash[hash_val] = new_head;
                }
                add_node(word_hash[hash_val], linenum);
                word_idx = 0;
                hash_input = 0;
                found_word = 0;
            }
            if(c=='\n'){
                linenum++;
            }
        }
        else{
            curr_word[word_idx] = tolower(c);
            hash_input += curr_word[word_idx]; //naive as multiple diff words could add to same value, cant use curr_word pointer, would have to get a freq count of each letter and hash string w/ letter count e.g. cat = c1a1t1
            word_idx++;
            found_word = 1;

        }
    }

    // printf("printing set\n\n set_size:%d\n",set_size);

    merge_sort_strarr(word_set,0,set_size-1);

    // printf("printing SORTED set\n\n set_size:%d\n",set_size);
    //the idea was:
    //sort the set
    //for each word in set: get hashtable linkedlist of unique line numbers
    //copy linkedlist line numbers to array
    //sort array
    //print word and print each item in array with a comma inbetween
    
    int z1 = 0;
    for(z1=0;z1<set_size;z1++){
        int hash_in = 0;
        int ch = 0;
        while(word_set[z1][ch]!='\0'){
            printf("%c",word_set[z1][ch]);
            hash_in+=word_set[z1][ch];
            ch++;
        }
        // printf(" HASH:%d ",hash_in);
        printf(" ");
        unsigned int hash_v = hash_fn(hash_in);
        node* num_list = word_hash[hash_v];
        int lino = 0;
        node* p = num_list->next;
        for (; p != num_list; p = p->next) { //iterating through the list checking to see if it contains value
            lino = p->data;
            printf("%d",lino);
            if(p->next!=num_list){
                printf(",");
            }
	    }
        printf("\n");
        
    }
    //free memory
    if(input_flag){
        free(file_location);
    }
    int del_idx = 0;
    for(del_idx=0;del_idx<hash_size;del_idx++){
        free_linked_list(word_hash[del_idx]);
    }
    del_idx = 0;
    for(del_idx=0;del_idx<set_size;del_idx++){
        free(word_set[del_idx]);
    }
    free(curr_word);
}
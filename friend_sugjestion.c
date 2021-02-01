/*
		Alec Uyematsu	
		HW 6 CS305
		In this hw assignment I created a graph bassed
		on an excel document to reccomend a user new
		friends.

*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define N 7604  //largest vertex

//struct to store adjacency list
typedef struct Node{
	int dest;
	int weight;
	int current;
	struct Node* next;
}Node;

//struct to store graph
typedef struct Graph{
	Node* head[N];
}Graph;

typedef struct Friend{
	int avg_counter;
	int total;
	struct Friend* next_friend;
}Friend;

typedef struct FGraph{
	Friend* head[N];
}FGraph;

//used to get the field to read csv file
char* getfield(char* line, int num){
	char* tok;
	for(tok = strtok(line, ";"); tok && *tok; tok = strtok(NULL, ";\n")){
		if(!--num){
			return tok;
		}
	} 
	return NULL;
}




Graph* create_graph(){

	Graph* graph = (Graph*)malloc(sizeof(Graph));
	
	//initialize head pointers
	for(int i = 0; i < N; i++){
		graph->head[i] = NULL;
	}
	
	FILE* stream = fopen("abc.csv", "r");
	char line[1024];
	//add edges to directed graph
	while(fgets(line, 1024, stream) != NULL){
		char* tmp = strdup(line);
		char* str;
		str = getfield(tmp, 1);	

		int val1;
		int val2;
		int val3;
		int count = 1; //count resets to determine values

		char* pt;	
		pt = strtok(str, ","); //distinguish different chars from a ","

		//sets values of val1 val2 and val3 from csv file
		while(pt != NULL){
			int a = atoi(pt);
			//if statements look at count and assign a accordingly
			if(count == 3){
				val3 = a;
				count++;
			}
			if(count == 2){
				val2 = a;
				count++;
			}	
			if(count == 1){
				val1 = a;
				count++;
			}
			pt = strtok (NULL, ","); //moves on to the next value
		
		}
		
		int current = val1;
		int dest = val2;
		int weight = val3;
		
		Node* new_node = (Node*)malloc(sizeof(Node));
		//setting values of new_node			
		new_node->dest = dest;
		new_node->weight = weight;
		new_node->current = current;

		//point new node to current head
		new_node->next = graph->head[current];
		//point head pointer to new node
		graph->head[current] = new_node;		

	}
	return graph;
}



//for testing purposes only
void print_graph(Graph* graph){
	for(int i = 0; i < N; i++){
		Node* ptr = graph->head[i];
		while(ptr != NULL){
			printf("(%d->%d) (%d)\t", i, ptr->dest, ptr->weight);
			ptr = ptr->next; 
		}
	}
}



//figures out the average and suggjustion and prints out the result
int calculate(Graph* graph, int u){
	if(graph == NULL){
		printf("there was an error loading graph");
		exit(0);
	}
	//create new FGraphs
	FGraph* friend_graph = (FGraph*)malloc(sizeof(FGraph));
	FGraph* fof_graph  = (FGraph*)malloc(sizeof(FGraph));
	Node* unode = graph->head[u]; //create unode from given int
	//while loop to find all friend of friends
	while(unode != NULL){
		Friend* new_friend = (Friend*)malloc(sizeof(Friend));
		if(unode->weight > 0){ //if friend
			Node* friend_node = graph->head[unode->dest]; //create node to find friends of friends
			Node* check_friend = graph->head[u];//create node to check if friend of friend is friend of user
			while(friend_node != NULL){
					//while loop to see if friend of friend is friend of user
					int isfriend = 0;
					while(check_friend != NULL){
						if(check_friend->weight > 0){ //checks if friend
							if(check_friend->dest == friend_node->dest|| isfriend == 1){
								isfriend = 1; 
							}
							else{
								isfriend = 0;
							}
						}
						check_friend = check_friend->next; //iterate through loop
					}
					check_friend = graph->head[u];//resets check_friend
				//creates graph for fiends of friends		
				if(friend_node->weight > 0 && friend_node->dest != u && isfriend == 0){
					if(fof_graph->head[friend_node->dest] != NULL){
						fof_graph->head[friend_node->dest]->total += friend_node->weight; //adds weight if node already exists
						fof_graph->head[friend_node->dest]->avg_counter ++; //adds one to counter if node already exists
					}
					//if doesn't exist
					else{
						fof_graph->head[friend_node->dest] = (Friend*)malloc(sizeof(Friend));
						fof_graph->head[friend_node->dest]->total = friend_node->weight; //sets weight
						fof_graph->head[friend_node->dest]->avg_counter = 1; //sets count to 1
						
					}			      	
				}
				//iterate through loop
				friend_node = friend_node->next;		
			}
		}
		//itterates through loop
		unode = unode->next;
	}


	int sug_friend;
	int avg_score = 0;

	//deals with friends of friends
	for(int i = 0; i < N; i++){
		Friend* friendb = fof_graph->head[i];
		while(friendb != NULL){
			int tmp = (friendb->total/friendb->avg_counter); //gets average of all inputs
			//if the tmp greater then last make it new avg_score and sug_friend
			if(tmp > avg_score){ 
				sug_friend = i;
				avg_score = tmp;
				
			}
		friendb = friendb->next_friend;
		}
	}
	//gives message to user on suggjusted user and average 
	if(avg_score <= 0){
		printf("sorry but no suggestions for user %d\n\n", u);
	}
	else{
	printf("for user %d:\nthe suggested is user: %d has an average score of:  %d\n\n",u, sug_friend, avg_score);
	}
}



int main(){
	Graph* graph = create_graph();
//	print_graph(graph);
	calculate(graph, 413);
	calculate(graph, 93);
	calculate(graph, 705);
	return 0;
}

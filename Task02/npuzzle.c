#include<stdio.h>
#include<stddef.h>
#include<stdlib.h>
#include<assert.h>
#include<time.h>


// n * n - 1 Puzzle
int n;
static FILE * outFile = NULL;

//A structure for holding states
struct state{
	int config[10][10];			//Board configuration
	int g_cost;					//g_cost of the state
	int h_cost;					//h_cost of the state
	int allowed_operators[4];	//Left, Right, Up, Down (in sequence)
	struct state* next;
	struct state* parent;
};

struct state* states;
long expanded_states;
long generated_states;

void get_allowed_operators(struct state* conf);
int goal_test(struct state* conf, struct state* goal);
void enqueue(struct state* conf);
struct state* dequeue();
void move_left(struct state* conf, struct state* goal);
void move_right(struct state* conf, struct state* goal);
void move_up(struct state* conf, struct state* goal);
void move_down(struct state* conf, struct state* goal);
int calculate_manhattan_distance(struct state* conf, struct state* goal, int r_conf, int c_conf);
int calculate_h_cost(struct state* conf, struct state* goal);
void create_solution(struct state* init, struct state* conf);
void print_solution(struct state* conf);

//A function to get allowed operators for a state
void get_allowed_operators(struct state* conf){
	int i, j, r=0, c=0;

	for(i=0;i<4;i++)
		conf->allowed_operators[i]=1;

	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(conf->config[i][j]==0){
				r=i;
				c=j;
			}
		}
	}

	if(r==0)
		conf->allowed_operators[2]=0;
	if(r==n-1)
		conf->allowed_operators[3]=0;
	if(c==0)
		conf->allowed_operators[0]=0;
	if(c==n-1)
		conf->allowed_operators[1]=0;
}

//A function to check whether a given state is a goal state
int goal_test(struct state* conf, struct state* goal){
	int i, j;
	int flag=1;

	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(conf->config[i][j]!=goal->config[i][j]){
				flag=0;
			}
		}
	}

	return flag;
}

//A function to enqueue a given state in a queue according to its f-cost
void enqueue(struct state* conf){
	struct state *temp, *temp1;

	if(states==NULL){
		states=(struct state*)malloc(sizeof(struct state));
		states=conf;
		states->next=NULL;
		return;
	}
	else if(states->next==NULL){
			states->next=conf;
			conf->next=NULL;
	}
	else{
		temp=(struct state*)malloc(sizeof(struct state));
		temp=states;
		temp1=NULL;
		while(temp!=NULL && ((conf->g_cost)+(conf->h_cost))>=((temp->g_cost)+(temp->h_cost))){
			if(temp1==NULL)
				temp1=(struct state*)malloc(sizeof(struct state));

			temp1=temp;

			if(temp->next==NULL)
				temp=NULL;
			else{
				temp=temp->next;
			}
		}
		if(temp==NULL)
			conf->next=NULL;
		else{
			conf->next=temp;
		}
		if(temp1!=NULL){
			temp1->next=conf;
		}
		else{
			conf->next=temp;
			states=conf;
		}
	}
}

//A function to dequeue the front state from the queue
struct state* dequeue(){
	struct state* temp;

	if(states==NULL){
		return NULL;
	}
	else{
		//temp=(struct state*)malloc(sizeof(struct state));

		temp=states;

		if(states->next==NULL){
			states=NULL;
		}
		else{
			states=states->next;
		}
		expanded_states++;
		return temp;
	}
}

//A function to make a left move of the blank slide
void move_left(struct state* conf, struct state* goal){
	struct state* new_state, *temp;
	int i, j, r=0, c=0, flag;

	new_state = (struct state*)malloc(sizeof(struct state));

	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			new_state->config[i][j]=conf->config[i][j];
			if(conf->config[i][j]==0){
				r=i;
				c=j;
			}
		}
	}

	new_state->config[r][c]=new_state->config[r][c-1];
	new_state->config[r][c-1]=0;

	new_state->g_cost=(conf->g_cost)+1;
	new_state->h_cost=calculate_h_cost(new_state, goal);

	new_state->next=NULL;
	get_allowed_operators(new_state);

	temp=states;

	while(temp!=NULL){
		flag=0;
		for(i=0;i<n;i++){
			for(j=0;j<n;j++){
				if(flag==0 && new_state->config[i][j]!=temp->config[i][j]){
					flag=1;
				}
			}
		}
		if(flag==0){
			return;
		}
		else{
			if(temp->next==NULL){
				temp=NULL;
			}
			else{
				temp=temp->next;
			}
		}
	}

	new_state->parent=conf;

	enqueue(new_state);
	generated_states++;
}

//A function to make a right move of the blank slide
void move_right(struct state* conf, struct state* goal){
	struct state* new_state, *temp;
	int i, j, r=0, c=0, flag;

	new_state = (struct state*)malloc(sizeof(struct state));

	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			new_state->config[i][j]=conf->config[i][j];
			if(conf->config[i][j]==0){
				r=i;
				c=j;
			}
		}
	}

	new_state->config[r][c]=new_state->config[r][c+1];
	new_state->config[r][c+1]=0;

	new_state->g_cost=(conf->g_cost)+1;
	new_state->h_cost=calculate_h_cost(new_state, goal);

	new_state->next=NULL;
	get_allowed_operators(new_state);

	temp=states;

	while(temp!=NULL){
		flag=0;
		for(i=0;i<n;i++){
			for(j=0;j<n;j++){
				if(flag==0 && new_state->config[i][j]!=temp->config[i][j]){
					flag=1;
				}
			}
		}
		if(flag==0){
			return;
		}
		else{
			if(temp->next==NULL){
				temp=NULL;
			}
			else{
				temp=temp->next;
			}
		}
	}

	new_state->parent=conf;

	enqueue(new_state);
	generated_states++;
}

//A function to make an up move of the blank slide
void move_up(struct state* conf, struct state* goal){
	struct state* new_state, *temp;
	int i, j, r=0, c=0, flag;

	new_state = (struct state*)malloc(sizeof(struct state));

	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			new_state->config[i][j]=conf->config[i][j];
			if(conf->config[i][j]==0){
				r=i;
				c=j;
			}
		}
	}

	new_state->config[r][c]=new_state->config[r-1][c];
	new_state->config[r-1][c]=0;

	new_state->g_cost=(conf->g_cost)+1;
	new_state->h_cost=calculate_h_cost(new_state, goal);
	new_state->next=NULL;

	get_allowed_operators(new_state);

	temp=states;

	while(temp!=NULL){
		flag=0;
		for(i=0;i<n;i++){
			for(j=0;j<n;j++){
				if(flag==0 && new_state->config[i][j]!=temp->config[i][j]){
					flag=1;
				}
			}
		}
		if(flag==0){
			return;
		}
		else{
			if(temp->next==NULL){
				temp=NULL;
			}
			else{
				temp=temp->next;
			}
		}
	}

	new_state->parent=conf;

	enqueue(new_state);
	generated_states++;
}

//A function to make a down move of the blank slide
void move_down(struct state* conf, struct state* goal){
	struct state* new_state, *temp;
	int i, j, r=0, c=0, flag;

	new_state = (struct state*)malloc(sizeof(struct state));

	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			new_state->config[i][j]=conf->config[i][j];
			if(conf->config[i][j]==0){
				r=i;
				c=j;
			}
		}
	}

	new_state->config[r][c]=new_state->config[r+1][c];
	new_state->config[r+1][c]=0;

	new_state->g_cost=(conf->g_cost)+1;
	new_state->h_cost=calculate_h_cost(new_state, goal);
	new_state->next=NULL;

	get_allowed_operators(new_state);

	temp=states;

	while(temp!=NULL){
		flag=0;
		for(i=0;i<n;i++){
			for(j=0;j<n;j++){
				if(flag==0 && new_state->config[i][j]!=temp->config[i][j]){
					flag=1;
				}
			}
		}
		if(flag==0){
			return;
		}
		else{
			if(temp->next==NULL){
				temp=NULL;
			}
			else{
				temp=temp->next;
			}
		}
	}

	new_state->parent=conf;

	enqueue(new_state);
	generated_states++;
}

//A function to calculate the manhattan distance of a given tile on the board
int calculate_manhattan_distance(struct state* conf, struct state* goal, int r_conf, int c_conf){
	int r_diff=0, c_diff=0, r_goal, c_goal;

	if(conf->config[r_conf][c_conf]==0){
		return 0;
	}

	for(r_goal=0;r_goal<n;r_goal++){
		for(c_goal=0;c_goal<n;c_goal++){
			if(conf->config[r_conf][c_conf]==goal->config[r_goal][c_goal]){
				r_diff = abs(r_conf-r_goal);
				c_diff = abs(c_conf-c_goal);
			}
		}
	}

	return (r_diff + c_diff);
}



/*
//Misplace heuristic function
int calculate_h_cost(struct state* conf, struct state* goal){
	int h_cost=0, r_conf, c_conf, r_goal, c_goal;

	for(r_conf=0;r_conf<n;r_conf++){
		for(c_conf=0;c_conf<n;c_conf++){
            r_goal=r_conf;
			c_goal=c_conf;
			if(conf->config[r_conf][c_conf]==0) continue;
			if(conf->config[r_conf][c_conf]!=goal->config[r_goal][c_goal]){
				h_cost += 1;
			}
		}
	}

	return h_cost;
}
*/



/*
//Manhattan distance heuristic function
int calculate_h_cost(struct state* conf, struct state* goal){
	int h_cost=0, r_conf, c_conf, r_goal, c_goal;

	for(r_conf=0;r_conf<n;r_conf++){
		for(c_conf=0;c_conf<n;c_conf++){
			r_goal=r_conf;
			c_goal=c_conf;
			if(conf->config[r_conf][c_conf]!=goal->config[r_goal][c_goal]){
				h_cost += calculate_manhattan_distance(conf, goal, r_conf, c_conf);
			}
		}
	}

	return h_cost;
}
*/




//Linear Conflict heuristic function
int calculate_h_cost(struct state* conf, struct state* goal){
	int h_cost=0, i, j, k;
	int goalRow[n*n];
	int goalCol[n*n];
	for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            int tile = goal->config[i][j];
            goalRow[tile] = i;
            goalCol[tile] = j;
        }
	}

	for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            //This part is Manhattan Distance
            int tile = conf->config[i][j];
            if(tile == 0) continue;
            h_cost += abs(i - goalRow[tile]);
            h_cost += abs(j - goalCol[tile]);

            // Two tiles I and J are in a linear conflict if I and J are
            // in the same line, the goal positions of I and J are both in
            // that line, I is to the right of J and goal position of I is
            // to the left of the goal position of J.
            if (goalRow[tile] != i) continue;
            for (k = j+1; k < n; k++) {
                if (conf->config[i][k] == 0) {
                    continue;
                }
                if (goalRow[conf->config[i][k]] == i && goalCol[conf->config[i][k]] < goalCol[tile]) {
                    h_cost += 2;
                }
            }
        }
	}
	return h_cost;
}




//A function to backtrack the solution from goal to the initial state
void create_solution(struct state* init, struct state* conf){
	if(conf==init){
		print_solution(conf);
		return;
	}

	create_solution(init, conf->parent);
	print_solution(conf);
}

//A function to print the stepwise solution
void print_solution(struct state* conf){
	static int i=0, x, y;

    fprintf(outFile, "\nstep %d: \n", i);


	for(x=0;x<n;x++){
		for(y=0;y<n;y++){
			if(conf->config[x][y]==0)
				fprintf(outFile, "0\t");
			else
				fprintf(outFile, "%d\t", conf->config[x][y]);
		}
		fprintf(outFile, "\n");
	}

	i++;
}

int main(void){
	struct state *init, *goal, *temp;
	int i, j;

	init = (struct state*)malloc(sizeof(struct state));

	//Get the initial configuration from the user
	FILE * pFile;
	pFile = fopen("npuzzle_in.txt", "r");
	outFile = fopen("npuzzle_out.txt", "w");
	fscanf(pFile, "%d", &n);
	assert(n < 10);
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			fscanf(pFile, "%d", &(init->config[i][j]));
		}
	}

	goal = (struct state*)malloc(sizeof(struct state));

	//Get the goal configuration from the user
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			fscanf(pFile, "%d", &(goal->config[i][j]));
		}
	}


	clock_t t = clock();
	//Initialize the initial state
	init->parent=init;

	init->g_cost=0;
	init->h_cost=calculate_h_cost(init, goal);

	get_allowed_operators(init);

	//Enqueue the initial state
	enqueue(init);

	//Loop
	while(1){

		//If there are no states left in the queue to expand, return failure
		if(states==NULL){
			printf("No Solution");
			return -1;
		}

		//Dequeue the front state from the queue
		temp=dequeue();

		//Check if the goal state has been reached
		if(goal_test(temp, goal)){
			create_solution(init, temp);
			fclose(outFile);
			printf("\nCost of the best solution path: %d", ((temp->g_cost)+(temp->h_cost)));
			printf("\nNumber of nodes generated: %ld", generated_states);
			printf("\nNumber of nodes expanded: %ld", expanded_states);
			t = clock() - t;
            printf ("\nIt took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
            system("pause");
			return 0;
		}

		//Make all the valid moves on the dequeued state and add the resulting states to the queue
		if(temp->allowed_operators[0])
			move_left(temp, goal);
		if(temp->allowed_operators[1])
			move_right(temp, goal);
		if(temp->allowed_operators[2])
			move_up(temp, goal);
		if(temp->allowed_operators[3])
			move_down(temp, goal);
	}

	return 0;
}

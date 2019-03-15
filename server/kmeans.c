#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define MAX_CENTROIDS 2
#define NUM_FEATURES 8
#define MAX_USERS 5
#include "extract_features.h"

//double cost_function(int X[num_books][100],int Theta[num_users][100],int r[1000][1000])//x[num_books*],theta[num_users*]
int c[MAX_USERS];
float kmeans(int num_iterations,int num_centroids,int num_users)
{
	float cost;
	extract_features();
	float u[MAX_CENTROIDS][NUM_FEATURES],min_distance=-1;
	int num_points[MAX_CENTROIDS];
	//u[0]...u[k]- centroids' coordinates
	//random initialization-cluster to which user i belongs
	//c[i] centroid of i th example
	srand(time(NULL));
	for(int k=0;k<num_centroids;k++)
	{	
		for(int i=1;i<NUM_FEATURES;i++)
		{
			u[k][i]=rand()%max_feature[i]+(float)rand()/RAND_MAX;
		}

	}
	for(int iter=0;iter<num_iterations;iter++)
	{	

		//cluster assigment
		for(int i=0;i<num_users;i++)
		{
			min_distance=-1;
			for(int k=0;k<num_centroids;k++)
			{
				float distance=0;
				for(int j=1;j<NUM_FEATURES;j++)
				{
					distance=distance+pow(X[i][j]-u[k][j],2);
				}
				distance=sqrt(distance);
				if(distance<min_distance ||min_distance==-1)
				{
					min_distance=distance;
					c[i]=k;
				}
			}
			
		}
		//moving centroid
		for(int k=0;k<num_centroids;k++)
		{
			for(int i=1;i<NUM_FEATURES;i++)
			u[k][i]=0;

		}
		for(int i=0;i<num_users;i++)
		{
			for(int j=1;j<NUM_FEATURES;j++)
			{
				u[c[i]][j]=u[c[i]][j]+X[i][j];
				num_points[c[i]]+=1;
			}
		}
		for(int i=0;i<num_centroids;i++)
		{
			for(int j=1;j<NUM_FEATURES;j++)
			{
				u[i][j]=u[i][j]/num_points[i];
			}
		}	
	}
	//calculate cost 
	cost=0;
	for(int i=0;i<num_users;i++)
	{
		float distance=0;
		for(int j=1;j<NUM_FEATURES;j++)
		{
			distance=distance+pow(X[i][j]-u[c[i]][j],2);
		}
		distance=sqrt(distance);
		//printf("distanta de la user %d=%f\n",i,distance);
		cost=cost+distance;
	}
	cost=cost/(float)(num_users);
	printf("cost:%f\n",cost);

	for(int i=0;i<num_users;i++){
		printf("User%d->%d\n",i,c[i]);
		fflush(stdout);
	}
	return cost;
}

#include <pthread.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <unistd.h>

using namespace std;

bool isFull = false;
int myMatrix[2][50];
int turn = 0; 
int seatnumber;
int remaining = 100;

void* reserveSeat1(void *){
    while(true)
    {
        
        while(turn != 0)
        {}
        cout << "Agency 1 entered Critical Region" << endl;
        int row = rand() % 2;
        int column = rand() % 50;
        //If there is a 0 element in our matrix make it 1.
        if(myMatrix[row][column] == 0)
        {
            myMatrix[row][column] = 1;
        }
        //If not continue generating random numbers until we find a zero element.
        else 
        {
            while(myMatrix[row][column]!= 0)
            {
                row = rand() % 2;
                column = rand() % 50;
            }
            myMatrix[row][column] = 1;
        }
        seatnumber = row*50 + column + 1;
        cout << "Seat number " << seatnumber << " is reserved by Agency 1" << endl;
        cout << "Agency 1 Exit Critical Region" << endl;
        cout << endl;
        turn = 1;
        remaining--;
            
    }
    return 0;
}

void* reserveSeat2(void*){

    while(true)
    {
        while(turn != 1)
        {}
        cout << "Agency 2 entered Critical Region" << endl;
        int row = rand() % 2;
        int column = rand() % 50;
        //If there is a 0 element in our matrix make it 2.
        if(myMatrix[row][column] == 0)
        {
            
             myMatrix[row][column] = 2;
             
        }
        //If not continue generating random numbers until we find a zero element.
        else 
        {
            while(myMatrix[row][column]!= 0)
            {
                row = rand() % 2;
                column = rand() % 50;
            }
            myMatrix[row][column] = 2;
        }    
        seatnumber = row*50 + column + 1;    
        cout << "Seat number " << seatnumber << " is reserved by Agency 2" << endl;
        cout << "Agency 2 Exit Critical Region" << endl;
        cout << endl;
        turn = 0;
        remaining--;
           
    }
    return 0;
}


int main(){
    srand(time(NULL));
    pthread_t travelAgency1, travelAgency2;
    int count = 0;

    //Construct and fill our matrix with 0's.
    for(int i = 0; i<2;i++)
    {
        for(int j = 0; j<50; j++)
        {
            myMatrix[i][j] = 0;
        }
    }

    pthread_create( &travelAgency1, NULL, reserveSeat1, NULL);
    pthread_create( &travelAgency2, NULL, reserveSeat2, NULL);

    //While remaining is not 0(which means our matrix has empty space) continue running the threads, otherwise break
    //the loop and print our matrix.
    while(true)
    {
       if(remaining == 0)
       {    
            cout << "No Seats Left" << endl;
            cout << endl;
            cout << "Plane is full:" << endl;
            for(int i = 0; i < 2; i++)
            {
                for(int j = 0; j< 50; j++)
                {
                    cout << myMatrix[i][j] << " ";
                    
                }
                cout << endl;
            }    
            break;
            pthread_join(travelAgency1,NULL);
            pthread_join(travelAgency2,NULL);
       }
    }

    return 0;
}
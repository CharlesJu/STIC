/*
The sensor outputs provided by the library are the raw 16-bit values
obtained by concatenating the 8-bit high and low gyro data registers.
They can be converted to units of dps (degrees per second) using the
conversion factors specified in the datasheet for your particular
device and full scale setting (gain).
Example: An L3GD20H gives a gyro X axis reading of 345 with its
default full scale setting of +/- 245 dps. The So specification
in the L3GD20H datasheet (page 10) states a conversion factor of 8.75
mdps/LSB (least significant bit) at this FS setting, so the raw
reading of 345 corresponds to 345 * 8.75 = 3020 mdps = 3.02 dps.
*/

#include <Wire.h>
#include <L3G.h>

L3G gyro;
#define A_SIZE  11
int gyroXA = 255;
int gyroYA = 0;

void quickSort(int arr[], int left, int right) {
     int i = left, j = right;
     int tmp;
     int pivot = arr[(left + right) / 2];

     /* partition */
     while (i <= j) {
           while (arr[i] < pivot)
                 i++;
           while (arr[j] > pivot)
                 j--;
           if (i <= j) {
                 tmp = arr[i];
                 arr[i] = arr[j];
                 arr[j] = tmp;
                 i++;
                 j--;
           }
     };

     /* recursion */
     if (left < j)
           quickSort(arr, left, j);
     if (i < right)
           quickSort(arr, i, right);
}


void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  if (!gyro.init())
  {
    Serial.println("Failed to autodetect gyro type!");
    while (1);
  }

  gyro.enableDefault();

  gyroXA = 255 * (A_SIZE/2 + 1);
//  gyroYA = 460 * (A_SIZE/2 + 2);
  
  delay(1000);
}

int gyroX [A_SIZE];
int gyroY [A_SIZE];



void loop() {
  gyro.read();

  //Shift one number over
  for(int i = A_SIZE - 1; i > 0; i--){
    gyroX[i] = gyroX[i - 1];
  }

  for(int i = A_SIZE - 1; i > 0; i--){
    gyroY[i] = gyroY[i - 1];
  }

  //Set the first value to current value
  gyroX[0] = (int)gyro.g.x;
  gyroY[0] = (int)gyro.g.y;

  int sortedGyroX [A_SIZE];
  int sortedGyroY [A_SIZE];

  //Set all values of gyro arrays to the sorted gyro array
  for( int i = 0 ; i < A_SIZE ; ++i ){
    sortedGyroX[ i ] = gyroX[ i ];
  }

  for( int i = 0 ; i < A_SIZE ; ++i ){
    sortedGyroY[ i ] = gyroY[ i ];
  }

  //Sort the sorted arrays
  quickSort(sortedGyroX, 0, A_SIZE);
  quickSort(sortedGyroY, 0, A_SIZE);

  //Print out the values


  if(sortedGyroX[A_SIZE/2] < 250 || sortedGyroX[A_SIZE/2] > 265){
    gyroXA += sortedGyroX[A_SIZE/2] - 257;
  }
  
  gyroYA += sortedGyroY[A_SIZE/2] - 460;
  
  Serial.print("X:    ");Serial.print(sortedGyroX[A_SIZE/2]);
  Serial.print(" "); Serial.print(gyroXA);
  Serial.print("  Y:    ");Serial.print(sortedGyroY[A_SIZE/2]);
    Serial.print(" "); Serial.print(gyroYA);
  Serial.print("  A0:   ");Serial.println(analogRead(0));
  
  delay(100);
}

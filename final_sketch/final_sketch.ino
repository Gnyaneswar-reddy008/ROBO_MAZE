
//All time constants 
const float delayleft = 370;//415 ; 
const float delayright =  370;//390;
const float delayuturn =700;//700 ;
const int fit_time = 60;//630; //time delay after sensing black tape

int dist = 23;
int inputValue = 0;
int arr[10];
int i=0;
int orient,x;
int distance_front,distance_left,distance_right,duration_front,duration_left,duration_right;

//Some position variables
int cur_row=1 ;
int cur_col=1;
int end_row ;
int end_col ;
int tick =1;

// all pins to arduino and components
int motor_lA = 4;
int motor_lB = 5;
int motor_enableA = 11; 

int motor_rA = 2
;
int motor_rB = 3;
int motor_enableB = 10;

int trigger_front = A2;
int echo_front = A3;

int trigger_left = A4;
int echo_left = A5;

int trigger_right = A0;
int echo_right = A1;
const int ir_sensor = 9;

int irvalue;
int stage =1;
int check = 0;
int maze[15][15]= {{-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2},          //Maze initially
                   {-2, 0,-2, 0,-2, 0,-2, 0,-2, 0,-2, 0,-2, 0,-2},
                   {-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2},
                   {-2, 0,-2, 0,-2, 0,-2, 0,-2, 0,-2, 0,-2, 0,-2},
                   {-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2},
                   {-2, 0,-2, 0,-2, 0,-2, 0,-2, 0,-2, 0,-2, 0,-2},
                   {-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2},
                   {-2, 0,-2, 0,-2, 0,-2, 0,-2, 0,-2, 0,-2, 0,-2},
                   {-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2},
                   {-2, 0,-2, 0,-2, 0,-2, 0,-2, 0,-2, 0,-2, 0,-2},
                   {-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2},
                   {-2, 0,-2, 0,-2, 0,-2, 0,-2, 0,-2, 0,-2, 0,-2},
                   {-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2},
                   {-2, 0,-2, 0,-2, 0,-2, 0,-2, 0,-2, 0,-2, 0,-2},
                   {-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2}};


void setup() {
  // put your setup code here, to run once:
  pinMode(motor_lA,OUTPUT);   //left motors forward
  pinMode(motor_lB,OUTPUT);   //left motors reverse
  pinMode(motor_enableA, OUTPUT);

  pinMode(motor_rA,OUTPUT);   //right motors forward
  pinMode(motor_rB,OUTPUT);   //rignt motors reverse
  pinMode(motor_enableB, OUTPUT);

  pinMode(trigger_front,OUTPUT);
  pinMode(echo_front,INPUT);

  pinMode(trigger_left,OUTPUT);
  pinMode(echo_left,INPUT);

  pinMode(trigger_right,OUTPUT);
  pinMode(echo_right,INPUT);
  
  analogWrite(motor_enableA,233.8); // we have used analog write as we need to control the speed of motor
  analogWrite(motor_enableB, 255);
  pinMode(ir_sensor,INPUT);

  arr[0] = 3;
  i++;
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  distance_calculation();
  delay(10);
   
 Serial.print("front = ");
  Serial.println(distance_front);
  Serial.print("Left = "); 
  Serial.println(distance_left);
  Serial.print("Right = ");
  Serial.println(distance_right);

  
  irvalue = digitalRead(ir_sensor);
  Serial.println(irvalue);

  if (stage == 1){
    distance_calculation();
    delay(10);
        distance_calculation();
    delay(10);
        distance_calculation();
    delay(10);
        distance_calculation();
    delay(10);
      irvalue = digitalRead(ir_sensor);
    if (check ==0){
      
      if(distance_left>dist){
        left();
      
      }else if (distance_front>dist){
        forward();
      }
      else if(distance_right>dist){
        right();
      }
      else{u_turn();}
      record(2);
      while(irvalue == 1){
        irvalue = digitalRead(ir_sensor);
forward();}
delay(fit_time);
      check++;
    }
    leftfollower(irvalue);
  }
  else if(stage == 2){
    
    Serial.println(cur_row);
    Serial.println(cur_col);
      
    while ((cur_row !=1)||(cur_col !=1)){
       if (check ==1){
      if(distance_left>dist){
        left();
      }else if (distance_front>dist){
        forward();
      }
      else if(distance_right>dist){
        right();
      }
      else{u_turn();}
      record(3);
      while(irvalue == 1){
          irvalue = digitalRead(ir_sensor);}
forward();
  delay(fit_time);
      check++;
    }
  
      irvalue = digitalRead(ir_sensor);
      distance_calculation();
      delay(10);
      leftfollower(irvalue);
      
    }
    if ((cur_row==1)&& (cur_col ==1)){
      Stop();
      while(orientation() != 1){
        left();
      }
      stage++;
    }
  }

  else if(stage==3){
    if (tick ==1){
      update(end_row,end_col,1);
      maze[1][1]=99;
      maze[end_row][end_col]=-3;
      tick++;
      for (int s=0;s<11;s++){
      for(int t =0;t<11;t++){
        Serial.print(maze[s][t]);
        Serial.print("\t");
      }
      Serial.println();
    }
    }
    floodfill(irvalue);

  }
  else if(stage==4){
      cur_row=end_row;
      cur_col=end_col;
      Stop();
       if (check ==2){
      if(distance_left>dist){
        left();
      }else if (distance_front>dist){
        forward();
      }
      else if(distance_right>dist){
        right();
      }
      else{u_turn();}
      record(6);
      while(irvalue == 1){
forward();
  irvalue = digitalRead(ir_sensor);}
 delay(fit_time);
      check++;
    }
   
      while((cur_row!=1)||(cur_col!=1)){
        distance_calculation();
        irvalue = digitalRead(ir_sensor);
         delay(10);
        leftfollower(irvalue);
      }
      Stop();
      stage++;
  }

   else if (stage == 5){
    
   
    distance_calculation();
    delay(10);
    if (check ==3){
      if(distance_left>dist){
        left();
      }else if (distance_front>dist){
        forward();
      }
      else if(distance_right>dist){
        right();
      }
      else{u_turn();}
      record(3);
      while(irvalue == 1){
forward();
  irvalue = digitalRead(ir_sensor);}
delay(fit_time);
      check++;
    }
    distance_calculation();
     irvalue = digitalRead(ir_sensor);
    rightfollower(irvalue);
  }
  else if(stage == 6){
    Stop();
    
   
    distance_calculation();
    delay(10);
    if (check ==4){
      if(distance_left>dist){
        left();
      }else if (distance_front>dist){
        forward();
      }
      else if(distance_right>dist){
        right();
      }
      else{u_turn();}
      record(4);
      while(irvalue == 1){
forward();  irvalue = digitalRead(ir_sensor);}
delay(fit_time);

      check++;
    }
    while ((cur_row !=1)||(cur_col !=1)){
      distance_calculation();
       irvalue = digitalRead(ir_sensor);
        delay(10);
      rightfollower(irvalue);
    }
    if ((cur_row==1)&& (cur_col ==1)){
      Stop();
      while(orientation() != 2){
        left();
      }
      stage++;
    }
  }

  else if(stage==7){
    if(tick == 2){
    update(end_row,end_col,1);}
    maze[1][1]=99;
    maze[end_row][end_col]=-3;
    
    tick++;
      for (int s=0;s<11;s++){
      for(int t =0;t<11;t++){
        Serial.print(maze[s][t]);
        Serial.print("\t");
      }
      Serial.println();
    }
    floodfill(irvalue);

  }
  else if(stage==8){
      cur_row=end_row;
      cur_col=end_col;
      Stop();
      while((cur_row!=1)||(cur_col!=1)){
         if (check ==4){
      if(distance_left>dist){
        left();
      }else if (distance_front>dist){
        forward();
      }
      else if(distance_right>dist){
        right();
      }
      else{u_turn();}
      record(5);
      while(irvalue == 1){
forward();
  irvalue = digitalRead(ir_sensor);}
delay(fit_time);
      check++;
    }
        irvalue = digitalRead(ir_sensor);
        distance_calculation();
        delay(10);
        rightfollower(irvalue);
      }
      Stop();
      stage++;
  }
  else if(stage==9){
    if(tick==3){
    update(end_row,end_col,1);}
    maze[1][1]=99;
    maze[end_row][end_col]=-3;
    tick++;
      for (int s=0;s<11;s++){
      for(int t =0;t<11;t++){
        Serial.print(maze[s][t]);
        Serial.print("\t");
      }
      Serial.println();
    }
    
    floodfill(irvalue);

  }
  else if(stage==10){
      cur_row=end_row;
      cur_col=end_col;
      Stop();
      while((cur_row!=1)||(cur_col!=1)){
        distance_calculation();
        irvalue = digitalRead(ir_sensor); if (check ==4){
      if(distance_left>dist){
        left();
      }else if (distance_front>dist){
        forward();
      }
      else if(distance_right>dist){
        right();
      }
      else{u_turn();}
      record(4);
      while(irvalue == 1)
forward();

      check++;
    }
         delay(10);
        rightfollower(irvalue);
      }
      Stop();
      delay(10000);
      stage++;
  }          /*
  if (tick ==1){
    update(end_row,end_col,1);
    maze[end_row][end_col]=-3;
    delay(1000);
    tick++;
    for (int s=0;s<11;s++){
      for(int t =0;t<11;t++){
        Serial.print(maze[s][t]);
        Serial.print("\t");
      }
      Serial.println();
    }
  }
  
  floodfill(irvalue);

*/
  //leftfollower(irvalue);

 /* for (int s=0;s<11;s++){
      for(int t =0;t<11;t++){
        Serial.print(maze[s][t]);
        Serial.print("\t");
      }
      Serial.println();
  }

  delay(5000);*/
  delay(60);
}


//All functions declared and defined here


void straight()
{
   arr[i]=0;
   i++;
}

void right(){
  
  arr[i]=1;
    i++;
    digitalWrite(motor_lA,1);
    digitalWrite(motor_lB,0);
    digitalWrite(motor_rA,0);
    digitalWrite(motor_rB,1);
    delay(delayright);
   
}
void righty(){
  

    digitalWrite(motor_lA,1);
    digitalWrite(motor_lB,0);
    digitalWrite(motor_rA,0);
    digitalWrite(motor_rB,1);
    delay(100);
   
}


void left(){
  digitalWrite(motor_lA,0);
  digitalWrite(motor_lB,1);
  digitalWrite(motor_rA,1);
  digitalWrite(motor_rB,0);
   arr[i]=3;
    i++;
  delay(delayleft);
  
}
void lefty(){
  digitalWrite(motor_lA,0);
  digitalWrite(motor_lB,1);
  digitalWrite(motor_rA,1);
  digitalWrite(motor_rB,0);
   delay(100);
  
  
}


void Stop(){
  digitalWrite(motor_lA,0);
  digitalWrite(motor_lB,0);
  digitalWrite(motor_rA,0);
  digitalWrite(motor_rB,0);
  delay(3000);
}
void u_turn()
{
 digitalWrite(motor_lA,0);
    digitalWrite(motor_lB,1);
    digitalWrite(motor_rA,1);
    digitalWrite(motor_rB,0);
   arr[i]=2;
    i++;
  delay(delayuturn);
  
}
void forward()
{
  digitalWrite(motor_lA,1);
  digitalWrite(motor_lB,0);
  digitalWrite(motor_rA,1);
  digitalWrite(motor_rB,0);
  
 
}
void distance_calculation()
{

   
  digitalWrite(trigger_front, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_front, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_front, LOW);
  duration_front = pulseIn(echo_front, HIGH,10000);
  distance_front= duration_front*0.034/2;
  digitalWrite(trigger_left, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_left, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_left, LOW);
  duration_left = pulseIn(echo_left, HIGH);
  distance_left= duration_left*0.034/2;
  
  digitalWrite(trigger_right, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_right, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_right, LOW);
  duration_right = pulseIn(echo_right, HIGH);
  distance_right= duration_right*0.034/2;
}

int orientation(){
  int orient =0;
  int j;
  Serial.print("arr = ");
  for (j=i;j>=0;j--){
    orient += arr[j];
    Serial.print(arr[j]);
}
  Serial.println();
  orient %=4;
  return orient;
}


void record(int x){
  int orient = (orientation() )%4;
  if (orient ==0){
    maze[cur_row-1][cur_col]=-1;
    cur_row-=2;
  }else if  (orient ==1){
    maze[cur_row][cur_col+1]=-1;
    cur_col+=2;
  }else if  (orient ==2){
    maze[cur_row+1][cur_col]=-1;
    cur_row += 2;
  }else  {
    maze[cur_row][cur_col-1]=-1;
    cur_col-=2;
  } 


}

void leftfollower(int irvalue)
{
  
  if (distance_left > dist)
  {
    left();
    while(irvalue!=1){
      forward();
      irvalue = digitalRead(ir_sensor);
      delay(10);
    }
    record(3);
    forward();
    delay(fit_time);
  }
  else if (distance_front>dist )//|| distance_front==false )
  {
    straight();
    while(irvalue!=1){
      forward();
      irvalue = digitalRead(ir_sensor);
      delay(10);
    }
    record(0);
    forward();
    delay(fit_time);
  }
  else if (distance_front<dist && (distance_right>dist) && distance_left <dist)
  {
    right();
  while((irvalue!=1)||((cur_row ==1)&& (cur_col ==2))){
      forward();
      irvalue = digitalRead(ir_sensor);

      delay(10);
    }
    record(1);
    forward();
    delay(fit_time);
  }
  
  else if(distance_left<dist && distance_front<dist && distance_right<dist) {
    u_turn();
  while(irvalue!=1){
      forward();
      irvalue = digitalRead(ir_sensor);
      delay(10);
    }
    record(2);
    forward();
    delay(fit_time);
  }
  else {forward();
  }
  irvalue = digitalRead(ir_sensor);
  if ((irvalue == 1)&&((cur_row !=1)||(cur_col!=1))){
     stage++;
     end_row = cur_row;
     end_col = cur_col;
     Stop();
  }

  
}

void rightfollower(int irvalue)
{
  if (distance_right > dist)
  {
    right();
    while(irvalue!=1){
      forward();
      irvalue = digitalRead(ir_sensor);
      delay(10);
    }
    record(1);
    forward();
    delay(fit_time);
  }
  else if (distance_front>dist )
  {
    straight();
    while(irvalue!=1){
      forward();
      irvalue = digitalRead(ir_sensor);
      delay(10);
    }
    record(0);
    forward();
    delay(fit_time);
  }
  else if (distance_front<dist && (distance_left>dist) && distance_right <dist)
  {
    left();
  while(irvalue!=1){
      forward();
      irvalue = digitalRead(ir_sensor);

      delay(10);
    }
    record(1);
    forward();
    delay(fit_time);
  }
  
  else if(distance_left<dist && distance_front<dist && distance_right<dist) {
    u_turn();
  while(irvalue!=1){
      forward();
      irvalue = digitalRead(ir_sensor);
      delay(10);
    }
    record(2);
    forward();
    delay(fit_time);
  }
  else {forward();
  }
  if (irvalue == 1){
     stage++;
     end_row = cur_row;
     end_col = cur_col;
  }

  
}

int floodfill(int irvalue){
    int r =100,l=100,u=100,d=100,min_val = INT8_MAX;
    int cur_dir;
    cur_dir = orientation();
    Serial.print("cur_dir =  ");
    Serial.println(cur_dir);   
   
    if ((cur_row>1)&&(maze[cur_row-1][cur_col]==-1)){
        u = maze[cur_row-2][cur_col];
        min_val=min(min_val,u);
 
    }
    if ((cur_col>1)&&(maze[cur_row][cur_col-1]==-1)){
        l = maze[cur_row][cur_col-2];
        min_val=min(min_val,l);

    }
    if ((cur_row<13)&&(maze[cur_row+1][cur_col]==-1)){
        d = maze[cur_row+2][cur_col];
        min_val=min(min_val,d);
    }
    if (( cur_col<13)&&(maze[cur_row][cur_col+1]==-1)){
        r = maze[cur_row][cur_col+2];
        min_val=min(min_val,r);

    }
    Serial.print("min value  ");
    Serial.println(min_val);

    if(cur_dir==0){
      if(min_val==l){
         left();
         cur_col -= 2;
      Serial.println("left");
      while(irvalue!=1){
        forward();
        irvalue = digitalRead(ir_sensor);
        delay(10);
      }
      forward();
      delay(fit_time);
      }
      else if(min_val==r){
         right();
          cur_col += 2;
      Serial.println("right");
      while(irvalue!=1){
        forward();
        irvalue = digitalRead(ir_sensor);
        delay(10);
      }
      forward();
      delay(fit_time);
      }
      else if(min_val==u){
         cur_row -= 2;
      Serial.println("forward");
      while(irvalue!=1){
        forward();
        irvalue = digitalRead(ir_sensor);
        delay(10);
      }
      forward();
      delay(fit_time);
      }
      else if(min_val==d){
         u_turn();
         cur_row += 2;
      Serial.println("uturn");
      while(irvalue!=1){
        forward();
        irvalue = digitalRead(ir_sensor);
        delay(10);
      }
      forward();
      delay(fit_time);
      }
    }

    else if(cur_dir==1){
      if(min_val==l){
         u_turn();
         cur_col -= 2;
      Serial.println("uturn");
      while(irvalue!=1){
        forward();
        irvalue = digitalRead(ir_sensor);
        delay(10);
      }
      forward();
      delay(fit_time);
      }
      else if(min_val==r){
         cur_col +=2;
      Serial.println("forward");
      while(irvalue!=1){
        forward();
        irvalue = digitalRead(ir_sensor);
        delay(10);
      }
      forward();
      delay(fit_time);
      }
      else if(min_val==u){
         left();
         cur_row -=2;
      Serial.println("left");
      while(irvalue!=1){
        forward();
        irvalue = digitalRead(ir_sensor);
        delay(10);
      }
      forward();
      delay(fit_time);
      }
      else if(min_val==d){
         right();
         cur_row += 2;
      Serial.println("right");
      while(irvalue!=1){
        forward();
        irvalue = digitalRead(ir_sensor);
        delay(10);
      }
      forward();
      delay(fit_time);
      }}

     else  if(cur_dir==2){
      if(min_val==l){
         right();
         cur_col -=2;
      Serial.println("right");
      while(irvalue!=1){
        forward();
        irvalue = digitalRead(ir_sensor);
        delay(10);
      }
      forward();
      delay(fit_time);
      }
      else if(min_val==r){
         left();
         cur_col+=2;
      Serial.println("left");
      while(irvalue!=1){
        forward();
        irvalue = digitalRead(ir_sensor);
        delay(10);
      }
      forward();
      delay(fit_time);
      }
      else if(min_val==u){
         u_turn();
         cur_row -= 2;
      Serial.println("uturn");
      while(irvalue!=1){
        forward();
        irvalue = digitalRead(ir_sensor);
        delay(10);
      }
      forward();
      delay(fit_time);
      }
      else if(min_val==d){
        cur_row += 2;
      Serial.println("forward");
      while(irvalue!=1){
        forward();
        irvalue = digitalRead(ir_sensor);
        delay(10);
      }
      forward();
      delay(fit_time);
      }
       }
      else  if(cur_dir==3){
      if(min_val==l){
         cur_col -=2;
      Serial.println("forward");
      while(irvalue!=1){
        forward();
        irvalue = digitalRead(ir_sensor);
        delay(10);
      }
      forward();
      delay(fit_time);
      }
      else if(min_val==r){
         u_turn();
         cur_col+=2;
      Serial.println("uturn");
      while(irvalue!=1){
        forward();
        irvalue = digitalRead(ir_sensor);
        delay(10);
      }
      forward();
      delay(fit_time);
      }
      else if(min_val==u){
         right();
         cur_row -= 2;
      Serial.println("right");
      while(irvalue!=1){
        forward();
        irvalue = digitalRead(ir_sensor);
        delay(10);
      }
      forward();
      delay(fit_time);
      }
      else if(min_val==d){
         left();
         cur_row += 2;
      Serial.println("left");
      while(irvalue!=1){
        forward();
        irvalue = digitalRead(ir_sensor);
        delay(10);
      }
      forward();
      delay(fit_time);
      }}
if(min_val==-3){
  Stop();
  stage++;
  return 0;
}

}

int update(int row,int col,int count){
    if (row ==1 && col==1){
        maze[1][1] = 99;
         return 0;}
          
    bool check =0;
    if ((row < 13)&&(maze[row+1][col] == -1)&&((maze[row+2][col] > count)||(maze[row+2][col]==0))){
        maze[row+2][col] = count;
        check =1;
        update(row+2,col,count+1);
    }
    if ((row > 1)&&(maze[row-1][col] == -1)&&((maze[row-2][col] >count)||(maze[row-2][col]==0))){
        maze[row-2][col] = count;
        check=1;
        update(row-2,col,count+1);
    }
    if ((col < 13)&&(maze[row][col+1] == -1)&&((maze[row][col+2] >count)||(maze[row][col+2]==0))){
        maze[row][col+2] = count;
        check =1;
        update(row,col+2,count+1);
    }
    if ((col>1)&&(maze[row][col-1] == -1)&&((maze[row][col-2] > count)||(maze[row][col-2]==0))){
        maze[row][col-2] = count;
        check =1;
        update(row,col-2,count+1);
    
    }

   
    if (check ==0)return 0;
}







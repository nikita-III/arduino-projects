#include <Servo.h>
#include <Ultrasonic.h>
int ud[9], v0=50, l[9]={21, 45, 21, 37, 15, 41, 7, 15, 53}, mc[8], d[9]={5, 15, 5, 20, 3, 10, 2, 3, 10}, dif[9]={21, 45, 21, 37, 15, 41, 7, 15, 53}, edgestate, objboustate, movec[3]={100, 100, 100}, timpas=0, timcon;
//char [1], [1];//={'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'}
//Ultrasonic us[9]={(36, 37), (38, 39), (40, 41), (42, 43), (44, 45), (46, 47), (48, 49), (50, 51), (52, 53)};
Ultrasonic us1(36, 37);
Ultrasonic us2(38, 39);
Ultrasonic us3(40, 41);
Ultrasonic us4(42, 43);
Ultrasonic us5(44, 45);
Ultrasonic us6(46, 47);
Ultrasonic us7(48, 49);
Ultrasonic us8(50, 51);
Ultrasonic us9(52, 53);
Servo sm[3];

void setup()
{
  Serial.begin(9600);
  //bounding values of out/in pins to the elements of char array (initialising ultrasonic sensors along with pins 36-53).
  for(auto i=36; i<53; i+=2)
  {
    //Ultrasonic us[((((i / 4) - 9) * 2) + ((i%4>0) ? (i%4/2) : 0))](i, i+1);
    pinMode (i, OUTPUT);
    pinMode (i+1, INPUT);
  }
  //defining out pins for motor controllers and servos.
  for(auto i=2; i<10; i++)
  {
    mc[i-2]=i;
    pinMode (i, OUTPUT);
  }
  for(auto i=0; i<3; i++)
  {
    sm[i].attach(i+10);
  }
}

void Forward (int s)
{
digitalWrite (mc[0], HIGH);
digitalWrite (mc[1], HIGH);
digitalWrite (mc[2], s);
digitalWrite (mc[3], s);
}

void Backward (int s)
{
digitalWrite (mc[0], LOW);
digitalWrite (mc[1], LOW);
digitalWrite (mc[2], s);
digitalWrite (mc[3], s);
}

void Left (int s)
{
digitalWrite (mc[0], HIGH);
digitalWrite (mc[1], LOW);
digitalWrite (mc[2], s);
digitalWrite (mc[3], s);
}

void Right (int s)
{
digitalWrite (mc[0], LOW);
digitalWrite (mc[1], HIGH);
digitalWrite (mc[2], s);
digitalWrite (mc[3], s);
}

void Grab()
{
/*sm[1].write(0);
digitalWrite (30, LOW);
digitalWrite (31, HIGH);
digitalWrite (32, LOW);
digitalWrite (33, HIGH);
delay(100);
digitalWrite (33, LOW);
sm[0].write(180);
sm[2].write(180);
digitalWrite (32, HIGH);
digitalWrite (33, LOW);
delay(100);
digitalWrite (32, LOW);
sm[0].write(0);
sm[2].write(0);
digitalWrite (30, LOW);
digitalWrite (31, LOW);
sm[1].write(90);*/
}

/*void Approve()
{
  ufl0=usfl0.read();
  ufr0=usfr0.read();
  for(auto i=0; i<10&&(ufl0<lfl0-d||lfr0+d<ufr0); i++)
  {
    Right(v0);
    ufl0=usfl0.read();
    ufr0=usfr0.read();
  }
  for(auto i=0; i<10&&(ufr0<lfr0-d||lfl0+d<ufl0); i++)
  {
    Left(v0);
    ufl0=usfl0.read();
    ufr0=usfr0.read();
  }
  ufl0=usfl0.read();
  ufr0=usfr0.read();
  if(ufl0<lfl0-d&&ufr0<lfr0-d)
  {
     for(auto i=0; i<20; i++)
    {
      Right(v0);
    }
  }
  else
  {
    Grab();
  }
}*/

bool ae(float a, float b, float d)
{
  return ((b-d<=a)&&(a<=b+d));
}

int ob(float a, float b, float d)
{
  if((b-d<=a)&(a<=b+d))
  return 0;
  if(a>b+d)
  return 1;
  if(a<b-d)
  return -1;
}

int objbou()//not contextual function
{
  if((ud[0]>l[0])&(ud[2]>l[2])&((ob(ud[1],l[1], d[1])==-1)|(ob(ud[3], l[3], d[1])==-1))) //object is in front of the machine
  return 1;
  if((ud[4]>l[4])&(ob(ud[5], l[5], d[5])==-1)) //object is to the right of the machine
  return 2;
  if((ud[7]>l[7])&(ob(ud[8], l[8], d[8])==-1))//object is to the left of the machine
  return 3;
  //if(()()()()()()()())
  return 0;
}

int cloobjbou()//contextual function
{
  if((ud[0]>l[0])&(ud[2]>l[2])&((ob(ud[1],l[1], d[1])==0)&(ob(ud[3], l[3], d[3])==0))) //object is in front of the machine
  return 1;
  if((ud[0]>l[0])&(ud[2]>l[2])&((ob(ud[1],l[1], d[1])==0)&(ob(ud[3], l[3], d[3])==-1))) //object is to the right of the machine
  return 2;
  if((ud[0]>l[0])&(ud[2]>l[2])&((ob(ud[1],l[1], d[1])==-1)&(ob(ud[3], l[3], d[3])==0)))//object is to the left of the machine
  return 3;
  if((ud[0]<=l[0])|(ud[2]<=l[2])|((ob(ud[1],l[1], d[1])==1)|(ob(ud[3], l[3], d[3])==1)))//the machine is facing some edge
  return 4;
  return 0;//the object can't be grabbed by the machine
}

int edg()
{
  if((ud[0]>l[0])&ae(ud[1], l[1], d[1])&(ud[2]>l[2])&ae(ud[3], l[3], d[3])&ae(ud[7], l[7], d[7])) //free space
  return 0;
  if(((ob(ud[3], l[3], d[3])==1)|(ob(ud[1], l[1], d[1])==1)|(ud[2]<l[2])|ud[0]<l[0])&ae(ud[7], l[7], d[7])) //edge is in front of machine
  return 1;
  if((ud[0]>l[0])&ae(ud[1], l[1], d[1])&(ud[2]>l[2])&ae(ud[3], l[3], d[3])&(ob(ud[7], l[7], d[7])==1)) //edge is behind machine
  return 2;
  if((((ud[0]>l[0])|(ud[2]>l[2]))|((ob(ud[1], l[1], d[1])==1)|(ob(ud[3], l[3], d[3])==1)))&(ob(ud[7], l[7], d[7])==1)) //machine is at the edge
  return 3;
  //return 4;
}

void Aprandgra(int side)
{
  switch(side)
  {
    case(1):
    {
      if(cloobjbou()==2)
      {
        while(cloobjbou()!=3)
        Right(v0);
        while(cloobjbou()!=2)
        Left(v0);
        if(cloobjbou()==0)
        {
          for(auto i=0; i<movec[0]; i++)
          Backward(v0);
          for(auto i=0; i<movec[1]*1.5; i++)
          Right(v0);
        }
        else
        {
          Right(v0);
          Forward(v0);
          Grab();
        }
        return;
      }
      if(cloobjbou()==3)
      {
        while(cloobjbou()!=2)
        Left(v0);
        while(cloobjbou()!=3)
        Right(v0);
        if(cloobjbou()==0)
        {
          for(auto i=0; i<movec[0]; i++)
          Backward(v0);
          for(auto i=0; i<movec[1]*1.5; i++)
          Right(v0);
        }
        else
        {
          Left(v0);
          Forward(v0);
          Grab();
        }
        return;
      }
      if(cloobjbou()==0)
      {
        for(auto i=0; i<movec[0]; i++)
        Backward(v0);
        for(auto i=0; i<movec[1]*1.5; i++)
        Right(v0);
      }
      if(cloobjbou()==1)
      {
        for(auto i=0; i<movec[1];i++)
        {
          Right(v0);
          if(cloobjbou()==2)
            Aprandgra(2);
        }
        for(auto i=0; i<movec[1];i++)
        {
          Left(v0);
          if(cloobjbou()==3)
            Aprandgra(3);
        }
      }
      return;
    }
    case(2):
    {
      while(cloobjbou()!=3)
      Right(v0);
      while(cloobjbou()!=2)
      Left(v0);
      if(cloobjbou()==0)
      {
        for(auto i=0; i<movec[0]; i++)
        Backward(v0);
        for(auto i=0; i<movec[1]*1.5; i++)
        Right(v0);
      }
      else
      {
        Right(v0);
        Forward(v0);
        Grab();
      }
      return;
    }
    case(3):
    {
      while(cloobjbou()!=2)
      Left(v0);
      while(cloobjbou()!=3)
      Right(v0);
      if(cloobjbou()==0)
      {
        for(auto i=0; i<movec[0]; i++)
        Backward(v0);
        for(auto i=0; i<movec[1]*1.5; i++)
        Right(v0);
      }
      else
      {
        Left(v0);
        Forward(v0);
        Grab();
      }
      return;
    }
    default:
    {
      return;
    }
  }
}

void loop()
{
    /*Serial.print(us1.read());
    Serial.print("      ");
    Serial.print(us2.read());
    Serial.print("      ");
    Serial.print(us3.read());
    Serial.print("      ");
    Serial.print(us4.read());
    Serial.print("      ");
    Serial.print(us5.read());
    Serial.print("      ");
    Serial.print(us6.read());
    Serial.print("      ");
    Serial.print(us7.read());
    Serial.print("      ");
    Serial.print(us8.read());
    Serial.print("      ");
    Serial.println(us9.read());
    //Serial.print("      ");*/

while(1)
Forward(v0);
    
    ud[0]=us1.read();
    ud[1]=us2.read();
    ud[2]=us3.read();
    ud[3]=us4.read();
    ud[4]=us5.read();
    ud[5]=us6.read();
    ud[6]=us7.read();
    ud[7]=us8.read();
    ud[8]=us9.read();
    edgestate=edg();
    objboustate=objbou();
    if(edgestate==1)
    {
      Backward(v0);
    }
    if(edgestate==2)
    {
      Forward(v0);
    }
    if(edgestate==3)
    {
      Right(v0);
    }
    if(edgestate==0)
    {
      if(objboustate==1)
      {
        Aprandgra(1);
      }
      if(objboustate==2)
      {
        for(auto i=0; i<movec[0]; i++)
        {
          Backward(v0);
        }
        for(auto i=0; cloobjbou()!=2; i++)
        {
          Right(v0);
        }
        Right(v0);
        Aprandgra(2);
      }
      if(objboustate==3)
      {
        for(auto i=0; i<movec[0]; i++)
        {
          Backward(v0);
        }
        for(auto i=0; cloobjbou()!=3; i++)
        {
          Left(v0);
        }
        Left(v0);
        Aprandgra(3);
      }
      if(objboustate==0)
      {
        if(timpas<=timcon)
        {
          Forward(v0);
          timpas++;
        }
        else
        {
          timpas=0;
          Right(v0);
        }
      }
    }
    
}

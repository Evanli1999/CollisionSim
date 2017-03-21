// basic file operations
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
using namespace std;

//appends text to end of file
void write (char filename[],string text)
{
  ofstream myfile;
  myfile.open (filename,ios::app);
  myfile << text;
  myfile.close();
}

//appends text to end
void write (char filename[],double writeit)
{
  ofstream myfile;
  myfile.open (filename,ios::app);
  myfile << writeit;
  myfile.close();
}

//deletes all text
void clear (char filename[])
{
  ofstream myfile;
  myfile.open (filename);
  myfile << "";
  myfile.close();
}

//replaces entire file with replacewith
void clear(char filename[], string replacewith)
{
  ofstream myfile;
  myfile.open (filename);
  myfile << replacewith;
  myfile.close();	
}

//converts string to decimal
double toNum(string Text)
{
	double Number;
	if ( ! (istringstream(Text) >> Number) )
	{
		cout<<"Number convert error: "<<Text<<"\n";
		Number = 0.0;
	} 
	return Number;
}

//converts decimal to string
string fromNum(double num)
{
	string String = static_cast<ostringstream*>( &(ostringstream() << num) )->str();
	return String;
}

//finds the element separated into rows by return characters, and columns by comma characters.
string parse(char file[],int row,int col)
{
	string line,contents="";
    ifstream r(file);//open stream of the given file
    
    if (r.is_open())//can read
  	{
  		for(int a = 0;a<=row;a++)//find the right row
  		{
  			if(!r.eof())
  			getline (r,line);//read the line goes to next line
			else
			{
				cout<<"End of file reached.";
				return line;
			}
  		}
  		
  		int count = 0,pos = 0;//next we get the column based on commas
  		
  		while(count<col)
  		{
  			if (line[pos]==',')//comma means there is another element
  				count++;
  			pos++;
  		}
  		for(int len = 0;true;len++)
  		{
  			if(pos+len>line.length())
  				break;
  			else if(line[pos+len]==',')
  			{
  				return line.substr(pos,len);
  			}
  		}
  		//cout<<"Reached end of line\n";
		return line.substr(pos,line.length());
		
	}
	else
	{
		cout<<"Couldn't read file "<<file;
		return "";
	}
}
//returns the entire file of the string.
string copy(char inputfile[])
{
	string result="",temp;
	ifstream results;
	results.open (inputfile);//open stream of the given file
	
	while(true)
  	{
  		if(results.eof())
  		{
  			cout<<"Finished copy ";
  			return result;
  		}
  			
  		getline(results,temp);
  		result+=temp+"\n";
  	}
}

//get the first specific amount of lines only.
string copy(char inputfile[], int lines)
{
	string result="",temp;
	int a = 0;
	ifstream results;
	results.open (inputfile);//open stream of the given file
	
	while(a<lines)
  	{
  		if(results.eof())
  		{
  			cout<<"Finished copy ";
  			return result;
  		}
  			
  		getline(results,temp);
  		result+=temp+"\n";
  		a++;
  	}
}

int main () 
{  
	cout<<"enter input file: ";
	string akita;
	cin>>akita;
	if(akita.length()==1)
		akita = "input1.csv";
	string lastchance;
	if(toNum(parse(&akita[0u],7,0))!=1)
	{
		cout<<"\nPlease ensure that you have the values filled in for the simulation to work properly.";
		clear(&akita[0u],"pos1x,pos2x,pos1y,pos2y,vel1x,vel2x,vel1y,vel2y,timeint,mass1,mass2,spring,size1,size2\n\ntheta1,theta2,omega1,omega2,fric_const,inertia_1,inertia_2\n\nstoptime,rotation,output\n,,output.csv,\ncheck\n1,");		
		cin>>lastchance;
	}
  bool rotation = false;
  //to be calculated (mostly)
  double acc1x = 0, acc2x = 0,acc1y = 0,acc2y = 0, pos1,pos2,pos1x,pos2x,pos1y,pos2y,vel1, vel2,vel1x,vel1y,vel2x,vel2y;
  //given inital stuff
  double time = 0,stoptime = toNum(parse(&akita[0u],5,0)), timeint = toNum(parse(&akita[0u],1,8)),mass1 = toNum(parse(&akita[0u],1,9)), mass2 = toNum(parse(&akita[0u],1,10)),spring = toNum(parse(&akita[0u],1,11)),size1 = toNum(parse(&akita[0u],1,12)),size2 = toNum(parse(&akita[0u],1,13));
  //rotational variables
  double acc_ang1 = 0, acc_ang2 = 0, vel_ang1 = toNum(parse(&akita[0u],3,2)), vel_ang2 = toNum(parse(&akita[0u],3,3)),pos_ang1 = toNum(parse(&akita[0u],3,0)),pos_ang2 = toNum(parse(&akita[0u],3,1));
  //to be calculated
  double force_1x = 0, force_1y = 0, force_2x = 0, force_2y = 0;
  //to be calculated (for rotation only)
  double force_fric = 0, LoCx, LoCy, net_rotational_force = 0, rel_vel_ang = 0,rel_vel_LoC = 0, rel_vel_net = 0,torque = 0;
  //given stuff (for rotation only)
  double fric_const = toNum(parse(&akita[0u],3,4)),inertia1 = toNum(parse(&akita[0u],3,5)), inertia2 = toNum(parse(&akita[0u],3,6)); 
  
  if(toNum(parse(&akita[0u],5,1))!=0)
  	rotation = true;
  
  //output string
  string outs;
  //components for position and velocity for each object
  	pos1x = toNum(parse(&akita[0u],1,0));
	pos2x = toNum(parse(&akita[0u],1,1));
	vel1x = toNum(parse(&akita[0u],1,4));
	vel2x = toNum(parse(&akita[0u],1,5));
	pos1y = toNum(parse(&akita[0u],1,2));
	pos2y = toNum(parse(&akita[0u],1,3));
	vel1y = toNum(parse(&akita[0u],1,6));
	vel2y = toNum(parse(&akita[0u],1,7));
	//polar form if needed
  	pos1 = sqrt(pos1x*pos1x+pos1y*pos1y);
  	pos2 = sqrt(pos2x*pos2x+pos2y*pos2y);
  	vel1 = sqrt(vel1x*vel1x+vel1y*vel1y);
  	vel2 = sqrt(vel2x*vel2x+vel2y*vel2y);
  
  
  
  outs+="Collision sim Run start------------------\nInitial Conditions\nvar,mass1,mass2,size1,size2,spring const,friction coeff,inertia1,inertia2\n";
  outs+=","+fromNum(mass1)+","+fromNum(mass2),","+fromNum(size1)+fromNum(size2)+","+fromNum(spring)+","+fromNum(fric_const)+","+fromNum(inertia1)+","+fromNum(inertia2);
  outs+="\n\nCollision Chart\n";
  outs+="\nVar,pos1x,pos1y,pos2x,pos2y,vel1x,vel1y,vel2x,vel2y,acc1x,acc1y,acc2x,acc2y,force1x,force1y,force2x,force2y,theta1,theta2,omega1,omega2,alpha1,alpha2";
  outs+=fromNum(pos1x)+","+fromNum(pos1y)+","+fromNum(pos2x)+","+fromNum(pos2y)+","+fromNum(vel1x)+","+fromNum(vel1y)+",0,0,0,0,0,0,0,0,"+fromNum(pos_ang1)+","+fromNum(pos_ang2)+","+fromNum(vel_ang1)+","+fromNum(vel_ang2)+",0,0,\n";
  cout<<"initial conditions\npos1: ("+fromNum(pos1x)+" , "+fromNum(pos1y)+")\npos2: ("+fromNum(pos2x)+" , "+fromNum(pos2y)+")\nvel1: ("+fromNum(vel1x)+" , "+fromNum(vel1y)+"\n\u03b81: "+fromNum(pos_ang1)+" \u03b82: "+fromNum(pos_ang2)+"\n\u03c91: "+fromNum(vel_ang1)+" \u03c92: "+fromNum(vel_ang2)+"\nm1: "+fromNum(mass1)+" m2: "+fromNum(mass2)+"\nspr: "+fromNum(spring)+"\n\n";
      
  double over;

	over = (sqrt((pos2y-pos1y)*(pos2y-pos1y)+(pos2x-pos1x)*(pos2x-pos1x))-size1-size2);
  double distance = sqrt((pos2x-pos1x)*(pos2x-pos1x)+(pos2y-pos1y)*(pos2y-pos1y));
  
  while(true)
  {
  	//update time
  	time += timeint;
  	//variables
  	double compression = 0;
  	distance = sqrt((pos2x-pos1x)*(pos2x-pos1x)+(pos2y-pos1y)*(pos2y-pos1y));
  	if(distance<size1+size2)
  	{
  		//calculate compression and force components on each object
	  	compression =  (size1 + size2 - distance) / 2;
	  	double force_spr = compression * spring;
	  	force_1x = -1*(pos2x - pos1x)*force_spr / distance;
	  	force_2x = -1*(pos1x - pos2x)*force_spr / distance;
	  	force_1y = -1*(pos2y - pos1y)*force_spr / distance;
	  	force_2y = -1*(pos1y - pos2y)*force_spr / distance;
	  	
		  //only activates if rotation field is 1 in input
	  	
  		if(rotation)
  		{
	  		//there must be a frcitional force for this to work
	  		//apparantly, half gets used ....
	  		force_fric = 0.5 * force_spr * fric_const;
	  		
	  		//this is the line of collision, a perpendicular unit vector pointing in tangential point of collision
	  		LoCx = (pos2x-pos1x)/distance;
	  		LoCy = (-1*(pos2y-pos1y))/distance;
	  		//find relative velocities to determine direction
	  		rel_vel_ang = 1*(vel_ang2*size2 + vel_ang1*size1);
	  		rel_vel_LoC = 1*((vel2x-vel1x)*LoCx + (vel2y-vel1y)*LoCy);
	  		rel_vel_net = rel_vel_ang + rel_vel_LoC;
	  		
	  		//direction based on the net relative velocity
  			if(rel_vel_net<0)
  			{
				//forces are the fricitional forces upon that line
	  			force_1x += LoCx*force_fric;
	  			force_2x -= LoCx*force_fric;
	  			force_1y += LoCy*force_fric;
	  			force_2y -= LoCy*force_fric;
	  			net_rotational_force = force_fric;
	  		}
	  		else if(rel_vel_net>0)
	  		{
	  			force_1x -= LoCx*force_fric;
	  			force_2x += LoCx*force_fric;
	  			force_1y -= LoCy*force_fric;
	  			force_2y += LoCy*force_fric;
	  			net_rotational_force = -1*force_fric;
	  		}
	  		//if rel_vel_net == 0, do nothing;
	  		torque = 0.5*net_rotational_force * distance;
	  		// t = alpha I
	  		//angular accelerations
	  		acc_ang1 = torque / inertia1;
	  		acc_ang2 = torque / inertia2;
	  	}
		
		acc1x = force_1x / mass1;
		acc2x = force_2x / mass2;
		acc1y = force_1y / mass1;
	  	acc2y = force_2y / mass2;
  	}
  	if(distance>=size1+size2)
  	{
  		acc1x = 0;
  		acc2x = 0;
  		acc1y = 0;
  		acc2y = 0;
  		acc_ang1 = 0;
  		acc_ang2 = 0;
  	}
  	
  	//X component 1D 
  	pos1x += vel1x*timeint+0.5*acc1x*timeint*timeint;
  	pos2x += vel2x*timeint+0.5*acc2x*timeint*timeint;
  	vel1x += timeint*acc1x;
  	vel2x += timeint*acc2x; 
  	
  	//Y component 1D 
  	pos1y += vel1y*timeint+0.5*acc1y*timeint*timeint;
  	pos2y += vel2y*timeint+0.5*acc2y*timeint*timeint;
  	vel1y += timeint*acc1y;
  	vel2y += timeint*acc2y; 
  	
  	//angular component
  	pos_ang1 += vel_ang1 * timeint + 0.5*acc_ang1 * timeint * timeint;
  	pos_ang2 += vel_ang2 * timeint + 0.5 * acc_ang2 * timeint * timeint;
  	vel_ang1 += acc_ang1 * timeint;
  	vel_ang2 += acc_ang2 * timeint;
  	
  	//output
  	outs+=fromNum(time)+","+fromNum(pos1x)+","+fromNum(pos1y)+","+fromNum(pos2x)+","+fromNum(pos2y)+","+fromNum(vel1x)+","+fromNum(vel1y)+","+fromNum(vel2x)+","+fromNum(vel2y)+","+fromNum(acc1x)+","+fromNum(acc1y)+","+fromNum(acc2x)+","+fromNum(acc2y)+","+fromNum(force_1x)+","+fromNum(force_1y)+","+fromNum(force_2x)+","+fromNum(force_2y)+",";
  	outs+=fromNum(pos_ang1)+","+fromNum(pos_ang2)+","+fromNum(vel_ang1)+","+fromNum(vel_ang2)+","+fromNum(acc_ang1)+","+fromNum(acc_ang2)+"\n";
  	cout<<"time: "+fromNum(time)+"\npos1: ("+fromNum(pos1x)+" , "+fromNum(pos1y)+")\npos2: ("+fromNum(pos2x)+" , "+fromNum(pos2y)+")\nvel1: ("+fromNum(vel1x)+" , "+fromNum(vel1y)+")\nvvel2: ("+fromNum(vel2x)+" , "+fromNum(vel2y)+")\nacc1: "+fromNum(acc1x)+","+fromNum(acc1y)+"\nacc2: "+fromNum(acc2x)+","+fromNum(acc2y)+"\n";
  	cout<<"distance: "+fromNum(distance)+" compression: "+fromNum(compression)+"\ntheta1: "+fromNum(pos_ang1)+" theta2: "+fromNum(pos_ang2)+"\nomega1: "+fromNum(vel_ang1)+" omega2: "+fromNum(vel_ang2)+"\nalpha1: "+fromNum(acc_ang1)+" alpha2: "+fromNum(acc_ang2)+"\n\n";
  	
  	
  	if(time+timeint>=stoptime)
  		break;
  }
  //done
  cout<<"End time: "<<time<<endl;
  outs+="\nend.";
  //user input
  int apao;
  cin>>apao;
  write(&(parse(&akita[0u],5,2)[0u]),outs);
}



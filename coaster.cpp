#ifdef __APPLE__
#include <GLUT/glut.h> 
#else
#include <GL/glut.h> 
#endif

#include <iostream>

#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <math.h>

#include "load_and_bind_texture.h"

//create multi-dimensional array for the track
float track[][3] = {{0.6, 0.8, -0.7},
		   {0.3, 0.7, -0.65},
		   {0.2, 0.6, -0.62},
		   {-0.1, 0.65, -0.6},
		   {-0.3, 0.75, -0.5},
		   {-0.5, 0.78, -0.25},
		   {-0.52, 0.73, -0.15},
		   {-0.6, 0.65, -0.05},
		   {-0.63, 0.35, 0.15},
		   {-0.65, -0.15, 0.2},
		   {-0.67, -0.5, 0.23},
		   {-0.72, -0.63, 0.3},
		   {-0.77, -0.69, 0.5},
		   {-0.73, -0.72, 0.55},
		   {-0.68, -0.75, 0.6},
		   {-0.6, -0.76, 0.63},
		   {-0.5, -0.74, 0.56},
		   {-0.2, -0.6, 0.51},
		   {0.05, -0.4, 0.47},
		   {0.4, -0.2, 0.45},
		   {0.5, -0.15, 0.4},
		   {0.53, -0.18, 0.38},
		   {0.57, -0.26, 0.32},
		   {0.63, -0.36, 0.27},
		   {0.69, -0.39, 0.29},
		   {0.72, -0.36, 0.35},
		   {0.69, -0.3, 0.42},
		   {0.64, -0.2, 0.5},
		   {0.53, -0.05, 0.58},
		   {0.41, 0.2, 0.67},
		   {0.3, 0.45, 0.74},
		   {0.1, 0.6, 0.8},
		   {0.02, 0.65, 0.82},
		   {-0.1, 0.67, 0.74},
		   {-0.2, 0.7, 0.69},
		   {-0.35, 0.75, 0.61},
		   {-0.42, 0.8, 0.48},
		   {-0.38, 0.83, 0.36},
		   {-0.26, 0.77, 0.18},
		   {-0.1, 0.69, 0.15},
		   {0.05, 0.55, 0.13},
		   {0.1, 0.3, 0.12},
		   {0.15, -0.05, 0.12},
		   {0.19, -0.4, 0.1},
		   {0.26, -0.6, 0.08},
		   {0.31, -0.65, 0.03},
		   {0.39, -0.65, -0.03},
		   {0.43, -0.59, -0.16},
		   {0.49, -0.5, -0.29},
		   {0.59, -0.39, -0.42},
		   {0.66, -0.25, -0.56},
		   {0.71, 0.05, -0.65},
		   {0.72, 0.5, -0.71},
		   {0.68, 0.7, -0.73},
		   {0.65, 0.8, -0.71}};
/*
float track[][3] = {{0.3, 0.5, 0.6},
		   {-0.6, -0.2, 0.1},
		   {-0.7, 0.4, 0.3},
		   {0.2, 0.1, -0.7}};
*/
int trackCounter[] = {1, 1, 1}; //counts which index in the track is next
int loopCount[] = {0, 0, 0};

// properties of some material
float mat_ambient[] = {0.05, 0.05, 0.05, 1.0};
float mat_diffuse[] = {0.75, 0.75, 0.75, 1.0};
float mat_specular[] = {1.0, 1.0, 1.0, 1.0};
float mat_shininess[] = {50.0};

//need 3 carts
float pos[][3] = {{track[0][0], track[0][1], track[0][2]},
		  {track[0][0], track[0][1], track[0][2]},
		  {track[0][0], track[0][1], track[0][2]}};
float dir[][3] = {{(track[1][0] - pos[0][0])/50,
	       (track[1][1] - pos[0][1])/50,
	       (track[1][2] - pos[0][2])/50},
	       {(track[1][0] - pos[1][0])/50,
	       (track[1][1] - pos[1][1])/50,
	       (track[1][2] - pos[1][2])/50},
	       {(track[1][0] - pos[2][0])/50,
	       (track[1][1] - pos[2][1])/50,
	       (track[1][2] - pos[2][2])/50}};
float speed = 1.0f;

bool g_moving = false;
bool g_coasting = false;

unsigned int g_track = 0;

void setNextTrack(int i) {
    trackCounter[i] = (trackCounter[i] + 1) % (sizeof(track)/sizeof(track[0]));
    loopCount[i]++; //loop count ignores when things loop
    dir[i][0] = track[trackCounter[i]][0] - pos[i][0];
    dir[i][1] = track[trackCounter[i]][1] - pos[i][1];
    dir[i][2] = track[trackCounter[i]][2] - pos[i][2];
    //normalise the direction
    float absoluteVal = (dir[i][0]*dir[i][0]) + (dir[i][1]*dir[i][1]) + (dir[i][2]*dir[i][2]);
    absoluteVal = sqrt(absoluteVal);
    //scale to a fiftieth of the speed
    dir[i][0] = dir[i][0]/(absoluteVal*50);
    dir[i][1] = dir[i][1]/(absoluteVal*50);
    dir[i][2] = dir[i][2]/(absoluteVal*50);
    /*
    float speed = sqrt((dir[0]*dir[0]) + (dir[1]*dir[1]) + (dir[2]*dir[2]));
    std::cout << "Speed for next section will be " << speed << std::endl;
    */
}

//method to check whether we are on to a new track position
void checkTrackPos(int i) {
    //make sure we haven't overshot the destination
    float tmp[] = {pos[i][0]+dir[i][0], pos[i][1]+dir[i][1], pos[i][2]+dir[i][2]};
    //calculate square of distance to next track point
    float dx = pos[i][0] - track[trackCounter[i]][0];
    float dy = pos[i][1] - track[trackCounter[i]][1];
    float dz = pos[i][2] - track[trackCounter[i]][2];
    float calcNow = (dx*dx) + (dy*dy) + (dz*dz);
    
    //calculate what it will be by next tick
    dx = tmp[0] - track[trackCounter[i]][0];
    dy = tmp[1] - track[trackCounter[i]][1];
    dz = tmp[2] - track[trackCounter[i]][2];
    float calcNext = (dx*dx) + (dy*dy) + (dz*dz);
    
    //if we are going to be further away, just snap to the next point
    if (calcNext > calcNow) {
	pos[i][0] = track[trackCounter[i]][0];
	pos[i][1] = track[trackCounter[i]][1];
	pos[i][2] = track[trackCounter[i]][2];
	setNextTrack(i);
    }
}

//method to move forward the index specified cart
void moveCart(int i) {
    pos[i][0] += dir[i][0]*speed;
    pos[i][1] += dir[i][1]*speed;
    pos[i][2] += dir[i][2]*speed;
    
    if (i == 0) {
	//change speed based on how much we should accelerate
	float dist = sqrt((dir[i][0]*dir[i][0]) + (dir[i][2]*dir[i][2]));
	float angle = atan(dir[i][1]/dist);
	float acceleration = (-sin(angle)*9.81)/60; 
	if (angle < 0.0f) acceleration - 0.05f;
	speed += acceleration;
	if (speed < 1.0f) speed = 1.0f; //always maintain minimum speed
	//std::cout << "Speed is currently " << speed << std::endl;
	
    }
    
}

void idle()
{
    usleep(100000);
    //move the coaster
    for (int i=0; i<sizeof(pos)/sizeof(pos[0]); i++) {
	moveCart(i);
	
	//make sure that the carts aren't too far apart and speed them up if they are
	//slow them down if they are too close
	float dx = pos[i][0] - pos[i+1][0];
	float dy = pos[i][1] - pos[i+1][1];
	float dz = pos[i][2] - pos[i+1][2];
	float dist =  sqrt((dx*dx)+(dy*dy)+(dz*dz));
	
	if (dist < 0.1f) {
	    //need to shift this one back a lot
	    pos[i+1][0] -= dir[i+1][0]*speed*3;
	    pos[i+1][1] -= dir[i+1][1]*speed*3;
	    pos[i+1][2] -= dir[i+1][2]*speed*3;
	} else if (dist < 0.2f) {
	    //need to shift this one back a bit
	    pos[i+1][0] -= dir[i+1][0]*speed*(2/3);
	    pos[i+1][1] -= dir[i+1][1]*speed*(2/3);
	    pos[i+1][2] -= dir[i+1][2]*speed*(2/3);
	} else if (dist > 0.4f) {
	    //need to shift this one forward a bit
	    pos[i+1][0] += dir[i+1][0]*speed;
	    pos[i+1][1] += dir[i+1][1]*speed;
	    pos[i+1][2] += dir[i+1][2]*speed;
	} else if (dist > 0.2f) {
	    //need to shift this one forward a bit
	    pos[i+1][0] += dir[i+1][0]*speed*(1/3);
	    pos[i+1][1] += dir[i+1][1]*speed*(1/3);
	    pos[i+1][2] += dir[i+1][2]*speed*(1/3);
	}
	checkTrackPos(i);
    }
    
    glutPostRedisplay();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 
    
    // position and orient camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (g_coasting) {
	gluLookAt(pos[0][0], pos[0][1]+0.2f, pos[0][2], // eye position
		  pos[0][0]+dir[0][0], pos[0][1]+0.2f+dir[0][1], pos[0][2]+dir[0][2], // reference point
		  0, 1, 0  // up vector
		);
    } else {
	gluLookAt(1, 1, 3, // eye position
			0, 0, 0, // reference point
			0, 1, 0  // up vector
	      );
    }

    glPushMatrix();	

	// set the surface properties
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	
	glDisable(GL_LIGHTING);

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	    glVertex3f(-1.0f, -1.0f, -1.0f);
	    glVertex3f(-1.0f, -1.0f, 1.0f);
	    glVertex3f(1.0f, -1.0f, 1.0f);
	    glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();
	
	
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	    glVertex3f(-0.8f, -0.9f, -1.0f);
	    glVertex3f(-0.6f, -0.9f, -1.0f);
	    glVertex3f(1.0f, -0.9f, 0.6f);
	    glVertex3f(1.0f, -0.9f, 0.8f);
	glEnd();
	glBegin(GL_QUADS);
	    glVertex3f(-1.0f, -0.9f, -0.8f);
	    glVertex3f(-1.0f, -0.9f, -0.6f);
	    glVertex3f(0.6f, -0.9f, 1.0f);
	    glVertex3f(0.8f, -0.9f, 1.0f);
	glEnd();
	
	

	
	//draw track
	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	int maxSize = sizeof(track)/sizeof(track[0]);
	for (unsigned int i=0; i < maxSize; i++) {
	    float tWid = 0.05f; //half of the track width
	    float rWid = 0.025f; //half of the rail width
	    int j = (i+1)%maxSize;
	    /*
	    for (int k=-1; k<3; k+=2) {
		//left track
		glBegin(GL_QUADS);
		    glVertex3f(track[i][0]+((tWid+rWid)*i), track[i][1]+rWid, track[i][2]);
		    glVertex3f(track[j][0]+((tWid+rWid)*i), track[j][1]+rWid, track[j][2]);
		    glVertex3f(track[j][0]+(tWid*i), track[j][1]+rWid, track[j][2]);
		    glVertex3f(track[i][0]+(tWid*i), track[i][1]+rWid, track[i][2]);
		glEnd();
		glBegin(GL_QUADS);
		    glVertex3f(track[i][0]+((tWid+rWid)*i), track[i][1]-rWid, track[i][2]);
		    glVertex3f(track[j][0]+((tWid+rWid)*i), track[j][1]-rWid, track[j][2]);
		    glVertex3f(track[j][0]+(tWid*i), track[j][1]-rWid, track[j][2]);
		    glVertex3f(track[i][0]+(tWid*i), track[i][1]-rWid, track[i][2]);
		glEnd();
		glBegin(GL_QUADS);
		    glVertex3f(track[i][0]+((tWid+rWid)*i), track[i][1]-rWid, track[i][2]);
		    glVertex3f(track[j][0]+((tWid+rWid)*i), track[j][1]-rWid, track[j][2]);
		    glVertex3f(track[j][0]+((tWid+rWid)*i), track[j][1]+rWid, track[j][2]);
		    glVertex3f(track[i][0]+((tWid+rWid)*i), track[i][1]+rWid, track[i][2]);
		glEnd();
		glBegin(GL_QUADS);
		    glVertex3f(track[i][0]+(tWid*i), track[i][1]-rWid, track[i][2]);
		    glVertex3f(track[j][0]+(tWid*i), track[j][1]-rWid, track[j][2]);
		    glVertex3f(track[j][0]+(tWid*i), track[j][1]+rWid, track[j][2]);
		    glVertex3f(track[i][0]+(tWid*i), track[i][1]+rWid, track[i][2]);
		glEnd();
	    }
	    
	    
	    glBegin(GL_QUADS);
		glVertex3f(track[i][0]-0.04f, track[i][1], track[i][2]-0.05f);
		glVertex3f(track[j][0]-0.038f, track[j][1], track[j][2]-0.05f);
		glVertex3f(track[j][0]+0.05f, track[j][1], track[j][2]+0.04f);
		glVertex3f(track[i][0]+0.05f, track[i][1], track[i][2]+0.038f);
	    glEnd();
	    
	    glBegin(GL_QUADS);
		glVertex3f(track[i][0]-0.05f, track[i][1], track[i][2]-0.04f);
		glVertex3f(track[j][0]-0.05f, track[j][1], track[j][2]-0.038f);
		glVertex3f(track[j][0]+0.038f, track[j][1], track[j][2]+0.05f);
		glVertex3f(track[i][0]+0.04f, track[i][1], track[i][2]+0.05f);
	    glEnd();
	    */
	    
	    /*
	    glBegin(GL_QUADS);
		glVertex3f(track[i][0]-0.05f, track[i][1], track[i][2]);
		glVertex3f(track[j][0]-0.05f, track[j][1], track[j][2]);
		glVertex3f(track[j][0]+0.05f, track[j][1], track[j][2]);
		glVertex3f(track[i][0]+0.05f, track[i][1], track[i][2]);
	    glEnd();
	    */
	    
	    
	    /*
	    // enable texturing
	    glEnable(GL_TEXTURE_2D); 
	    
	    //prep the texture
	    glBindTexture(GL_TEXTURE_2D, g_track);
	    
	    //need to calculate track length
	    float dx = track[i][0] - track[j][0];
	    float dy = track[i][1] - track[j][1];
	    float dz = track[i][2] - track[j][2];
	    float dist = sqrt((dx*dx)+(dy*dy)+(dz*dz));
	    dist = dist/10.0f;
	    // specify texture coordinates
	    glBegin (GL_QUADS);
		glTexCoord2f (0.0f,0.0f); // lower left corner
		glVertex3f(track[i][0]-0.05f, track[i][1], track[i][2]);
		glTexCoord2f (1.0f, 0.0f); // lower right corner
		glVertex3f(track[i][0]+0.05f, track[i][1], track[i][2]);
		glTexCoord2f (1.0f, 1.0f*dist); // upper right corner
		glVertex3f(track[j][0]+0.05f, track[j][1], track[j][2]);
		glTexCoord2f (0.0f, 1.0f*dist); // upper left corner
		glVertex3f(track[j][0]-0.05f, track[j][1], track[j][2]);
	    glEnd ();

	    glDisable(GL_TEXTURE_2D); 
	    */
	}
	
	
	
	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	
	glBegin(GL_LINE_LOOP);
	    for (unsigned int i=0; i < sizeof(track)/sizeof(track[0]); i++) {
		//need to figure out spot for left and right rail
		//just shifting along the x dimension
		
		
		glVertex3f(track[i][0], track[i][1], track[i][2]);
	    }
	glEnd();
	
	//draw the cart
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	for (int i=0; i<sizeof(pos)/sizeof(pos[0]); i++) {
	    glBegin(GL_LINE_LOOP);
		glVertex3f(pos[i][0]+0.02f, pos[i][1]+0.02f, pos[i][2]);
		glVertex3f(pos[i][0]-0.02f, pos[i][1], pos[i][2]-0.02f);
		glVertex3f(pos[i][0], pos[i][1]-0.02f, pos[i][2]+0.02f);
	    glEnd();
	}
	
	
	
	glEnable(GL_LIGHTING);

    glPopMatrix();
    glutSwapBuffers(); 
}

//spaces the carts out so that reversing doesn't occur
void spaceCarts() {
    for (int i=(sizeof(pos)/sizeof(pos[0]))-1; i>-1; i--) {
	
	//make sure that the carts aren't too far apart and speed them up if they are
	//slow them down if they are too close
	float dx = pos[i][0] - pos[i-1][0];
	float dy = pos[i][1] - pos[i-1][1];
	float dz = pos[i][2] - pos[i-1][2];
	float dist =  sqrt((dx*dx)+(dy*dy)+(dz*dz));
	
	while (dist < 0.2f) {
	    //need to move all carts in front of this one forward
	    //has to move front one last, since that is the one that informs speed
	    for (int j=i-1; j>-1; j--) {
		moveCart(j);
		checkTrackPos(j);
	    }
	    dx = pos[i][0] - pos[i-1][0];
	    dy = pos[i][1] - pos[i-1][1];
	    dz = pos[i][2] - pos[i-1][2];
	    dist =  sqrt((dx*dx)+(dy*dy)+(dz*dz));
	}
    }
}

void keyboard(unsigned char key, int, int)
{
    switch (key)
    {
	case 'q': exit(1); break;
	case 'p': for (int i=0; i<sizeof(pos)/sizeof(pos[0]); i++) {
		      pos[i][0] = track[trackCounter[0]][0];
		      pos[i][1] = track[trackCounter[0]][1];
		      pos[i][2] = track[trackCounter[0]][2];
		      setNextTrack(i);
		  }
		  //when positioned, space the carts out
		  spaceCarts();
		  glutPostRedisplay();
		  break;

	case ' ': g_moving = !g_moving;
		  if (g_moving)
			  glutIdleFunc(idle);
		  else
			  glutIdleFunc(NULL);
		  break;
	case 'z': g_coasting = !g_coasting;
    }
    glutPostRedisplay();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h); 
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    gluPerspective(40.0, 1.0f, 1.0, 5.0);

    glutPostRedisplay();
}

void init()
{
    float light_ambient[] = {0.1, 0.1, 0.1, 1.0};
    float light_diffuse[] = {0.5, 0.5, 0.5, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    float light_position[] = {1.0, 1.0, 2.0, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    g_track = load_and_bind_texture("./track.png");
    
    //make sure that the texture wraps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    GLenum error = glGetError();
    if (error!=GL_NO_ERROR)
	fprintf(stderr, "GL error %s\n", gluErrorString(error));

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    
    //make sure the carts are seperated first
    spaceCarts();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv); 
    
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH); 

    glutInitWindowSize(512, 512); 
    glutInitWindowPosition(50, 50); 

    glutCreateWindow("Rollercoaster"); 

    glutKeyboardFunc(keyboard); 
    glutReshapeFunc(reshape); 
    glutDisplayFunc(display); 

    init(); 

    glutMainLoop(); 

    return 0; 
}
